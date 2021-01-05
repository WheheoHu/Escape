// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if (!Owner)
	{
		UE_LOG(LogTemp,Error,TEXT("NO Owner Finded"))
	}
	if (!PressurePlate)
	{
		UE_LOG(LogTemp,Error,TEXT("No Trigger Volume FOUND!"))
	}

}

void UOpenDoor::OpenDoor()
{

	// Open door
	if (!Owner)
	{
		return;
	}
	//Owner->SetActorRotation(FRotator(.0f, -OpenAngle, .0f));
	OnOpenRequest.Broadcast();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMassOfActorOnPlate() >= TriggerDoorMass)
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	if (GetWorld()->GetTimeSeconds() >= LastDoorOpenTime + DoorCloseDelay)
	{

		//close door
		if (!Owner)
		{
			return;
		}
		//Owner->SetActorRotation(FRotator(.0f, 0, .0f));
		OnCloseRequest.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorOnPlate()
{
	float TotalMass = 0.f;
	//Find all the overlapping actors
	TArray<AActor*> OverLappingActors;
	if (!PressurePlate)
	{
		return TotalMass;
	}
	PressurePlate->GetOverlappingActors(OverLappingActors);
	//Iterate through them adding their masses
	for (auto& Actor : OverLappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("total mass is %f"), TotalMass)
	}
	return TotalMass;
}

