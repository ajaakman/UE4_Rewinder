// Fill out your copyright notice in the Description page of Project Settings.

// When activated this actor component returns the parent actor to its previous location. 
// Tracking the last 5 seconds of the parent actors location and rotation.

#pragma once

#include <memory>
#include "UserWidget.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Rewind.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REWINDDEVELOPMENT_API URewind : public UActorComponent 
{
	GENERATED_BODY()

public:		

	URewind();
	~URewind();
	
protected:	

	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float fDeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	UFUNCTION(BlueprintCallable, Category = "Rewind")
	static void SetRewindState(bool bNewRewindState);	
	
	UFUNCTION(BlueprintPure, Category = "Rewind")
	static int GetRemainingRewind(); 

private:

	AActor* const m_pPARENTACTOR = GetOwner(); // Actor that this component is attached to.

	const static uint16 s_ui16TRACKED_VALUES_SIZE = 600; // Size of the arrays that track our values.
	const static float s_fREFRESH_RATE; // How many times per second the values are tracked.
	
	static float s_fDeltaTime; // Used to get an FPS independant effect.
	static float s_fSpeedModifier; // Multiplied with s_fDeltaTime to get a slowmo or speedup effect.
	static bool s_bIsRewindActive; // Has the rewind loob been activated or not.
	static uint16 s_ui16RemainingRewind; // How much of the rewind loop is left.
	static uint16 s_ui16HeadLocation; // Holds the current position of the tracking cycle.	
	static uint16 s_ui16ObjectCount; // Keeps track of the current number of objects that have this component in the scene.
	static uint16 s_ui16ObjectIterator; // Used to make sure that some blocks of code run only once per tick even if multiple objects are present.

	const std::unique_ptr<FVector[]> m_arrTrackedLocation = std::make_unique<FVector[]>(s_ui16TRACKED_VALUES_SIZE); // Vector array that tracks the position of the parent object.	
	const std::unique_ptr<FQuat[]> m_arrTrackedRotation = std::make_unique<FQuat[]>(s_ui16TRACKED_VALUES_SIZE); // Quaterion array that tracks the rotation of the parent object.		
};
