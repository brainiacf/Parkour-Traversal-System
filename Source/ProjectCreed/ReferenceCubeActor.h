#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReferenceCubeActor.generated.h"

class UStaticMeshComponent;
class UArrowComponent;


UCLASS()
class PROJECTCREED_API AReferenceCubeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AReferenceCubeActor();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* CubeMesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Mesh")
	UArrowComponent* ArrowComponentRed;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Mesh")
	UArrowComponent* ArrowComponentGreen;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Mesh")
	UArrowComponent* ArrowComponentBlue;
};
