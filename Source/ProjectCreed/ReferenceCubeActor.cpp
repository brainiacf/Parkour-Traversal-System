#include "ReferenceCubeActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "UObject/ConstructorHelpers.h"



AReferenceCubeActor::AReferenceCubeActor()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	// CUBE MESH
	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	RootComponent = CubeMesh;
	// RED ARROW
	ArrowComponentRed = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponentRed"));
	ArrowComponentRed->ArrowColor = FColor::Red;
	ArrowComponentRed->ArrowSize = 10.0f;
	ArrowComponentRed->bHiddenInGame = false;
	ArrowComponentRed->SetupAttachment(CubeMesh);
	// GREEN ARROW
	ArrowComponentGreen = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponentGreen"));
	ArrowComponentGreen->ArrowColor = FColor::Green;
	ArrowComponentGreen->ArrowSize = 10.0f;
	ArrowComponentGreen->bHiddenInGame = false;
	ArrowComponentGreen->SetupAttachment(CubeMesh);
	ArrowComponentGreen->SetWorldRotation(FRotator(0.0f,90.0f,0.0f));
	// BLUE ARROW
	ArrowComponentBlue = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponentBlue"));
	ArrowComponentBlue->ArrowColor = FColor::Blue;
	ArrowComponentBlue->ArrowSize = 10.0f;
	ArrowComponentBlue->bHiddenInGame = false;
	ArrowComponentBlue->SetupAttachment(CubeMesh);
	ArrowComponentBlue->SetWorldRotation(FRotator(90.0f,0.0f,0.0f));
	



	CubeMesh->SetWorldScale3D(FVector(0.1f, 0.1f, 0.1f));
}


void AReferenceCubeActor::BeginPlay()
{
	Super::BeginPlay();
	
}


void AReferenceCubeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

