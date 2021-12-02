// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveManager.h"
#include "SaveGameMetadata.h"
#include "SaveGameData.h"
#include "Kismet/GameplayStatics.h"

FString USaveManager::CurrentSaveSlot;
TArray<TScriptInterface<ISaveInterface>> USaveManager::SaveInterfaces;


static const FString kMetadataSaveSlot = "SaveGameMetadata";
static const int32 kMaxSaveSlot = 10;

void USaveManager::Init()
{
	CurrentSaveSlot = "Default";

	// Make sure the metadata file exist in case the game has never been ran
	USaveGame* saveGameMetadata = UGameplayStatics::LoadGameFromSlot(kMetadataSaveSlot, 0);

	if (saveGameMetadata == nullptr)
	{
		// Since the metadata file doesn't exist, we need to create one
		USaveGame* saveGameObject = UGameplayStatics::CreateSaveGameObject(USaveGameMetadata::StaticClass());

		UGameplayStatics::SaveGameToSlot(saveGameObject, kMetadataSaveSlot, 0);
	}
}

void USaveManager::QueryAllSaveInterfaces()
{
	// Clearing old entries
	SaveInterfaces.Empty();

	// Get all the actors that implement the save interface
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsWithInterface(GWorld, USaveInterface::StaticClass(), actors);

	for (AActor* actor : actors)
	{
		SaveInterfaces.Add(actor);
	}
}

void USaveManager::SaveGame()
{
	// Create a new save game data instance
	USaveGameData* saveGameData = Cast<USaveGameData>(UGameplayStatics::CreateSaveGameObject(USaveGameData::StaticClass()));

	// Go over all the actors that need to be saved and save them
	for (auto& saveInterface : SaveInterfaces)
	{
		if (saveInterface.GetObject() == nullptr)
			continue;

		//Let the object know that it's about to be saved
		saveInterface->Execute_OnBeforeSave(saveInterface.GetObject());

		// Find the object's save data using it's unique name
		FString uniqueSaveName = saveInterface->Execute_GetUniqueSaveName(saveInterface.GetObject());
		FSaveData& saveData = saveGameData->SerializedData.Add(uniqueSaveName);

		FMemoryWriter memoryWriter = FMemoryWriter(saveData.Data);
		memoryWriter.ArIsSaveGame = true;

		saveInterface.GetObject()->Serialize(memoryWriter);
	}

	// Save the game to the current slot
	UGameplayStatics::SaveGameToSlot(saveGameData, CurrentSaveSlot, 0);

	// Update the metadata file with the new slot
	USaveGameMetadata* saveGameMetadata = Cast<USaveGameMetadata>(UGameplayStatics::LoadGameFromSlot(kMetadataSaveSlot, 0));

	FSaveMetadata& saveMetadata = saveGameMetadata->SavedGamesMetadata.FindOrAdd(CurrentSaveSlot);
	saveMetadata.SlotName = CurrentSaveSlot;
	saveMetadata.Date = FDateTime::Now();

	// Save the changes to the metadata file
	UGameplayStatics::SaveGameToSlot(saveGameMetadata, kMetadataSaveSlot, 0);
	
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, "Saved: " + CurrentSaveSlot);
}

void USaveManager::LoadGame()
{
	USaveGameData* saveGameData = Cast<USaveGameData>(UGameplayStatics::LoadGameFromSlot(CurrentSaveSlot, 0));

	if (saveGameData == nullptr)
	{
		// No saves exist yet for this slot. Save a default one
		SaveGame();

		//Reload it
		saveGameData = Cast<USaveGameData>(UGameplayStatics::LoadGameFromSlot(CurrentSaveSlot, 0));
	}

	// Loop over all the actors that need to load data and load their data
	for (auto& saveInterface : SaveInterfaces)
	{
		if (saveInterface.GetObject() == nullptr)
			continue;

		// Find the object's save data using it's unique name
		FString uniqueSaveName = saveInterface->Execute_GetUniqueSaveName(saveInterface.GetObject());
		FSaveData* saveData = saveGameData->SerializedData.Find(uniqueSaveName);

		if (saveData == nullptr)
			continue;

		//Deserialize the object's save data
		FMemoryReader memoryReader(saveData->Data);
		memoryReader.ArIsSaveGame = false;
		
		saveInterface.GetObject()->Serialize(memoryReader);
	}

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, "Loaded: " + CurrentSaveSlot);
}

void USaveManager::DeleteSlot(const FString& slot)
{
	//Delete the slot
	UGameplayStatics::DeleteGameInSlot(slot, 0);

	// Loading the metadata file
	USaveGameMetadata* saveGameMetadata = Cast<USaveGameMetadata>(UGameplayStatics::LoadGameFromSlot(kMetadataSaveSlot, 0));

	saveGameMetadata->SavedGamesMetadata.Remove(slot);

	// Save the metadata slot
	UGameplayStatics::SaveGameToSlot(saveGameMetadata, kMetadataSaveSlot, 0);

}

FString USaveManager::GetNewSaveSlot(bool& slot_found)
{
	slot_found = false;

	// Loading the metadata file
	USaveGameMetadata* saveGameMetadata = Cast<USaveGameMetadata>(UGameplayStatics::LoadGameFromSlot(kMetadataSaveSlot, 0));

	for (int32 i = 0; i < kMaxSaveSlot; i++)
	{
		FString slotName = "SaveSlot" + FString::FromInt(i);

		if (saveGameMetadata->SavedGamesMetadata.Contains(slotName) == false)
		{
			// We found a free slot. Return it
			slot_found = true;
			return slotName;
		}
	}
	return FString();
}

void USaveManager::SetCurrentSaveSlot(const FString& slot)
{
	CurrentSaveSlot = slot;
}

FString USaveManager::GetCurrentSaveSlot()
{
	return CurrentSaveSlot;
}

TArray<FSaveMetadata> USaveManager::GetAllSaveMetadata()
{
	TArray<FSaveMetadata> metadata;

	// Loading the metadata file
	USaveGameMetadata* saveGameMetadata = Cast<USaveGameMetadata>(UGameplayStatics::LoadGameFromSlot(kMetadataSaveSlot, 0));

	metadata.Reserve(saveGameMetadata->SavedGamesMetadata.Num());

	// Add each saved game's metadata to the return array
	for (const auto& saved_game : saveGameMetadata->SavedGamesMetadata)
	{
		metadata.Push(saved_game.Value);
	}

	return metadata;
}
