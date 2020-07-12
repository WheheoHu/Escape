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
	Owner = GetOwner();
	Super::BeginPlay();
	ActorOpenDoor = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::OpenDoor()
{
	
	// ...
	Owner->SetActorRotation(FRotator(.0f, -OpenAngle, .0f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (PressurePlate->IsOverlappingActor(ActorOpenDoor))
	{
		OpenDoor();
		LastDoorOpenTime= GetWorld()->GetTimeSeconds();
	}
	
	if (GetWorld()->GetTimeSeconds()>=LastDoorOpenTime+DoorCloseDelay)
	{
		
		//close door
		
		Owner->SetActorRotation(FRotator(.0f, 0, .0f));
	}
}

