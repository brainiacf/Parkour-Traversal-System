// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectCreedGameMode.h"
#include "ProjectCreedCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectCreedGameMode::AProjectCreedGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
