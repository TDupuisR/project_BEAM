// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BeamCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Characters/BeamCharacterStateMachine.h"
#include "Characters/BeamCharacterSettings.h"
#include "Characters/PlayerAim.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/BeamCharacterStateID.h"
#include "Components/BoxComponent.h"


// Sets default values
ABeamCharacter::ABeamCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

EProjectileType ABeamCharacter::ProjectileGetType()
{
	return EProjectileType::Player;
}

bool ABeamCharacter::ProjectileContext(int power, FVector position)
{
	TakeDamage(power+1);

	FVector direction = GetActorLocation() - position;
	direction.Normalize();
	
	KnockBack(direction, (power+1)*500.f); // Magic Number for the force, to dertemine how to tweak it
	
	return true;
}

AProjectile* ABeamCharacter::GetProjectile()
{
	return nullptr;
}

// Called when the game starts or when spawned
void ABeamCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//GetComponentByClass<UPlayerAim>()->Character = this;
	
	InitCharacterSettings();
	SetupCollision();
	CreateStateMachine();
	InitStateMachine();

	StartLocation = this->GetActorLocation();
}

// Called every frame
void ABeamCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TickStateMachine(DeltaTime);
	RotateMeshUsingOrientX();

	TickPush(DeltaTime);

	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, FString::Printf(TEXT("WOWWWW : %d"), InputMappingContext));

	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, GetName());

	if (GetActorLocation().Y != StartLocation.Y)
	{
		SetActorLocation(FVector(GetActorLocation().X, StartLocation.Y, GetActorLocation().Z));
	}
}

// Called to bind functionality to input
void ABeamCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupMappingContextIntoController();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if(EnhancedInputComponent == nullptr) return;

	BindInputActions(EnhancedInputComponent);
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

	// CHARACTER COMPONENT SETTINGS
	GetCharacterMovement()->MaxAcceleration = CharacterSettings->MaxAcceleration;
	GetCharacterMovement()->GroundFriction = CharacterSettings->GroundFriction;
	GetCharacterMovement()->GravityScale = CharacterSettings->GravityScale;
	GetCharacterMovement()->Mass = CharacterSettings->Mass;
	GetCharacterMovement()->BrakingDecelerationWalking = CharacterSettings->BreakingDecelerationWalking;
	GetCharacterMovement()->JumpZVelocity = CharacterSettings->Jump_Force;
	GetCharacterMovement()->AirControl = CharacterSettings->AirControl;
	GetCharacterMovement()->FallingLateralFriction = CharacterSettings->FallingLateralFriction;
	GetCharacterMovement()->MaxFlySpeed = CharacterSettings->Fly_MaxSpeed;
	
	// CHARACTER STATS SETTINGS
	MaxLife = CharacterSettings->MaxLife;
	Life = MaxLife;
	LifeToFly = CharacterSettings->LifeToFly;
	timeToWaitPush = CharacterSettings->Push_Cooldown;
}

void ABeamCharacter::KnockBack(FVector Direction, float Force)
{
	this->GetCharacterMovement()->Launch(Direction * Force);
}

int const ABeamCharacter::GetLife() const
{
	return Life;
}

int const ABeamCharacter::GetMaxLife() const
{
	return MaxLife;
}

int const ABeamCharacter::GetLifeToFly() const
{
	return LifeToFly;
}

void ABeamCharacter::SetLife(const int NewLife)
{
	Life = NewLife;
}

void const ABeamCharacter::SetMaxLife(const int NewMaxLife)
{
	MaxLife = NewMaxLife;
}

void const ABeamCharacter::SetLifeToFly(const int NewLifeToFly)
{
	LifeToFly = NewLifeToFly;
}

void ABeamCharacter::TakeDamage(const int Damage)
{
	Life -= Damage;
	if (Life <= 0) {
		Life = 0;
	}
	CheckLife();
}

void const ABeamCharacter::ResetLife()
{
	Life = MaxLife;
}

bool ABeamCharacter::IsPhaseTwo() const
{
	return Life <= LifeToFly;
}

void ABeamCharacter::CheckLife()
{
	if (Life > 0 && Life <= LifeToFly) {
		if (StateMachine->GetCurrentStateID() != EBeamCharacterStateID::Fly) {
			StateMachine->ChangeState(EBeamCharacterStateID::Fly);
		}
	}
	else if (Life > LifeToFly) {
		if (StateMachine->GetCurrentStateID() != EBeamCharacterStateID::Idle) {
			StateMachine->ChangeState(EBeamCharacterStateID::Idle);
		}
	}
	else {
		StateMachine->ChangeState(EBeamCharacterStateID::Dead);
	}
}

void ABeamCharacter::Push()
{
	if (PlayersInZone.Num() == 0 || CharacterSettings == nullptr) return;

	for (ABeamCharacter* player : PlayersInZone) {
		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Emerald, FString::Printf(TEXT("Push Zone Detect")));
		FVector direction = (player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		player->KnockBack(direction, CharacterSettings->Push_Force);
	}
}

bool ABeamCharacter::CanPush() const
{
	return canPush;
}

void ABeamCharacter::SetCanPush(bool NewCanPush)
{
	canPush = NewCanPush;
}

void ABeamCharacter::TickPush(float DeltaTime)
{
	if (!canPush) {
		timerPush += DeltaTime;
		if (timerPush >= timeToWaitPush) {
			canPush = true;
			timerPush = 0;
		}
	}
}

void ABeamCharacter::SetupCollision()
{
	boxCollision = GetComponentByClass<UBoxComponent>();

	if (boxCollision == nullptr || CharacterSettings == nullptr) return;

	boxCollision->SetBoxExtent(FVector(CharacterSettings->ZoneKnockback_Size.X, 40.f, CharacterSettings->ZoneKnockback_Size.Y));

	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Emerald, FString::Printf(TEXT("WOWWWW : %s"), *boxCollision->GetName()));

	boxCollision->OnComponentBeginOverlap.AddDynamic(this, &ABeamCharacter::OnBeginOverlapZone);
	boxCollision->OnComponentEndOverlap.AddDynamic(this, &ABeamCharacter::OnEndOverlapZone);

}

void ABeamCharacter::OnBeginOverlapZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABeamCharacter* player = Cast<ABeamCharacter>(OtherActor);
	if (player == nullptr) return;

	PlayersInZone.Add(player);
}

void ABeamCharacter::OnEndOverlapZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABeamCharacter* player = Cast<ABeamCharacter>(OtherActor);
	if (player == nullptr) return;

	PlayersInZone.Remove(player);
}
void ABeamCharacter::creatAim()
{
	localPlayerAim = NewObject<UPlayerAim>(this);
}

void ABeamCharacter::playerAimInit()
{
	if(localPlayerAim == nullptr) return;
	localPlayerAim->InitCharacter(this);
}

const UBeamCharacterSettings* ABeamCharacter::GetCharacterSettings() const
{
	return CharacterSettings;
}

void ABeamCharacter::SetupMappingContextIntoController()
{
	playerController = Cast<APlayerController>(Controller);
	if (playerController == nullptr) return;

	ULocalPlayer* player = playerController->GetLocalPlayer();
	if (player == nullptr) return;

	UEnhancedInputLocalPlayerSubsystem* InputSystem = player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (InputSystem == nullptr) return;

	InputSystem->AddMappingContext(InputMappingContext, 0);
}

void ABeamCharacter::BindInputActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (InputData == nullptr) return;

	if(InputData->InputActionMoveVector2D)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveVector2D,
			ETriggerEvent::Started,
			this,
			&ABeamCharacter::OnInputMove
			);

		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveVector2D,
			ETriggerEvent::Completed,
			this,
			&ABeamCharacter::OnInputMove
			);

		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveVector2D,
			ETriggerEvent::Triggered,
			this,
			&ABeamCharacter::OnInputMove
			);
	}
	if(InputData->InputActionJump)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionJump,
			ETriggerEvent::Started,
			this,
			&ABeamCharacter::OnInputJump
			);

		EnhancedInputComponent->BindAction(
			InputData->InputActionJump,
			ETriggerEvent::Completed,
			this,
			&ABeamCharacter::OnInputJump
			);
	}
	if(InputData->InputActionDash)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionDash,
			ETriggerEvent::Started,
			this,
			&ABeamCharacter::OnInputDash
			);

		EnhancedInputComponent->BindAction(
			InputData->InputActionDash,
			ETriggerEvent::Completed,
			this,
			&ABeamCharacter::OnInputDash
			);
	}

	if(InputData->InputActionCharge)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionCharge,
			ETriggerEvent::Started,
			this,
			&ABeamCharacter::OnInputCharge
			);

		EnhancedInputComponent->BindAction(
			InputData->InputActionCharge,
			ETriggerEvent::Completed,
			this,
			&ABeamCharacter::OnInputCharge
			);
	}
	if(InputData->InputActionAimVector2D)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionAimVector2D,
			ETriggerEvent::Started,
			this,
			&ABeamCharacter::OnInputAim
			);

		EnhancedInputComponent->BindAction(
			InputData->InputActionAimVector2D,
			ETriggerEvent::Completed,
			this,
			&ABeamCharacter::OnInputAim
			);

		EnhancedInputComponent->BindAction(
			InputData->InputActionAimVector2D,
			ETriggerEvent::Triggered,
			this,
			&ABeamCharacter::OnInputAim
			);
	}
	if(InputData->InputActionShoot)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionShoot,
			ETriggerEvent::Started,
			this,
			&ABeamCharacter::OnInputShoot
			);

		EnhancedInputComponent->BindAction(
			InputData->InputActionShoot,
			ETriggerEvent::Completed,
			this,
			&ABeamCharacter::OnInputShoot
			);
	}

	if(InputData->InputActionPunch)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionPunch,
			ETriggerEvent::Started,
			this,
			&ABeamCharacter::OnInputPush
			);

		EnhancedInputComponent->BindAction(
			InputData->InputActionPunch,
			ETriggerEvent::Completed,
			this,
			&ABeamCharacter::OnInputPush
			);
	}

	if (InputData->InputActionFly) 
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionFly,
			ETriggerEvent::Started,
			this,
			&ABeamCharacter::OnInputFly
		);

		EnhancedInputComponent->BindAction(
			InputData->InputActionFly,
			ETriggerEvent::Completed,
			this,
			&ABeamCharacter::OnInputFly
		);


	}
}


void ABeamCharacter::OnInputMove(const FInputActionValue& InputActionValue)
{
	InputMove = InputActionValue.Get<FVector2D>();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, FString::Printf(TEXT("WOWWWW : %s"), *InputMove.ToString()));
}
void ABeamCharacter::OnInputJump(const FInputActionValue& InputActionValue)
{
	InputJump = InputActionValue.Get<bool>();
}
void ABeamCharacter::OnInputDash(const FInputActionValue& InputActionValue)
{
	InputDash = InputActionValue.Get<bool>();
}

void ABeamCharacter::OnInputCharge(const FInputActionValue& InputActionValue)
{
	InputCharge = InputActionValue.Get<bool>();
}
void ABeamCharacter::OnInputAim(const FInputActionValue& InputActionValue)
{
	InputAim = InputActionValue.Get<FVector2D>();
}
void ABeamCharacter::OnInputShoot(const FInputActionValue& InputActionValue)
{
	InputShoot = InputActionValue.Get<bool>();
}

void ABeamCharacter::OnInputPush(const FInputActionValue& InputActionValue)
{
	InputPush = InputActionValue.Get<bool>();
}

void ABeamCharacter::OnInputFly(const FInputActionValue& InputActionValue)
{
	InputFly = InputActionValue.Get<bool>();
}


FVector2D ABeamCharacter::GetInputMove() const{ return InputMove; }
bool ABeamCharacter::GetInputJump() const{ return InputJump; }
bool ABeamCharacter::GetInputDash() const{ return InputDash; }

bool ABeamCharacter::GetInputCharge() const{ return InputCharge; }
FVector2D ABeamCharacter::GetInputAim() const{ return InputAim; }
bool ABeamCharacter::GetInputShoot() const{ return InputShoot; }

bool ABeamCharacter::GetInputPush() const{ return InputPush; }

bool ABeamCharacter::GetInputFly() const { return InputFly; }
