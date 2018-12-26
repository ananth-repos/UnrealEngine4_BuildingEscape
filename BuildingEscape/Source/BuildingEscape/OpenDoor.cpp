// Implementation for methods defined in OpenDoor.h

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Grabber.h"
#include "Components/PrimitiveComponent.h"


// To mark output variables from methods
#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	AActor* Owner = GetOwner();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Check mass on the pressure plate against TriggerMass
	if (GetTotalMassOfActorsOnPlate() > TriggerMass)
	{
		// Opens door
		OnOpenRequest.Broadcast();
	}
	else
	{
		// Closes door
		OnCloseRequest.Broadcast();
	}
	
}

// Returns total mass on the pressure plate
float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f; // Initialize

	// Find all overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Iterate through them adding their masses
	for (const auto* Actor : OverlappingActors) {
		
		// Display which object is on teh pressure plate
		UE_LOG(LogTemp, Warning, TEXT("%s on Pressure Plate!"), *Actor->GetName());

		// Add total mass
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}
