// Fill out your copyright notice in the Description page of Project Settings.

#include "Rewinder.h"

const float Rewinder::s_fREFRESH_RATE = 1.0f / 30.0f;
	  bool  Rewinder::s_bIsRewindActive = false;

Rewinder::Rewinder()
	:s_fDeltaTime(0.0f), s_ui8RemainingRewind(0), s_ui8HeadLocation(0)
{
}

Rewinder::~Rewinder()
{
}

void Rewinder::SetRewindState(bool bNewRewindState)
{
	s_bIsRewindActive = bNewRewindState ? true : false;
}

uint8 Rewinder::GetRewind()
{
	return s_ui8RemainingRewind;
}

void Rewinder::RewindInit(UStaticMeshComponent* mesh , AActor* actor)
{
	for (int i = 0; i < s_ui8TRACKED_VALUES_SIZE; ++i) // Setting initial values by filling array with starting value on creation.
	{
		m_arrTrackedLocation[i] = actor->GetActorLocation();
		m_arrTrackedRotation[i] = actor->GetActorQuat();
		m_arrTrackedLinearVelocity[i] = mesh->GetPhysicsLinearVelocity();
		m_arrTrackedAngularVelocity[i] = mesh->GetPhysicsAngularVelocity();
	}
}

void Rewinder::RewindTick(float deltaTime, UStaticMeshComponent* mesh, AActor* actor)
{
	s_fDeltaTime += deltaTime; // If the framerate changes this will allow us to run the following the correct number of times. The speed multiplier can create a speedup or slowmo effect.

	while (s_fDeltaTime >= s_fREFRESH_RATE) // This loop insures that our rewind is FPS independant. 
	{
		s_fDeltaTime -= s_fREFRESH_RATE; // If the delta time is too hight this will cause the loop to run twice per frame. If it's too low it wont run at all.

		if (s_bIsRewindActive)	// After incrementing set new position.
		{
			actor->SetActorLocationAndRotation(m_arrTrackedLocation[s_ui8HeadLocation], m_arrTrackedRotation[s_ui8HeadLocation], false, nullptr, ETeleportType::TeleportPhysics);
			mesh->SetPhysicsLinearVelocity(m_arrTrackedLinearVelocity[s_ui8HeadLocation]);
			mesh->SetPhysicsAngularVelocity(m_arrTrackedAngularVelocity[s_ui8HeadLocation]);
		}
		if (s_bIsRewindActive) // If active decrement loop.
		{
			if (s_ui8RemainingRewind)
			{
				--s_ui8HeadLocation;
				--s_ui8RemainingRewind;
			}
		}
		else // If inactive increment loop.
		{
			++s_ui8HeadLocation;

			if (s_ui8RemainingRewind != s_ui8TRACKED_VALUES_SIZE)// Should only go up to 255.				
				++s_ui8RemainingRewind;			
		}
		if (!s_bIsRewindActive) // Record new position after decrementing.
		{
			m_arrTrackedLocation[s_ui8HeadLocation] = actor->GetActorLocation();
			m_arrTrackedRotation[s_ui8HeadLocation] = actor->GetActorQuat();
			m_arrTrackedLinearVelocity[s_ui8HeadLocation] = mesh->GetPhysicsLinearVelocity();
			m_arrTrackedAngularVelocity[s_ui8HeadLocation] = mesh->GetPhysicsAngularVelocity();
		}
	}
}
