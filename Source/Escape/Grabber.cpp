// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Math/Vector.h"
#include "DrawDebugHelpers.h"
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

	// look for attached physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	InputComponemt = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing physics handle component"), *GetOwner()->GetName());
	}
	if (!InputComponemt)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing input component"), *GetOwner()->GetName());
	}

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
	//UE_LOG(LogTemp, Warning,
	//	TEXT("PlayerViewPointLocation is %s ,Rotation is %s"),
	//	*PlayerViewPointViewLocation.ToString(),
	//	*PlayerViewPointViewRotation.ToString()
	//);
	
	FVector LineTraceEnd = PlayerViewPointViewLocation +PlayerViewPointViewRotation.Vector()*Reach;
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointViewLocation,
		LineTraceEnd,
		FColor(200,0,0),
		false,
		0.f,
		0.f,
		10.f
		);

	
	//Ray-cast out to reach distance
	FHitResult Hit;
	FCollisionQueryParams TraceParam(FName(""), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		Hit,
		PlayerViewPointViewLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParam
	);

	auto HitRes = Hit.GetActor();
	if (HitRes)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *HitRes->GetName());
	}

	//see what we hit
}

