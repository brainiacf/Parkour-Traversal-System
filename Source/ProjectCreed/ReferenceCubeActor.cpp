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
	ArrowComponentRed->bHiddenInGame = false;
	ArrowComponentRed->SetupAttachment(CubeMesh);
	// GREEN ARROW
	ArrowComponentGreen = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponentGreen"));
	ArrowComponentGreen->ArrowColor = FColor::Green;
	ArrowComponentGreen->bHiddenInGame = false;
	ArrowComponentGreen->SetupAttachment(CubeMesh);
	// BLUE ARROW
	ArrowComponentBlue = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponentBlue"));
	ArrowComponentBlue->ArrowColor = FColor::Blue;
	ArrowComponentBlue->bHiddenInGame = false;
	ArrowComponentBlue->SetupAttachment(CubeMesh);
	



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

