// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/PointLightComponent.h"
#include "Cours_Unreal_CPP/Cours_Unreal_CPPCharacter.h"
#include "ModifyHealthVolume.generated.h"


UCLASS()
class COURS_UNREAL_CPP_API AModifyHealthVolume : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditAnywhere)
		bool IsHealing;

	UPROPERTY(EditAnywhere)
		float Amount;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	UPointLightComponent* Light;
	//FTimerHandle TimerHandle;
	//FTimerDelegate TimerDel;
	//ACours_Unreal_CPPCharacter* Character;

	// Sets default values for this actor's properties
	AModifyHealthVolume();

	/*UFUNCTION()
		void DoModifyHealth(ACours_Unreal_CPPCharacter* character);*/

	// declare overlap begin function
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// declare overlap end function
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
