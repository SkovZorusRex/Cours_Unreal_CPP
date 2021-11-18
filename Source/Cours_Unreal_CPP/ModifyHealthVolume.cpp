// Fill out your copyright notice in the Description page of Project Settings.


#include "ModifyHealthVolume.h"

// Sets default values
AModifyHealthVolume::AModifyHealthVolume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Mesh"));
	Mesh->SetCollisionProfileName(TEXT("OverlapAll"));
	RootComponent = Mesh;

	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("My light"));
	Light->SetRelativeLocation(FVector(0.f, 0.f, 25.f));
	Light->SetupAttachment(RootComponent);

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AModifyHealthVolume::OnOverlapBegin);
	Mesh->OnComponentEndOverlap.AddDynamic(this, &AModifyHealthVolume::OnOverlapEnd);

}

void AModifyHealthVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		ACours_Unreal_CPPCharacter* character = Cast<ACours_Unreal_CPPCharacter>(OtherActor);
		if (!character)
			return;

		//GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, 2.f, true, 0.f);

		if (IsHealing)
		{
			character->ModifyHealth(Amount);
		}
		else
		{
			character->ModifyHealth(-Amount);
		}
		//stock varaible character dans le .h
		//call le timer -> character->Modifyhealth
		//OnEnd -> clear timer reset variable character

	}
}

void AModifyHealthVolume::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// Called when the game starts or when spawned
void AModifyHealthVolume::BeginPlay()
{
	Super::BeginPlay();

	if (IsHealing)
	{
		Light->SetLightColor(FColor::Green);
	}
	else
	{
		Light->SetLightColor(FColor::Red);
	}
	//TimerDel.BindUFunction(this, FName("DoModifyHealth"), Character);
}

// Called every frame
void AModifyHealthVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

