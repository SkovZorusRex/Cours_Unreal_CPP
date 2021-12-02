// Copyright Epic Games, Inc. All Rights Reserved.

#include "Cours_Unreal_CPPGameMode.h"
#include "SaveManager.h"
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

void ACours_Unreal_CPPGameMode::InitGameState()
{
	USaveManager::QueryAllSaveInterfaces();
	USaveManager::LoadGame();

	Super::InitGameState();
}

APawn* ACours_Unreal_CPPGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	APawn* Pawn = Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);

	USaveManager::QueryAllSaveInterfaces();
	USaveManager::LoadGame();

	return Pawn;
}

void ACours_Unreal_CPPGameMode::OnPlayerKilled()
{
	auto Controller = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetController();
	//auto Controller = Player->GetController();
	Controller->UnPossess();
	RestartPlayer(Controller);
}
