// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PrimeCalculator.generated.h"

UCLASS()
class REWINDDEVELOPMENT_API APrimeCalculator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APrimeCalculator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void RunPrimeTask(int32 NumPrimes);

	UFUNCTION(BlueprintCallable)
	void RunPrimeTaskOnMain(int32 NumPrimes);
	
};


class PrimeSearchTask : public FNonAbandonableTask
{
public:

	PrimeSearchTask(int32 _primeCount);
	
	~PrimeSearchTask();

	// requered by UE 4
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(PrimeSearchTask, STATGROUP_ThreadPoolAsyncTasks);
	}

	int32 PrimeCount;

	void DoWork();

	void DoWorkMain();

};