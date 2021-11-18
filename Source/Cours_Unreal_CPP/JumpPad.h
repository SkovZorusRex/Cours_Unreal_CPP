// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "JumpPad.generated.h"

UCLASS()
class COURS_UNREAL_CPP_API AJumpPad : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditAnywhere)
		FVector Force = { 0.f,0.f,1000.f };

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	// Sets default values for this actor's properties
	AJumpPad();

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
