// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Math/Vector.h"
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	auto Owner = GetOwner();
	UE_LOG(LogTemp, Warning, TEXT("%s at %s !!"), *Owner->GetName(), *Owner->GetActorLocation().ToString());

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player view point this tick
	FVector PlayerViewPointViewLocation;
	FRotator PlayerViewPointViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		PlayerViewPointViewLocation,
		PlayerViewPointViewRotation
	);
	//TODO log out this   
	UE_LOG(LogTemp, Warning,
		TEXT("PlayerViewPointLocation is %s ,Rotation is %s"),
		*PlayerViewPointViewLocation.ToString(),
		*PlayerViewPointViewRotation.ToString()
	);
	
	//Ray-cast out to reach distance

	//see what we hit
}

