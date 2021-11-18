// Fill out your copyright notice in the Description page of Project Settings.


#include "Lava.h"

// Sets default values
ALava::ALava()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Mesh"));
	Mesh->SetCollisionProfileName(TEXT("OverlapAll"));
	Material = CreateDefaultSubobject<UMaterial>(TEXT("Material"));

	Mesh->SetMaterial(0, Material);
	RootComponent = Mesh;

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ALava::OnOverlapBegin);
	Mesh->OnComponentEndOverlap.AddDynamic(this, &ALava::OnOverlapEnd);
}

void ALava::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		ACours_Unreal_CPPCharacter* character = Cast<ACours_Unreal_CPPCharacter>(OtherActor);
		if (!character)
			return;
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("You are hitting : % s"), *OtherActor->GetName()));
		character->Kill();
	}
}

void ALava::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// Called when the game starts or when spawned
void ALava::BeginPlay()
{
	Super::BeginPlay();
	
	Mesh->SetMaterial(0, Material);
}

// Called every frame
void ALava::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

