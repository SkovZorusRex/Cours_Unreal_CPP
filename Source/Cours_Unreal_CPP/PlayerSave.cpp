// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSave.h"

UPlayerSave::UPlayerSave()
{
	PlayerLocation = FVector(0.f, 0.f, 0.f);
	PlayerHealth = 100.f;
}

void UPlayerSave::SaveGame()
{
}

void UPlayerSave::LoadGame()
{
}

FString UPlayerSave::GetSaveSlot(bool& slot_found)
{
	return FString();
}
