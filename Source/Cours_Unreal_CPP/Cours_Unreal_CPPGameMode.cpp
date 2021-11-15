// Copyright Epic Games, Inc. All Rights Reserved.

#include "Cours_Unreal_CPPGameMode.h"
#include "Cours_Unreal_CPPCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACours_Unreal_CPPGameMode::ACours_Unreal_CPPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
