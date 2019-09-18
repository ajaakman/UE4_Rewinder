// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include <memory>

/**
 * 
 */
class REWIND_API Rewinder 
{
public:
	Rewinder();
	~Rewinder();

	UFUNCTION(BlueprintPure, Category = "Rewind")
	uint8 GetRewind();
protected:
	void RewindInit(UStaticMeshComponent* mesh, AActor* actor);
	void RewindTick(float deltaTime, UStaticMeshComponent* mesh, AActor* actor);

	UFUNCTION(BlueprintCallable, Category = "Rewind")
	static void SetRewindState(bool bNewRewindState);

	const static uint8 s_ui8TRACKED_VALUES_SIZE = 255; // Size of the arrays that track our values.
	const static float s_fREFRESH_RATE; // How many times per second the values are tracked.

	static bool s_bIsRewindActive; // Has the rewind loop been activated or not.

	float s_fDeltaTime; // Used to get an FPS independant effect.
	uint8 s_ui8RemainingRewind; // How much of the rewind loop is left.
	uint8 s_ui8HeadLocation; // Holds the current position of the tracking cycle.	

	const std::unique_ptr<FVector[]> m_arrTrackedLocation = std::make_unique<FVector[]>(s_ui8TRACKED_VALUES_SIZE+1); // Vector array that tracks the position of the parent object.	
	const std::unique_ptr<FQuat[]> m_arrTrackedRotation = std::make_unique<FQuat[]>(s_ui8TRACKED_VALUES_SIZE+1); // Quaterion array that tracks the rotation of the parent object.		
	const std::unique_ptr<FVector[]> m_arrTrackedLinearVelocity = std::make_unique<FVector[]>(s_ui8TRACKED_VALUES_SIZE+1); // Quaterion array that tracks the rotation of the parent object.		
	const std::unique_ptr<FVector[]> m_arrTrackedAngularVelocity = std::make_unique<FVector[]>(s_ui8TRACKED_VALUES_SIZE+1); // Quaterion array that tracks the rotation of the parent object.
};
