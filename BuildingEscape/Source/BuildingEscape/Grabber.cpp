// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"


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

    UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
	
    //Look for attached Physics Handle
    PhysicsHandle =  GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if(PhysicsHandle) {
        
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("Error: %s missed PhysicsHandleComponent"), *(GetOwner()->GetName()));
    }
    
    //Look for attached Input Component
    InputComponent =  GetOwner()->FindComponentByClass<UInputComponent>();
    if(InputComponent) {
        UE_LOG(LogTemp, Warning, TEXT("InputComponent found"));
        // Bind the input axis
        InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("Error: %s missed PhysicsHandleComponent"), *(GetOwner()->GetName()));
    }
    
}

void UGrabber::Grab()
{
    UE_LOG(LogTemp, Error, TEXT("Grab pressed"));
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player view point this tick
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
    
    FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
    
    //Draw red trace to visualize
    DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(255, 0, 0), false, 0.f, 0.f, 10.f);
    
    // Ray-cast out to reach distance
    FCollisionQueryParams TraceParameter(FName(TEXT("")), false, GetOwner());
    FHitResult Hit;
    
    GetWorld()->LineTraceSingleByObjectType(
        Hit,
        PlayerViewPointLocation,
        LineTraceEnd,
        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
        TraceParameter
    );
    
    //See what we hit
    AActor* ActorHit = Hit.GetActor();
    if(ActorHit) {
        UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()));
    }
}

