// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Grabber.h"
#include "Components/PrimitiveComponent.h"


#define OUT

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
	AActor* Owner = GetOwner();
}

void UOpenDoor::OpenDoor()
{
	// Set the door rotation
	OnOpenRequest.Broadcast();
	//GetOwner()->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

void UOpenDoor::CloseDoor()
{
	// Set the door rotation
	GetOwner()->SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume every frame
	if (GetTotalMassOfActorsOnPlate() > 30.f) //TO DO MAKE IT A PARAMETER
	{ 
		// If the ActorThatOpens is in the volume
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	// Check if its time to close the door
	//CurrentTime = GetWorld()->GetTimeSeconds();
	
	if (GetWorld()->GetTimeSeconds() -  LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
	
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	// Find all overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	// Iterate through them adding their masses
	for (const auto* Actor : OverlappingActors) {
		UE_LOG(LogTemp, Warning, TEXT("%s on Pressure Plate!"), *Actor->GetName());
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}
