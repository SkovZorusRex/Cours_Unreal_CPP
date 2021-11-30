// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelStreamerBox.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

// Sets default values
ALevelStreamerBox::ALevelStreamerBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("MyBoxComponent"));
	BoxComp->InitBoxExtent(FVector(150, 100, 100));
	BoxComp->SetCollisionProfileName("Trigger");
	RootComponent = BoxComp;

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ALevelStreamerBox::OnOverlapBegin);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &ALevelStreamerBox::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ALevelStreamerBox::BeginPlay()
{
	Super::BeginPlay();

	DrawDebugBox(GetWorld(), GetActorLocation(), BoxComp->GetScaledBoxExtent(), FQuat(GetActorRotation()), FColor::Turquoise, true, -1, 0, 2);
}

// Called every frame
void ALevelStreamerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelStreamerBox::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	levelLoaded = UGameplayStatics::GetStreamingLevel(GetWorld(), LevelToLoad);

	if (levelLoaded->IsLevelVisible())
	{
		UGameplayStatics::UnloadStreamLevel(this, LevelToLoad, LatentInfo, false);
	}
	else
	{
		UGameplayStatics::LoadStreamLevel(this, LevelToLoad, true, false, LatentInfo);
	}
}

void ALevelStreamerBox::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}
