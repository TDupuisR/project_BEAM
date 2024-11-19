// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/BeamCharacterStateFall.h"

#include "Characters/BeamCharacter.h"	
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/BeamCharacterStateMachine.h"
#include "Characters/BeamCharacterSettings.h"


EBeamCharacterStateID UBeamCharacterStateFall::GetStateID()
{
	return EBeamCharacterStateID::Fall;
}

void UBeamCharacterStateFall::StateEnter(EBeamCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	ZVelocity = 0;

	/*GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Purple,
		FString::Printf(TEXT("ENTER FALL"), GetStateID())
	);*/

	if (PreviousStateID == EBeamCharacterStateID::Jump) {
		canCoyote = false;
	}
	else {
		canCoyote = true;
	}
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.0f,
		FColor::Purple,
		FString::Printf(TEXT("BOOL : %b"), canCoyote)
	);
	ResetCoyoteTimer();
}

void UBeamCharacterStateFall::StateExit(EBeamCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	/*GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Purple,
		FString::Printf(TEXT("EXIT FALL"), GetStateID())
	);*/
}

void UBeamCharacterStateFall::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	
	if (Character->GetMovementComponent()->Velocity.Z < ZVelocity) {
		ZVelocity = Character->GetMovementComponent()->Velocity.Z;
	}

	 GEngine->AddOnScreenDebugMessage(
	 	-1,
	 	0.1f,
	 	FColor::Blue,
	 	FString::Printf(TEXT("STATE TICK FALL"))
	 );

	 GEngine->AddOnScreenDebugMessage(
		 -1,
		 0.1f,
		 FColor::Purple,
		 FString::Printf(TEXT("%f"), ZVelocity)
	 );


	if (Character->GetInputPush() && Character->CanPush()) {
		StateMachine->ChangeState(EBeamCharacterStateID::Push);
	}

	if (canCoyote) {
		timerCoyote += DeltaTime;
		if (timerCoyote >= timerCoyoteMax) {
			GEngine->AddOnScreenDebugMessage(
				-1,
				0.1f,
				FColor::Purple,
				FString::Printf(TEXT("END TIMER"))
			);
			OnCoyoteTimerEnd();
		}
	}

	if (Character->GetInputMove().X != 0) {

		float appliedForce = Character->GetInputMove().X;
		Character->AddMovementInput(FVector::ForwardVector, appliedForce);
	}

	if (IsKeyWasPressed(EKeys::I)) {
		StateMachine->ChangeState(EBeamCharacterStateID::Projection);
	}


	if (Character->GetMovementComponent()->IsMovingOnGround() || Character->GetMovementComponent()->Velocity.Z >= 0) {

		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Purple,
			FString::Printf(TEXT("%f"), ZVelocity)
		);	

		if (Character->GetCharacterSettings()->MinVelocityZStunFall < FMath::Abs(ZVelocity)) {
			Character->SetStunTime((Character->GetCharacterSettings()->MultiplyerStunFall / 1000) * FMath::Abs(ZVelocity));
			StateMachine->ChangeState(EBeamCharacterStateID::Stun);
		}
		else {
			StateMachine->ChangeState(EBeamCharacterStateID::Idle);
		}
	}

	if (Character->GetInputJump() && canCoyote) {
		GEngine->AddOnScreenDebugMessage(
			-1,
			0.1f,
			FColor::Purple,
			FString::Printf(TEXT("TAPPED"))
		);
		StateMachine->ChangeState(EBeamCharacterStateID::Jump);
	}

	
}

void UBeamCharacterStateFall::OnCoyoteTimerEnd()
{
	canCoyote = false;
}

void UBeamCharacterStateFall::ResetCoyoteTimer()
{
	timerCoyote = 0.0f;
	timerCoyoteMax = Character->GetCharacterSettings()->CoyoteTime;
}
