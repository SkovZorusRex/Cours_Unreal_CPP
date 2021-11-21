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

void AModifyHealthVolume::DoModifyHealth()
{
	if (IsHealing)
	{
		Character->ModifyHealth(Amount);
	}
	else
	{
		Character->ModifyHealth(-Amount);
	}
}

void AModifyHealthVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		Character = Cast<ACours_Unreal_CPPCharacter>(OtherActor);
		if (!Character)
			return;

		GetWorldTimerManager().SetTimer(TimerHandle, this, &AModifyHealthVolume::DoModifyHealth, 2.f, true, 0.f);
	}
}

void AModifyHealthVolume::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GetWorldTimerManager().ClearTimer(TimerHandle);
	Character = nullptr;
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

