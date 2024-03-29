// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectCreedCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ReferenceCubeActor.h"
#include "GameplayTagsManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "TraversalComponent.h"
#include "MotionWarpingComponent.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AProjectCreedCharacter

AProjectCreedCharacter::AProjectCreedCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 600.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a MotionWarpingComponent
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));
	VaultMontage = ConstructorHelpers::FObjectFinder<UAnimMontage>(TEXT("C:/Users/nemju/OneDrive/Desktop/ProjectCreed/Parkour-Traversal-System/Content/Animations/AM_VaultAction")).Object;
	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}



void AProjectCreedCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	UWorld* World = GetWorld();
	

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	

}



//////////////////////////////////////////////////////////////////////////
// Input

void AProjectCreedCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AProjectCreedCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AProjectCreedCharacter::Look);

		// Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AProjectCreedCharacter::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AProjectCreedCharacter::Sprint);

		// Vaulting
		EnhancedInputComponent->BindAction(VaultAction, ETriggerEvent::Started, this, &AProjectCreedCharacter::Vault);
		EnhancedInputComponent->BindAction(VaultAction, ETriggerEvent::Completed, this, &AProjectCreedCharacter::StopVaulting);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AProjectCreedCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y*GetCharacterMovement()->MaxWalkSpeed);
		AddMovementInput(RightDirection, MovementVector.X*GetCharacterMovement()->MaxWalkSpeed);
	}
}

void AProjectCreedCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
void AProjectCreedCharacter::Sprint(const FInputActionValue& Value)
{
	// input is a float
	float SprintValue = Value.Get<float>();

	if (SprintValue > 0.0f)
	{
		GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	}
}
void AProjectCreedCharacter::StopSprinting(const FInputActionValue& Value)
{
	// input is a float
	float SprintValue = Value.Get<float>();

	if (SprintValue > 0.0f)
	{
		GetCharacterMovement()->MaxWalkSpeed = 340.0f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	}
}
void AProjectCreedCharacter::Vault(const FInputActionValue& Value)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AReferenceCubeActor::StaticClass(), FoundActors);
	float VaultValue = Value.Get<float>();
	if (VaultValue > 0.0f)
	{
		//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent() ->SetCapsuleSize(42.f, 45.0f);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		FName HeightWarpRef = TEXT("HeightWrapRef");
		FName DepthWarpRef = TEXT("DepthWrapRef");
		FName VaultWarpRef = TEXT("VaultWrapRef");
		for(AActor*Actors: FoundActors)	//Height Warp
		{
			if(Actors->ActorHasTag(HeightWarpRef))
			{
				if(FoundActors.Num()>0)
				{
					AActor* FirstActor = FoundActors[0];
					FVector ActorLocation = FirstActor->GetActorLocation();
					ActorLocation.Z+=-60.f;
					FRotator ActorRotation = FirstActor->GetActorRotation();
					MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation
					(HeightWarpRef,
					ActorLocation, 
					ActorRotation);

					GEngine->AddOnScreenDebugMessage(-1, 5.f,			//////////////
					FColor::Red, 										//////////////
					FString::Printf(TEXT("Actor Location: %s"), 		//////////////
					*ActorLocation.ToString()));						//////////////

					FName Tag = Actors->Tags[0];
					FString TagString = Tag.ToString();
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TagString);

					
				}

			}
			if(Actors->ActorHasTag(DepthWarpRef))	//Depth Warp
			{
				if(FoundActors.Num()>0)
				{
					AActor* FirstActor = FoundActors[0];
					FVector ActorLocation = FirstActor->GetActorLocation();
					ActorLocation.Z+=-45.f;
					FRotator ActorRotation = FirstActor->GetActorRotation();
					MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation
					(DepthWarpRef,
					ActorLocation, 
					ActorRotation);
				}
			}
			if(Actors->ActorHasTag(VaultWarpRef))	//Vault Warp
			{
				if(FoundActors.Num()>0)
				{
					AActor* FirstActor = FoundActors[0];
					FVector ActorLocation = FirstActor->GetActorLocation();
					ActorLocation.Z+=3.0f;
					FRotator ActorRotation = FirstActor->GetActorRotation();
					MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation
					(VaultWarpRef,
					ActorLocation, 
					ActorRotation);
				}
			}
		
			
			
		


		}

		//Mesh 
		USkeletalMeshComponent* CharacterMesh = GetMesh();
		UAnimInstance* AnimInstance = CharacterMesh->GetAnimInstance();
		if (AnimInstance && VaultMontage)
		{
			AnimInstance->Montage_Play(VaultMontage, 1.0f);
			CharacterMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Vaulting"));
		}

    }
	
	
	
	
	
	
	

}
void AProjectCreedCharacter::StopVaulting(const FInputActionValue& Value)
{

}