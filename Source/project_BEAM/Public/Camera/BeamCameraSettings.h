// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "BeamCameraSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Beam Camera Settings"))
class PROJECT_BEAM_API UBeamCameraSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public: 
	UPROPERTY(Config, EditAnywhere, Category = "General", meta = (ToolTip = "Le zoom max que peux avoir la camera (c'est une distance)"))
	float CameraZoomYMin = 1500.f;

	UPROPERTY(Config, EditAnywhere, Category = "General", meta = (ToolTip = "Le zoom min que peux avoir la camera (c'est une distance)"))
	float CameraZoomYMax = 2000.f;

	UPROPERTY(Config, EditAnywhere, Category = "General", meta = (ToolTip = "La distance minimum que les joueurs ont pour le zoom"))
	float CameraZoomDistanceBetweenTargetsMin = 300.f;

	UPROPERTY(Config, EditAnywhere, Category = "General", meta = (ToolTip = "La distance minimum que les joueurs ont pour le zoom"))
	float CameraZoomDistanceBetweenTargetsMax = 700.f;

};