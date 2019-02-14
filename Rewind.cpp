// Fill out your copyright notice in the Description page of Project Settings.

#include "Rewind.h"
#include "GameFramework/Actor.h"

const float URewind::s_fREFRESH_RATE = 1.0f / 30.0f;

float URewind::s_fDeltaTime = 0.0f;
float URewind::s_fSpeedModifier = 1.0f;
bool URewind::s_bIsRewindActive = false;
uint16 URewind::s_ui16RemainingRewind = 0;
uint16 URewind::s_ui16HeadLocation = 0;
uint16 URewind::s_ui16ObjectCount = 0;
uint16 URewind::s_ui16ObjectIterator = 0;

URewind::URewind()
{	
	PrimaryComponentTick.bCanEverTick = true;
}

URewind::~URewind()
{
	--s_ui16ObjectCount; // Tracking number of objects in the scene with this component.
}

void URewind::BeginPlay()
{
	Super::BeginPlay();	

	++s_ui16ObjectCount;	// Tracking number of objects in the scene with this component.

	for (int i = 0; i < s_ui16TRACKED_VALUES_SIZE; ++i) // Setting initial values by filling array with starting value on creation.
	{
		m_arrTrackedLocation[i] = m_pPARENTACTOR->GetActorLocation();
		m_arrTrackedRotation[i] = m_pPARENTACTOR->GetActorQuat();
	}
	UE_LOG(LogClass, Log, TEXT("------------REWIND BEGIN PLAY------------"));
	//UE_LOG(LogClass, Log, TEXT("---------------Head Location is %i"), s_ui16HeadLocation);
	UE_LOG(LogClass, Log, TEXT("---------------Object Count is %i"), s_ui16ObjectCount);
	//UE_LOG(LogClass, Log, TEXT("---------------X Value at head is %f"), m_arrTrackedLocationX[s_ui16HeadLocation]);
}

void URewind::TickComponent(float fDeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(fDeltaTime, TickType, ThisTickFunction);

	if (s_bIsRewindActive) // Changing the parent actor mobility so we don't experience issues with physics.
		m_pPARENTACTOR->GetRootComponent()->SetMobility(EComponentMobility::Stationary);	
	else
		m_pPARENTACTOR->GetRootComponent()->SetMobility(EComponentMobility::Movable);

	if (s_ui16ObjectIterator == 0) // This loop ensures that the increment/decrement loop runs only once even if multiple objects of this class are present.
	{
		UE_LOG(LogClass, Log, TEXT("------------Increment Loop------------"));
		UE_LOG(LogClass, Log, TEXT("---------------Head Location is %i"), s_ui16HeadLocation);
		//UE_LOG(LogClass, Log, TEXT("---------------Object Count is %i"), s_ui16ObjectCount);
		UE_LOG(LogClass, Log, TEXT("---------------X Value at head is %f"), m_arrTrackedLocation[s_ui16HeadLocation].X);
		UE_LOG(LogClass, Log, TEXT("---------------X RemainingRewind is %i"), s_ui16RemainingRewind);

		s_ui16ObjectIterator = s_ui16ObjectCount; // Prevents it from running until it reaches 0. Then Resets.
		s_fDeltaTime += fDeltaTime * s_fSpeedModifier; // If the framerate changes this will allow us to run the following the correct number of times. The speed multiplier can create a speedup or slowmo effect.

		while (s_fDeltaTime >= s_fREFRESH_RATE) // This loop insures that our rewind is FPS independant. 
		{
			s_fDeltaTime -= s_fREFRESH_RATE; // If the delta time is too hight this will cause the loop to run twice per frame. If it's too low it wont run at all.

			if (s_bIsRewindActive) // If active decrement loop.
			{
				if (s_ui16RemainingRewind != 0) // Donst let this get below 0.
				{
					if (s_ui16HeadLocation == 0)
					{
						s_ui16HeadLocation = s_ui16TRACKED_VALUES_SIZE - 1; // If s_ui16HeadLocation reaches 0 set to 299					
					}
					else
					{
						--s_ui16HeadLocation;
					}

					--s_ui16RemainingRewind;
				}
			}
			else // If inactive increment loop.
			{
				if (s_ui16HeadLocation == s_ui16TRACKED_VALUES_SIZE - 1)
				{
					s_ui16HeadLocation = 0; // If s_ui16HeadLocation reaches 299 set to 0				
				}
				else
				{
					++s_ui16HeadLocation;
				}

				if (s_ui16RemainingRewind != s_ui16TRACKED_VALUES_SIZE - 1) // Should only go up to 299.	
				{
					++s_ui16RemainingRewind;
				}
			}
		}
		if (s_fSpeedModifier < 1.0f) s_fSpeedModifier += s_fREFRESH_RATE;
		else s_fSpeedModifier = 1.0f;
	}
	--s_ui16ObjectIterator; // This makes sure that the loop above runs only once per tick if multiple objects are in the scene.

	if (s_bIsRewindActive)	// After incrementing set new position.
	//if (s_bIsRewindActive && m_pPARENTACTOR->GetActorLocation() != m_arrTrackedLocation[s_ui16HeadLocation])	// After incrementing set new position.
	{
		if (m_pPARENTACTOR->GetActorLocation() == m_arrTrackedLocation[s_ui16HeadLocation] && m_pPARENTACTOR->GetActorQuat() == m_arrTrackedRotation[s_ui16HeadLocation])
		{} // This sould remove jumps by getting the inbetween locations. if && remaining rewind > 0
		else
		{
		m_pPARENTACTOR->GetRootComponent()->SetMobility(EComponentMobility::Movable);
		m_pPARENTACTOR->SetActorLocationAndRotation(m_arrTrackedLocation[s_ui16HeadLocation], m_arrTrackedRotation[s_ui16HeadLocation], false, nullptr, ETeleportType::TeleportPhysics);
		}
	}
	else // Record new position after decrementing.
	{
		m_arrTrackedLocation[s_ui16HeadLocation] = m_pPARENTACTOR->GetActorLocation();
		m_arrTrackedRotation[s_ui16HeadLocation] = m_pPARENTACTOR->GetActorQuat();
	}
}

void URewind::SetRewindState(bool bNewRewindState)
{	
	if (bNewRewindState)
	{
		UE_LOG(LogClass, Log, TEXT("------------Is active on------------"));
		s_fSpeedModifier = s_fREFRESH_RATE;
		s_bIsRewindActive = true;	
	}
	else
	{
		UE_LOG(LogClass, Log, TEXT("------------Is active off------------"));
		s_fSpeedModifier = 1.0f;
		s_bIsRewindActive = false;	
	}
}

int URewind::GetRemainingRewind()
{
	return s_ui16RemainingRewind;
}
