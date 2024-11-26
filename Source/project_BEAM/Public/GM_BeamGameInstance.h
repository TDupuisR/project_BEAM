// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MatchTypeID.h"

#include "GM_BeamGameInstance.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangePoints);


UCLASS()
class PROJECT_BEAM_API UGM_BeamGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintAssignable) FOnChangePoints OnChangePoints;
	
	UFUNCTION(BlueprintCallable) void ResetPlayerPoints();
	UFUNCTION(BlueprintCallable) void DeployEvent();

	UFUNCTION(BlueprintCallable) int GetManche() const { return Manche; };
	UFUNCTION(BlueprintCallable) void SetManche(int NewManche) { Manche = NewManche; };
	UFUNCTION(BlueprintCallable) void AddManche();

	UFUNCTION(BlueprintCallable) int GetPlayerPoints(int PlayerIndex);
	UFUNCTION(BlueprintCallable) void SetPlayerPoints(int PlayerIndex, int NewPoints);
	UFUNCTION(BlueprintCallable) void AddPlayerPoints(int PlayerIndex, int Points);

	void SetPlayerPoints(TArray<int> NewPoints) { PlayerPoints = NewPoints; };
	UFUNCTION(BlueprintCallable) TArray<int> GetPlayersPoints() { return PlayerPoints; };

	UFUNCTION(BlueprintCallable) void SetMaxManche(int NewMaxManche) { MaxManche = NewMaxManche; };
	UFUNCTION(BlueprintCallable) int GetMaxManche() const { return MaxManche; };

	UFUNCTION(BlueprintCallable) void SetMatchType(EMatchTypeID NewMatchType) { MatchType = NewMatchType; };
	UFUNCTION(BlueprintCallable) EMatchTypeID GetMatchType() const { return MatchType; };

	UFUNCTION(BlueprintCallable) void SetLastSpawnNumber(int NewSpawnNumber) { LastSpawnNumber = NewSpawnNumber; };
	UFUNCTION(BlueprintCallable) int GetLastSpawnNumber() const { return LastSpawnNumber; };

	UFUNCTION(BlueprintCallable) void SetNumberPairAppeared(int NewNumber) { NumberPairAppeared = NewNumber; };
	UFUNCTION(BlueprintCallable) int GetNumberPairAppeared() const { return NumberPairAppeared; };

private:
	UPROPERTY()
	int Manche = 0;
	UPROPERTY()
	int LastSpawnNumber = 0;
	UPROPERTY()
	int NumberPairAppeared = 0;

	UPROPERTY()
	TArray<int> PlayerPoints = {0, 0};
	
	UPROPERTY()
	int MaxManche = 3;	

	UPROPERTY()
	EMatchTypeID MatchType = EMatchTypeID::Deathmatch;

};
