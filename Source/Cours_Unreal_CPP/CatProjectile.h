// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "CatProjectile.generated.h"

UCLASS()
class COURS_UNREAL_CPP_API ACatProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere)
		UMaterialInterface* DecalMat;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
		UProjectileMovementComponent* Projectile;
	// Sets default values for this actor's properties
	ACatProjectile();

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
