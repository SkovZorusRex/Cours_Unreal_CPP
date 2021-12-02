// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
//#include "Cours_Unreal_CPP/Cours_Unreal_CPPCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Cours_Unreal_CPPGameMode.generated.h"

UCLASS(minimalapi)
class ACours_Unreal_CPPGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACours_Unreal_CPPGameMode();

	virtual void InitGameState() override;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;

	void OnPlayerKilled();
};



