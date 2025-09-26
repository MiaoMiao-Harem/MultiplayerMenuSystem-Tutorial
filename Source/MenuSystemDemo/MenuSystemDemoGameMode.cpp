// Copyright Epic Games, Inc. All Rights Reserved.

#include "MenuSystemDemoGameMode.h"

#include "MenuSystemDemoCharacter.h"
#include "UObject/ConstructorHelpers.h"


AMenuSystemDemoGameMode::AMenuSystemDemoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
