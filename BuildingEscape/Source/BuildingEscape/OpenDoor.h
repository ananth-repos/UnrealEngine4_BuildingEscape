// Description:
// 1. Door operations (open/close)
// 2. There are 2 doors and triggered using a pressure plate.
// 3. When the mass of pressure plate exceeds 'TriggerMass' the door is opened. 
//    Otherwise door is always closed.
// 4. Door actions are handled using unreal blueprint.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "GameFramework/Actor.h"
#include "OpenDoor.generated.h"


// Event to interact with unreal blue print
// This broadcasts when to open/close door
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Broadcast event variables
	UPROPERTY(BlueprintAssignable)
	FDoorEvent OnOpenRequest;

	UPROPERTY(BlueprintAssignable)
	FDoorEvent OnCloseRequest;

private:
	// Definition of trigger volume
	UPROPERTY(EditAnywhere) 
	ATriggerVolume* PressurePlate = nullptr; //TODO need to add pointer protection later

	// Above this value, the door will be opened.
	UPROPERTY(EditAnywhere)
	float TriggerMass = 30.f;

	AActor* Owner; // The owning door

	// Returns total mass in kg
	float GetTotalMassOfActorsOnPlate();
	
};
