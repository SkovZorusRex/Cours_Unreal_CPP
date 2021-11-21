// Fill out your copyright notice in the Description page of Project Settings.


#include "CatProjectile.h"

// Sets default values
ACatProjectile::ACatProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> CatSKMesh(TEXT("SkeletalMesh'/Game/LowPoly_Cat/Meshes/Mesh_Cat_Red_White.Mesh_Cat_Red_White'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CatMesh(TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> CatDecal(TEXT("Material'/Game/ThirdPersonCPP/Materials/M_CatDecal.M_CatDecal'"));

	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("My Projectile"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Mesh"));
	Mesh->SetCollisionProfileName("BlockAll");
	if (CatMesh.Succeeded())
	{
		Mesh->SetStaticMesh(CatMesh.Object);
	}
	if (CatDecal.Succeeded())
	{
		DecalMat = CatDecal.Object;
	}

	Mesh->SetWorldScale3D(FVector(0.3f, 0.3f, 0.3f));

	/*if (CatSKMesh.Succeeded())
	{
		USkeletalMeshComponent* characterMesh = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));
		if (!characterMesh)
			return;
		characterMesh->SetSkeletalMesh(CatSKMesh.Object);
	}*/
	//Mesh->SetCollisionProfileName(TEXT("OverlapAll"));
	RootComponent = Mesh;
	Projectile->Velocity = FVector(1000.f,0.f,0.f);

	Mesh->OnComponentHit.AddDynamic(this, &ACatProjectile::OnHit);

}

void ACatProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FVector UpVector = GetActorUpVector();
	FVector NormalVector = Hit.ImpactNormal;

	FVector RotationAxis = FVector::CrossProduct(UpVector, NormalVector);
	RotationAxis.Normalize();

	float DotProduct = FVector::DotProduct(UpVector, NormalVector);
	float RotationAngle = acosf(DotProduct);

	FQuat Quat = FQuat(RotationAxis, RotationAngle);
	FQuat RootQuat = GetActorQuat();

	FQuat NewQuat = Quat * RootQuat;

	ADecalActor* decal = GetWorld()->SpawnActor<ADecalActor>(Hit.ImpactPoint, NewQuat.Rotator());
	decal->SetDecalMaterial(DecalMat);
	decal->SetLifeSpan(2.0f);
	decal->GetDecal()->DecalSize = FVector(100.f, 100.f, 100.f);
	Destroy();
}

// Called when the game starts or when spawned
void ACatProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACatProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

