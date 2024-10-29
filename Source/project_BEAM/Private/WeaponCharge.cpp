// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponCharge.h"
#include "Engine/World.h"
#include "Characters/BeamCharacter.h"
#include "Characters/PlayerAim.h"

#include "MaterialHLSLTree.h"


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

	// ...
	
}

void UWeaponCharge::StartWeaponCharge()
{
	power = 0;
	qteTimeLeft = qteMaxTime;
	chargeWasPushed = false;
	isQteActive = true;
}
void UWeaponCharge::CancelWeaponCharge()
{
	isQteActive = false;
	pointAim->ShotCall(power);
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


// Called every frame
void UWeaponCharge::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (isQteActive)
	{
		if(qteTimeLeft > .0f) // remplace par le timer
		{
			if (Character->GetInputCharge() && !chargeWasPushed && power < 3) // Is Charge button == true
			{
				chargeWasPushed = true;
				
				if (qteTimeLeft <= qteTimeStamp[power]) // if QTE success
				{
					power++;
					qteTimeLeft = qteMaxTime;
					//PlayAnimQTE()
				}
				else // if QTE Fail
				{
					CancelWeaponCharge();
				}
			}
			else if (!Character->GetInputCharge() && chargeWasPushed && power < 3) // Is Charge button == false
			{
				chargeWasPushed = false;
			}

			if(power >= 3 && chargeWasPushed) // if Niv3 wait for x sec and shoot
			{
				qteTimeLeft = qteFinaleDelay;
				chargeWasPushed = false;
			}
			
			qteTimeLeft -= GetWorld()->DeltaTimeSeconds;
		}
		else
		{
			CancelWeaponCharge();
		}
	}
}

