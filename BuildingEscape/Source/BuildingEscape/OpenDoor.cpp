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

void UOpenDoor::OpenDoor() {
//    Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
    OnOpenRequest.Broadcast();
}

void UOpenDoor::CloseDoor() {
    Owner->SetActorRotation(FRotator(0.0f, -90.0f, 0.0f));
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
    
    //if(!Owner) {return; }
    Owner = GetOwner();
	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume
    if(GetTotalMassOfActorsOnPlate() > 25.f)
    {
        OpenDoor();
        LastDoorOpenTime = GetWorld()->GetTimeSeconds();
    }
    
    //check if it's time to close door
    if(GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay) {
        CloseDoor();
    }
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
    float TotalMass = 0.f;
    
    // Find all the olapping actors
    TArray<AActor*> OverlappingActors;
    PressurePlate->GetOverlappingActors(OverlappingActors);
    
    // Iterate though them adding their masses
    for (auto& Actor : OverlappingActors)
    {
        TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
        UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName());
    }
    return TotalMass;
}
