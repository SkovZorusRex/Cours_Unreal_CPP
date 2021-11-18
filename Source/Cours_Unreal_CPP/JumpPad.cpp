// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpPad.h"
#include "GameFrameWork/Character.h"

// Sets default values
AJumpPad::AJumpPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Mesh"));
	Mesh->SetSimulatePhysics(true);

	RootComponent = Mesh;

	Mesh->OnComponentHit.AddDynamic(this, &AJumpPad::OnHit);

}

void AJumpPad::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
		ACharacter* character = Cast<ACharacter>(OtherActor);
		if (!character)
			return;

		character->LaunchCharacter(Force, false, true);
}

// Called when the game starts or when spawned
void AJumpPad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJumpPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

