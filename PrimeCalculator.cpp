// Fill out your copyright notice in the Description page of Project Settings.

#include "PrimeCalculator.h"


// Sets default values
APrimeCalculator::APrimeCalculator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APrimeCalculator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APrimeCalculator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APrimeCalculator::RunPrimeTask(int32 NumPrimes)
{
	(new FAutoDeleteAsyncTask<PrimeSearchTask>(NumPrimes))->StartBackgroundTask();
}

void APrimeCalculator::RunPrimeTaskOnMain(int32 NumPrimes)
{
	PrimeSearchTask* task = new PrimeSearchTask(NumPrimes);

	task->DoWorkMain();
	
	delete task;
}

PrimeSearchTask::PrimeSearchTask(int32 _primeCount)
{
	PrimeCount = _primeCount;
}

PrimeSearchTask::~PrimeSearchTask()
{
	UE_LOG(LogTemp, Warning, TEXT("Task Finished !!!"))
}

void PrimeSearchTask::DoWork()
{
	int primesFound = 0;
	int currentTestNumber = 2;

	while (primesFound < PrimeCount)
	{
		bool isPrime = true;

		for (int i = 2; i < currentTestNumber / 2; i++)
		{
			if (currentTestNumber % i == 0)
			{
				isPrime = false;
				break;
			}
		}
		if (isPrime)
		{
			primesFound++;

			if (primesFound % 1000 == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("Primes found %i"), primesFound)
			}
		}
		currentTestNumber++;
	}
}

void PrimeSearchTask::DoWorkMain()
{
	DoWork();
}
