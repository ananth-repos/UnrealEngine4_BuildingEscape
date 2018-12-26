// Implementation for methods defined in Grabber.h

// TODO refactoring

#include "Grabber.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// Look for attached physics handle. Log error if not found. 
	FindPhysicsHandleComponent();

	// Look for input component & perform grab/move/release action based on control input
	SetupInputComponent();		
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) { return; }
	// if the physics handle is attached
	if (PhysicsHandle->GetGrabbedComponent())
	{
		// move the object that we're holding
		PhysicsHandle->SetTargetLocation(GetLineTracePoints().v2);
	}
		

}

// Look for attached physics handle:
void UGrabber::FindPhysicsHandleComponent()
{

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle) {
		// Physics handle found
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Physics Handle for %s not found"), *GetOwner()->GetName());
	}
}

// Look for input component:
void UGrabber::SetupInputComponent()
{

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Input component(%s) found"), *GetOwner()->GetName());
		// Bind the input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input component(%s) not found"), *GetOwner()->GetName());
	}
}


// Grab the component
void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Pressed!"));

	// LINE TRACE and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	// if we hit something then attach a physics handle
	if (ActorHit != nullptr)
	{
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			FRotator(0.0f, 0.0f, 0.0f)
		);
	}
}

// Release the component
void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Released!"));
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

// Return hit for first physics body in reach
FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	// Line-trace (AKA ray-cast) out to reach distance
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	FTwoVectors TracePoints = GetLineTracePoints();
	
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		TracePoints.v1, //Start
		TracePoints.v2, //End
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	return HitResult;
}

// Get line trace
FTwoVectors UGrabber::GetLineTracePoints() const
{
	FVector StartLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT StartLocation,
		OUT PlayerViewPointRotation
	);
	FVector EndLocation = StartLocation + PlayerViewPointRotation.Vector() * Reach;
	return FTwoVectors(StartLocation, EndLocation);
}





