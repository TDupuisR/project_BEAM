// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MatchTypeID.h"
//#include <Match/BeamMatchSystem.h>
#include "MatchSystemBeam.h"

#include "GM_BeamGameInstance.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangePoints);


UCLASS()
class PROJECT_BEAM_API UGM_BeamGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	void Init() override;

	UPROPERTY(BlueprintAssignable)
	FOnChangePoints OnChangePoints;

	UFUNCTION(BlueprintCallable)
	void DeployEvent();

	UFUNCTION(BlueprintCallable)
	void SetLastSpawnNumber(int NewSpawnNumber);

	UFUNCTION(BlueprintCallable)
	int GetLastSpawnNumber();

	UFUNCTION(BlueprintCallable)
	void SetNumberPairAppeared(int NewNumber);

	UFUNCTION(BlueprintCallable)
	int GetNumberPairAppeared();

	UFUNCTION(BlueprintCallable)
	void ChangeMatchType(EMatchTypeID NewMatchType);

	UFUNCTION(BlueprintCallable)
	TArray<int> GetPlayersPoints() const;

	TObjectPtr<UMatchSystemBeam> GetMancheSystem() const { return matchSystem; };

	UFUNCTION(BlueprintCallable)
	bool IsMatchFinished() {return matchSystem->IsMatchFinished();}

	UFUNCTION(BlueprintCallable)
	void ResetAll() {return matchSystem->ResetAll();}

private:
	UPROPERTY()
	int LastSpawnNumber = 0;
	UPROPERTY()
	int NumberPairAppeared = 0;

	UPROPERTY()
	TObjectPtr<UMatchSystemBeam> matchSystem;

};
