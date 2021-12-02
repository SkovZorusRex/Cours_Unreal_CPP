// Copyright Epic Games, Inc. All Rights Reserved.

#include "Cours_Unreal_CPPCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerSave.h"

//////////////////////////////////////////////////////////////////////////
// ACours_Unreal_CPPCharacter

ACours_Unreal_CPPCharacter::ACours_Unreal_CPPCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	ShootingPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ShootingPoint"));
	ShootingPoint->SetupAttachment(GetCapsuleComponent());
	ShootingPoint->SetRelativeLocation(FVector(120.f, -10.f, 40.f));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACours_Unreal_CPPCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACours_Unreal_CPPCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACours_Unreal_CPPCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACours_Unreal_CPPCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACours_Unreal_CPPCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACours_Unreal_CPPCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ACours_Unreal_CPPCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ACours_Unreal_CPPCharacter::OnResetVR);

	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &ACours_Unreal_CPPCharacter::Shoot);
	PlayerInputComponent->BindAction("PickUp", IE_Pressed, this, &ACours_Unreal_CPPCharacter::PickUp);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ACours_Unreal_CPPCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ACours_Unreal_CPPCharacter::StopCrouch);
}

// Called when the game starts or when spawned
void ACours_Unreal_CPPCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetActorLocation(PlayerLocation);
}


void ACours_Unreal_CPPCharacter::ModifyHealth(float _amount)
{
	if (Health + _amount >= 100 || Health + _amount <= 0)
	{
		if (_amount > 0)
		{
			Health = 100;
		}
		else
		{
			Health = 0;
			Kill();
		}
	}
	else
	{
		Health += _amount;
	}
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, _amount > 0 ? FColor::Green : FColor::Red, FString::Printf(TEXT("HEAL %f"), Health));
}

void ACours_Unreal_CPPCharacter::Kill()
{
	HandleDeath();
	auto MyMesh = GetMesh();
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->AttachToComponent(MyMesh, FAttachmentTransformRules::KeepWorldTransform, "pelvis");
	if (HoldedObject)
	{
		auto comp = HoldedObject->GetComponentByClass(UStaticMeshComponent::StaticClass());
		auto mesh = Cast<UStaticMeshComponent>(comp);
		if (!mesh)
			return;
		mesh->SetSimulatePhysics(true);
	}
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACours_Unreal_CPPCharacter::RespawnDelay, 5.f, false, 3.f);
}

void ACours_Unreal_CPPCharacter::OnResetVR()
{
	// If Cours_Unreal_CPP is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in Cours_Unreal_CPP.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ACours_Unreal_CPPCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ACours_Unreal_CPPCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ACours_Unreal_CPPCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACours_Unreal_CPPCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACours_Unreal_CPPCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACours_Unreal_CPPCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ACours_Unreal_CPPCharacter::Shoot()
{
		const FRotator SpawnRotation = ShootingPoint->GetComponentRotation();
		//const FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() + FVector(100.f, 0.f, 0.f); // Probleme spawn prend la camera pas le mesh
		const FVector SpawnLocation = ShootingPoint->GetComponentLocation();

		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GetWorld()->SpawnActor<ACatProjectile>(SpawnLocation, SpawnRotation, ActorSpawnParams);
}

void ACours_Unreal_CPPCharacter::PickUp()
{
	if (!HoldedObject)
	{
		FHitResult OutHit;
		FVector Start = FollowCamera->GetComponentLocation();

		FVector ForwardVector = FollowCamera->GetForwardVector();
		FVector End = ((ForwardVector * 650.f) + Start);
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);
		//CollisionParams.AddIgnoredActor(ECC_WorldStatic);

		DrawDebugLine(GetWorld(), Start, End, FColor::Red, true);

		//https://www.youtube.com/watch?v=50h2U_d1XVc
		//if (GetWorld()->LineTraceSingleByObjectType(OutHit, Start, End, ECC_PhysicsBody, CollisionParams))
		if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_PhysicsBody, CollisionParams))
		{
			if (OutHit.bBlockingHit)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("You are hitting : % s"), *OutHit.GetActor()->GetName()));
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Purple, FString::Printf(TEXT("Impact Point : % s"), *OutHit.ImpactPoint.ToString()));
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("Normal Point : % s"), *OutHit.ImpactNormal.ToString()));

				HoldedObject = Cast<AActor>(OutHit.Actor);
				if (!HoldedObject)
					return;
				auto comp = HoldedObject->GetComponentByClass(UStaticMeshComponent::StaticClass());
				auto mesh = Cast<UStaticMeshComponent>(comp);
				if (!mesh)
					return;
				mesh->SetSimulatePhysics(false);
				HoldedObject->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
				HoldedObject->SetActorRelativeLocation(FVector(120.f, 0.f, 0.f));
			}
		}
	}
	else
	{
		HoldedObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		auto comp = HoldedObject->GetComponentByClass(UStaticMeshComponent::StaticClass());
		auto mesh = Cast<UStaticMeshComponent>(comp);
		if (!mesh)
			return;
		mesh->SetSimulatePhysics(true);
		HoldedObject = nullptr;
	}
}

void ACours_Unreal_CPPCharacter::HandleDeath()
{
	auto MyCapsule = GetCapsuleComponent();
	auto MyMesh = GetMesh();
	MyCapsule->SetCollisionProfileName("NoCollision");
	MyMesh->SetSimulatePhysics(true);
	MyMesh->SetCollisionProfileName("Ragdoll");
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void ACours_Unreal_CPPCharacter::StartCrouch()
{
		Crouch();
}

void ACours_Unreal_CPPCharacter::StopCrouch()
{
		UnCrouch();
}

void ACours_Unreal_CPPCharacter::RespawnDelay()
{
	auto Gamemode = Cast<ACours_Unreal_CPPGameMode>(GetWorld()->GetAuthGameMode());
	if (!Gamemode)
		return;
	Gamemode->OnPlayerKilled();
	Destroy();
	GetWorldTimerManager().ClearTimer(TimerHandle);
}

FString ACours_Unreal_CPPCharacter::GetUniqueSaveName_Implementation()
{
	return "Player";
}

void ACours_Unreal_CPPCharacter::OnBeforeSave_Implementation()
{
	PlayerLocation = GetActorLocation();
}
