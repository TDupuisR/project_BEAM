// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "ProjectileInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CollisionQueryParams.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "Projectile.generated.h"

class AActor;

USTRUCT(BlueprintType)
struct FProjectileParameters

{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float speed = 100.f;
	float width = 100.f;
	float height = 100.f;
	float lifeSpan = 100000.f;
};


UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Projectile"))
class PROJECT_BEAM_API AProjectile : public AActor, public IProjectileInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile();
	void InitialisePower(int power);

	UPROPERTY(EditAnywhere)
	float height;
	UPROPERTY(EditAnywhere)
	float radius;
	
	virtual EProjectileType ProjectileGetType() override;
	virtual AProjectile* GetProjectile() override;
	virtual bool ProjectileContext(int power, FVector position) override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UCapsuleComponent* Capsule;

	UFUNCTION(BlueprintCallable)
	void GetDestroyed();
	UFUNCTION(BlueprintCallable)
	void FakeDestroye(int power);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* projectileComponent;
	UPROPERTY(Config, EditAnywhere, Category="Power Parameters") 
	TMap<int, FProjectileParameters> powerParameters;
	FProjectileParameters projectileCurrentParam;
	UPROPERTY(EditAnywhere)
	FHitResult projectileHitResult;
	FCollisionQueryParams params;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
	class UStaticMesh* ProjectileMesh;

	UFUNCTION(BlueprintCallable)
	int GetPower();

	AActor* actorParent;

private:
	bool canAccess = true;
	int ownPower;
	
};
