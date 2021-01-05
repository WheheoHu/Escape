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


	// look for attached physics handle
	FindPhysicsHandle();

	//Initial grab
	InitGrab();



}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));
	//try and reach any actor
	auto HitBody=GetFirstPhysicsBodyInReach().GetActor();
	//if we hit something then attach a physics handle 

	//TODO attach physics handle 
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released"));
	//TODO release physics handle 
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing physics handle component"), *GetOwner()->GetName());
	}
}

void UGrabber::InitGrab()
{
	InputComponemt = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!InputComponemt)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing input component"), *GetOwner()->GetName());
	}
	else
	{
		InputComponemt->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponemt->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{	// Get player view point this tick
	FVector PlayerViewPointViewLocation;
	FRotator PlayerViewPointViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		PlayerViewPointViewLocation,
		PlayerViewPointViewRotation
	);



	FVector LineTraceEnd = PlayerViewPointViewLocation + PlayerViewPointViewRotation.Vector() * Reach;
	//draw debug line for line trace
	/*DrawDebugLine(
		GetWorld(),
		PlayerViewPointViewLocation,
		LineTraceEnd,
		FColor(200, 0, 0),
		false,
		0.f,
		0.f,
		10.f
	);*/


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
	return Hit;
}

