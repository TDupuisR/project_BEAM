// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BeamCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Characters/BeamCharacterStateMachine.h"

#include "Characters/BeamCharacterSettings.h"

#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ABeamCharacter::ABeamCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABeamCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitCharacterSettings();
	CreateStateMachine();
	InitStateMachine();

	
}

// Called every frame
void ABeamCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickStateMachine(DeltaTime);
	RotateMeshUsingOrientX();


}

// Called to bind functionality to input
void ABeamCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ABeamCharacter::GetOrientX() const
{
	return OrientX;
}

void ABeamCharacter::SetOrientX(float NewOrientX)
{
	OrientX = NewOrientX;
}

void ABeamCharacter::RotateMeshUsingOrientX() const
{
	FRotator Rotation = GetMesh()->GetRelativeRotation();

	Rotation.Yaw = -90.f * OrientX;
	GetMesh()->SetRelativeRotation(Rotation);
}

void ABeamCharacter::CreateStateMachine()
{
	StateMachine = NewObject<UBeamCharacterStateMachine>(this);
}

void ABeamCharacter::InitStateMachine()
{
	if (StateMachine == nullptr) return;
	StateMachine->Init(this);
}

void ABeamCharacter::TickStateMachine(float DeltaTime) const
{
	if (StateMachine == nullptr) return;
	StateMachine->Tick(DeltaTime);
}

void ABeamCharacter::InitCharacterSettings()
{
	CharacterSettings = GetDefault<UBeamCharacterSettings>();

	if (CharacterSettings == nullptr) return;

	GetCharacterMovement()->MaxAcceleration = CharacterSettings->MaxAcceleration;
	GetCharacterMovement()->GroundFriction = CharacterSettings->GroundFriction;
	GetCharacterMovement()->GravityScale = CharacterSettings->GravityScale;
	GetCharacterMovement()->Mass = CharacterSettings->Mass;
	GetCharacterMovement()->BrakingDecelerationWalking = CharacterSettings->BreakingDecelerationWalking;
	GetCharacterMovement()->JumpZVelocity = CharacterSettings->Jump_Force;
	GetCharacterMovement()->AirControl = CharacterSettings->AirControl;
	GetCharacterMovement()->FallingLateralFriction = CharacterSettings->FallingLateralFriction;
	GetCharacterMovement()->MaxFlySpeed = CharacterSettings->Fly_MaxSpeed;
}

const UBeamCharacterSettings* ABeamCharacter::GetCharacterSettings() const
{
	return CharacterSettings;
}

