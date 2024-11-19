// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponCharge.generated.h"

class ABeamCharacter;
class UPlayerAim;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class PROJECT_BEAM_API UWeaponCharge : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWeaponCharge();
	
	UFUNCTION(BlueprintCallable)
	void StartWeaponCharge();
	UFUNCTION(BlueprintCallable)
	void CancelWeaponCharge(bool noShoot);
	UFUNCTION(BlueprintCallable)
	void InitValues();
	
	UFUNCTION()
	void InitCharacter(ABeamCharacter* playerCharacter);
	UFUNCTION()
	void InitAim(UPlayerAim* playerAim);
	
	UFUNCTION(BlueprintCallable)
	bool GetIsQteActive() const;
	UFUNCTION(BlueprintCallable)
	float GetQteTimeLeft() const;
	UFUNCTION(BlueprintCallable)
	float GetQteMaxTime() const;
	UFUNCTION(BlueprintCallable)
	float GetQteTimeStamp() const;
	
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ABeamCharacter> Character;
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UPlayerAim> pointAim;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere)
	float qteMaxTime;
	UPROPERTY(EditAnywhere)
	float qteFinaleDelay;
	UPROPERTY(EditAnywhere)
	TArray<float> qteTimeStamp;
	UPROPERTY(EditAnywhere)
	float qteTimeStampPhase2;
	UPROPERTY()
	bool isQteActive = false;
	UPROPERTY()
	bool chargeWasPushed = false;
	UPROPERTY()
	float qteTimeLeft = 0.f;
	UPROPERTY()
	int power = 0;
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
