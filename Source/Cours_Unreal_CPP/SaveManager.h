// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveMetadata.h"
#include "SaveInterface.h"
#include "GameFramework/SaveGame.h"
#include "SaveManager.generated.h"

/**
 * 
 */
UCLASS()
class COURS_UNREAL_CPP_API USaveManager : public USaveGame
{
	GENERATED_BODY()

private:

	// The current save slot
	static FString CurrentSaveSlot;

	// All the actors in the game which implement the save interface
	static TArray<TScriptInterface<ISaveInterface>> SaveInterfaces;

public:

	// Initialize the class, must be call when the game first launches
	static void Init();

	// Query all the actors in the game that implement the save interface
	static void QueryAllSaveInterfaces();

	// Saves the current state of the game
	UFUNCTION(BlueprintCallable, Category = "SavingAndLoading")
		static void SaveGame();
	
	// Loads the current state of the game
	UFUNCTION(BlueprintCallable, Category = "SavingAndLoading")
		static void LoadGame();

	// Deletes the specified slot
	UFUNCTION(BlueprintCallable, Category = "SavingAndLoading")
		static void DeleteSlot(const FString& slot);
	
	// Get a new, unused slot
	UFUNCTION(BlueprintPure, Category = "SavingAndLoading")
		static UPARAM(DisplayName = "Slot") FString GetNewSaveSlot(bool& slot_found);

	// Set the current save slot to the specified value
	UFUNCTION(BlueprintCallable, Category = "SavingAndLoading")
		static void SetCurrentSaveSlot(const FString& slot);

	// Get the current save slot to the specified value
	UFUNCTION(BlueprintPure, Category = "SavingAndLoading")
		static FString GetCurrentSaveSlot();

	// Get all the saved games
	UFUNCTION(BlueprintPure, Category = "SavingAndLoading")
		static TArray<FSaveMetadata> GetAllSaveMetadata();
};
