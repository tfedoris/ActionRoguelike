// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "SGameModeBase.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

	ProjectileAttackSocketName = FName("Muzzle_01");

	AimRange = 100000.f;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	CastParticleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>("CastParticleSystemComp");
	CastParticleSystemComp->SetAutoActivate(false);
	CastParticleSystemComp->SetupAttachment(GetMesh(), ProjectileAttackSocketName);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	GameMode = GetWorld()->GetAuthGameMode<ASGameModeBase>();
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	
	AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// X = Forward (Red)
	// Y = Right (Green)
	// Z = Up (Blue)

	const FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(RightVector, Value);
}

void ASCharacter::GetAimStartAndEnd(FVector& Start, FVector& End)
{
	FVector CameraLocation;
	FRotator CameraRotation;
	GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);
	FVector Target = CameraLocation + (CameraRotation.Vector() * AimRange);

	Start = CameraLocation;
	End = Target;
}

void ASCharacter::PrimaryAttack()
{
	GetAimStartAndEnd(AimStart, AimEnd);

	CastParticleSystemComp->Activate();
	PlayAnimMontage(AttackAnim);
	
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, [&](){ this->ProjectileAttack(PrimaryProjectileClass); }, 0.2f, false);
}

void ASCharacter::SpecialAttack()
{
	GetAimStartAndEnd(AimStart, AimEnd);

	CastParticleSystemComp->Activate();
	PlayAnimMontage(AttackAnim);
	
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, [&](){ this->ProjectileAttack(SpecialProjectileClass); }, 0.2f, false);
}

void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void ASCharacter::MovementAbility()
{
	GetAimStartAndEnd(AimStart, AimEnd);

	CastParticleSystemComp->Activate();
	PlayAnimMontage(AttackAnim);
	
	GetWorldTimerManager().SetTimer(TimerHandle_MovementAbility, [&](){ this->ProjectileAttack(MovementProjectileClass); }, 0.2f, false);
}

void ASCharacter::ProjectileAttack(TSubclassOf<AActor> ProjectileClass)
{
	CastParticleSystemComp->Deactivate();
	FVector PrimaryAttackSpawnLocation = GetMesh()->GetSocketLocation(ProjectileAttackSocketName);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	FHitResult Hit;
	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, AimStart, AimEnd, ObjectQueryParams);
	
	if (bBlockingHit && Hit.GetActor() != this)
	{
		AimEnd = Hit.ImpactPoint;
		if (GameMode->bShowDebugLines)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 60.0f, 32, FColor::Green, false, 2.0f);
		}
	}
	
	FRotator RotateTowards = FRotationMatrix::MakeFromX(AimEnd - PrimaryAttackSpawnLocation).Rotator();
	
	if (GameMode->bShowDebugLines)
	{
		DrawDebugLine(GetWorld(), PrimaryAttackSpawnLocation, AimEnd, FColor::Red, false, 4.f, 0, 2.f);
	}
	
	FTransform SpawnTM = FTransform(RotateTowards, PrimaryAttackSpawnLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);

	MoveIgnoreActorAdd(SpawnedActor);
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
		GetMesh()->SetVectorParameterValueOnMaterials("HitFlashColor", FVector(1.0f, 0.0f, 0.0f));
	}
	
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		DisableInput(PlayerController);
	}
}

void ASCharacter::VisualizePlayerRotation() const
{
	// -- Rotation Visualization -- //
	constexpr float DrawScale = 100.0f;
	constexpr float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();
	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in direction of the actor's forward
	const FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	const FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	VisualizePlayerRotation();
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("SpecialAttack", IE_Pressed, this, &ASCharacter::SpecialAttack);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);
	PlayerInputComponent->BindAction("MovementAbility", IE_Pressed, this, &ASCharacter::MovementAbility);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
}

