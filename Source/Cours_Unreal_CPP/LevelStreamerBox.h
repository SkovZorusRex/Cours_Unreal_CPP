// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/LevelStreaming.h"
#include "LevelStreamerBox.generated.h"

UCLASS()
class COURS_UNREAL_CPP_API ALevelStreamerBox : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		FName LevelToLoad;
	
	UPROPERTY(VisibleAnywhere, Category = "BoxComp")
		class UBoxComponent* BoxComp;

	FLatentActionInfo LatentInfo;
	ULevelStreaming* levelLoaded = nullptr;

public:	
	// Sets default values for this actor's properties
	ALevelStreamerBox();

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
