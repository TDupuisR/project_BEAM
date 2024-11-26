// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/BeamCharacterStateProjection.h"
#include "Characters/BeamCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/BeamCharacterStateMachine.h"
#include "Characters/BeamCharacterSettings.h"


EBeamCharacterStateID UBeamCharacterStateProjection::GetStateID()
{
	return EBeamCharacterStateID::Projection;
}

void UBeamCharacterStateProjection::StateEnter(EBeamCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	
	Character->SetCanTakeDamage(false);
	TimeToWait = Character->GetCharacterSettings()->TimeToWaitAfterProjection;
	Timer = 0.f;

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Blue,
		FString::Printf(TEXT("Enter State %d"), GetStateID())
	);
}

void UBeamCharacterStateProjection::StateExit(EBeamCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
	
	Character->SetCanTakeDamage(true);
	Character->SetCanTakeKnockback(true);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Blue,
		FString::Printf(TEXT("Exit State %d"), GetStateID())
	);
}

void UBeamCharacterStateProjection::StateTick(float DeltaTime)
{

	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	0.1f,
	// 	FColor::Red,
	// 	FString::Printf(TEXT("Tick State PROJECTION"))
	// );

	if (AfterProjection) {
		Timer += DeltaTime;
		if (Timer >= TimeToWait) {

			AfterProjection = false;
			Timer = 0.f;
			StateMachine->ChangeState(EBeamCharacterStateID::Idle);
		}
	}

	if (Character->GetCharacterMovement()->IsMovingOnGround() && Character->GetCharacterMovement()->GetLastUpdateVelocity().Size() <= Character->GetMinSizeVelocity()) 
	{
		AfterProjection = true;
		Character->SetCanTakeKnockback(false);
	}

}
