// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructibleWall.h"
#include "ProjectileInterface.h"


// Sets default values
ADestructibleWall::ADestructibleWall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

EProjectileType ADestructibleWall::ProjectileGetType()
{
	return EProjectileType::DestructWall;
}
AProjectile* ADestructibleWall::GetProjectile()
{
	return nullptr;
}
bool ADestructibleWall::ProjectileContext(int power, FVector position)
{
	if (power < resistance) return true;

	
}

// Called when the game starts or when spawned
void ADestructibleWall::BeginPlay()
{
	Super::BeginPlay();

	if (resistance < 0) resistance = 0;
	else if (resistance > 10) resistance = 10;
}

void ADestructibleWall::GetDestroyed()
{
	//Play Animation
	//Play Sound
	this->Destroy();
}



