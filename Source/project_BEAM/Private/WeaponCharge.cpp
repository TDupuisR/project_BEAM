// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponCharge.h"
#include "Engine/World.h"
#include "Characters/BeamCharacter.h"
#include "Characters/BeamCharacterSettings.h"
#include "Characters/PlayerAim.h"
#include <Camera/CameraWorldSubsystem.h>


// Sets default values for this component's properties
UWeaponCharge::UWeaponCharge()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWeaponCharge::BeginPlay()
{
	Super::BeginPlay();
}

void UWeaponCharge::StartWeaponCharge()
{
	InitValues();
	
	power = 0;
	qteTimeLeft = qteMaxTime;
	chargeWasPushed = false;
	isQteActive = true;

	Character->DisplayQte(Character);

	GetWorld()->GetSubsystem<UCameraWorldSubsystem>()->ShakeCamera(power + 1);


	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("QTE start")));
}
void UWeaponCharge::CancelWeaponCharge(bool noShoot)
{
	isQteActive = false;
	if (!noShoot) pointAim->ShotCall(power);
	Character->HideQte(Character);

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("QTE cancel")));
}

void UWeaponCharge::InitValues()
{
	qteMaxTime = Character->GetCharacterSettings()->QTEMaxTime;
	if (qteMaxTime <= 0) qteMaxTime = 5.f;
	
	qteFinaleDelay = Character->GetCharacterSettings()->QTELastWait;
	if (qteFinaleDelay < 0) qteFinaleDelay = 0.f;
	
	qteTimeStamp = Character->GetCharacterSettings()->QTETimeStamp;
	if (qteTimeStamp.Num() <= 0) qteTimeStamp = {2.f, 1.5f, 1.f};
	
	qteTimeStampPhase2 = Character->GetCharacterSettings()->QTETimeStampPhase2;
	if (qteTimeStampPhase2 <= 0) qteTimeStampPhase2 = 1.f;
}

void UWeaponCharge::InitCharacter(ABeamCharacter* playerCharacter)
{
	Character = playerCharacter;
}

void UWeaponCharge::InitAim(UPlayerAim* playerAim)
{
	pointAim = playerAim;
}

bool UWeaponCharge::GetIsQteActive() const
{
	return isQteActive;
}

float UWeaponCharge::GetQteTimeLeft() const
{
	return qteTimeLeft;
}

float UWeaponCharge::GetQteMaxTime() const
{
	return qteMaxTime;
}

float UWeaponCharge::GetQteTimeStamp() const
{;
	if (power < 0 || power > 2) return .0f;
	else if (Character->IsPhaseTwo()) return qteTimeStampPhase2;
	else return qteTimeStamp[power];
}

int UWeaponCharge::GetQtePower() const
{
	return power;
}


// Called every frame
void UWeaponCharge::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (isQteActive) // enter the QTE condition
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Cyan, FString::Printf(TEXT("QTE Time left: %f"), qteTimeLeft));
		
		if(qteTimeLeft >= .0f)
		{
			if (!Character->IsPhaseTwo()) // Phase One
			{
				if (power < qteTimeStamp.Num()) GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Cyan, FString::Printf(TEXT("QTE Time stamp: %f"), qteTimeStamp[power]));
				
				if (Character->GetInputCharge() && !chargeWasPushed && power < qteTimeStamp.Num())
				{
					chargeWasPushed = true;
				
					if (qteTimeLeft <= qteTimeStamp[power]) // if QTE success
					{
						power++;
						qteTimeLeft = qteMaxTime;
					
						//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Cyan, FString::Printf(TEXT("QTE success to power: %d "), power));
						Character->PassQte(Character);
						
						float shakeForce = power == 3 ? 30 : power == 2 ? 20 : 2;
						float shakeSpeed = power == 3 ? 20 : power == 2 ? 15 : 10;


						GetWorld()->GetSubsystem<UCameraWorldSubsystem>()->ShakeCamera(shakeForce, shakeSpeed);

					}
					else // if QTE Fail
					{
						Character->FailQte(Character);
						CancelWeaponCharge(false);
						GetWorld()->GetSubsystem<UCameraWorldSubsystem>()->UnShakeCamera();

					}
				}
				else if (!Character->GetInputCharge() && chargeWasPushed && power < qteTimeStamp.Num())
				{
					chargeWasPushed = false;
				}

				if(power >= 3 && chargeWasPushed)
				{
					qteTimeLeft = qteFinaleDelay;
					chargeWasPushed = false;

					Character->HideQte(Character);
					GetWorld()->GetSubsystem<UCameraWorldSubsystem>()->UnShakeCamera();
				}
			}
			else // Phase Two
			{
				if (power < 2) GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Cyan, FString::Printf(TEXT("QTE Time stamp: %f"), qteTimeStamp[power]));
				
				if (Character->GetInputCharge() && !chargeWasPushed && power < 2)
				{
					chargeWasPushed = true;
				
					if (qteTimeLeft <= qteTimeStampPhase2) // if QTE success
					{
						power += 2;
						qteTimeLeft = qteMaxTime;
					
						//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Cyan, FString::Printf(TEXT("QTE success to power: %d "), power));
						Character->PassQte(Character);
					}
					else // if QTE Fail
					{
						CancelWeaponCharge(false);
						Character->FailQte(Character);
					}
				}
				else if (!Character->GetInputCharge() && chargeWasPushed && power < 2)
				{
					chargeWasPushed = false;
				}

				if(power >= 2 && chargeWasPushed)
				{
					qteTimeLeft = qteFinaleDelay;
					chargeWasPushed = false;
					
					Character->HideQte(Character);
				}
			}
			
			qteTimeLeft -= GetWorld()->DeltaTimeSeconds;
		}
		else // Time over
		{
			CancelWeaponCharge(false);
			chargeWasPushed = false;
		}
	}
}
