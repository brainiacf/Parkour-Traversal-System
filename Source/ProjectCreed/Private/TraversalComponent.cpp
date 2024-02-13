// Fill out your copyright notice in the Description page of Project Settings.


#include "TraversalComponent.h"

// Sets default values
ATraversalComponent::ATraversalComponent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATraversalComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATraversalComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

