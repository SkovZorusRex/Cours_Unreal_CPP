// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerSave.generated.h"

/**
 * 
 */
UCLASS()
class COURS_UNREAL_CPP_API UPlayerSave : public USaveGame
{
	GENERATED_BODY()
	
public : 
	UPlayerSave();

	UPROPERTY(EditAnywhere)
		FVector PlayerLocation;
	UPROPERTY(EditAnywhere)
		float PlayerHealth;

	UFUNCTION(BlueprintCallable)
		static void SaveGame();

	UFUNCTION(BlueprintCallable)
		static void LoadGame();

	UFUNCTION(BlueprintCallable)
		static UPARAM(DisplayName = "Slot") FString GetSaveSlot(bool& slot_found);
};
