// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpPad.h"

// Sets default values
AJumpPad::AJumpPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionProfileName(TEXT("OverlapAll"));
	RootComponent = Mesh;

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AJumpPad::OnOverlapBegin);
	Mesh->OnComponentEndOverlap.AddDynamic(this, &AJumpPad::OnOverlapEnd);

}

void AJumpPad::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Name %s"), *OtherActor->GetName()));
	}
}

void AJumpPad::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "OverlapEnd");
	}
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

