// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "RewindDevelopmentGameMode.h"
#include "RewindDevelopmentBall.h"

ARewindDevelopmentGameMode::ARewindDevelopmentGameMode()
{
	// set default pawn class to our ball
	DefaultPawnClass = ARewindDevelopmentBall::StaticClass();
}
