// Description:
// 1. Grabber handles to pick/move/drop the objects within the building.
// 2. Ray is casted from the default pawn actor.
// 3. When the ray hit the object, actor can pick up using pre-defined control (Grab method).
// 4. Grab is defined using unreal physics handle.
// 5. Grabbed component is released using the Release method.

// TODO needs refactoring

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float Reach = 200.0f; // Look ahead distane from the player

	// Define physics handle used to grab the object
	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	// Component to which the physics handle is attached
	UInputComponent* InputComponent = nullptr; // TODO pointer protection

	// Ray-cast & grab what is in reach:
	void Grab();
	
	// Release what's grabbed:
	void Release();

	// Finding physics handle component
	void FindPhysicsHandleComponent();

	// Setup attached input component
	void SetupInputComponent();

	// Return hit for first physics body in reach
	FHitResult GetFirstPhysicsBodyInReach() const;
	

};
