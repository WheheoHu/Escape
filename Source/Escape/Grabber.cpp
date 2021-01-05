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
	if (!PhysicsHandle)
	{
		return;
	}
	FindPhysicsHandle();

	//Initial grab
	InitGrab();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//get player view information
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		PlayerViewPointViewLocation,
		PlayerViewPointViewRotation
	);
	auto LineTraceEnd = GetLineTraceEnd();
	//if physics handle is attached
	if (!PhysicsHandle)
	{
		return;
	}
	if (PhysicsHandle->GrabbedComponent)
	{
		//move object
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));
	//try and reach any actor
	auto Hitresult=GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = Hitresult.GetComponent();
	
	//if we hit something then attach a physics handle 
	auto HitBody = Hitresult.GetActor();
	if (HitBody)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *HitBody->GetName());
		// attach physics handle 
		if (!PhysicsHandle)
		{
			return;
		}
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			PlayerViewPointViewRotation
		);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released"));
	// release physics handle 
	if (!PhysicsHandle)
	{
		return;
	}
	PhysicsHandle->ReleaseComponent();
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
{
	//get LineTraceEnd
	auto LineTraceEnd = GetLineTraceEnd();
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

	//see what we hit
	return Hit;
}

const FVector UGrabber::GetLineTraceEnd()
{
	FVector LineTraceEnd = PlayerViewPointViewLocation + PlayerViewPointViewRotation.Vector() * Reach;
	return LineTraceEnd;
}

