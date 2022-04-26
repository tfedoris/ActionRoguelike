// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "DrawDebugHelpers.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "SGameModeBase.h"

static TAutoConsoleVariable<bool> CVarDebugDrawCharacter(TEXT("su.CharacterDebugDraw"), false, TEXT("Enable Debug Lines for SCharacter."), ECVF_Cheat);

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	
	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	TimeToHitParamName = "TimeToHit";
	HitFlashColorParamName = "HitFlashColor";

	RageMultiplier = 2.0f;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
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

void ASCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void ASCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

void ASCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "MagicProjectile");
}

void ASCharacter::SpecialAttack()
{
	ActionComp->StartActionByName(this, "BlackHoleProjectile");
}

void ASCharacter::MovementAbility()
{
	ActionComp->StartActionByName(this, "TeleportProjectile");
}

void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float ActualDelta, float Delta)
{
	if (ActualDelta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
		GetMesh()->SetVectorParameterValueOnMaterials(HitFlashColorParamName, FVector(1.0f, 0.0f, 0.0f));

		OwningComp->ApplyRageChange(FMath::Abs(ActualDelta) * RageMultiplier);
	}
	
	if (NewHealth <= 0.0f && ActualDelta < 0.0f)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		DisableInput(PlayerController);
	}
}

FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
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

	if(CVarDebugDrawCharacter.GetValueOnGameThread())
	{
		VisualizePlayerRotation();
	}
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
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::SprintStop);
}

void ASCharacter::HealSelf(float Amount /* = 100 */)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}

