// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"


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
    FindPhysicsHandleComponent();
    SetupInputComponent();
}

//Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{
    PhysicsHandle =  GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if(PhysicsHandle == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Error: %s missed PhysicsHandleComponent"), *(GetOwner()->GetName()));
    }
}

//Look for attached Input Component
void UGrabber::SetupInputComponent()
{
    InputComponent =  GetOwner()->FindComponentByClass<UInputComponent>();
    if(InputComponent) {
        InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
        InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("Error: %s missed PhysicsHandleComponent"), *(GetOwner()->GetName()));
    }
}

void UGrabber::Grab()
{
    //Line Trace and see if we reach any actors with physics body collision channel set
    auto HitResult = GetFirstPhysicsBodyInReach();
    auto ComponentToGrab = HitResult.GetComponent();
    auto ActorHit = HitResult.GetActor();
    //If we hit something then attach a physic handle
    if(ActorHit)
    {
        // attach physics handle
        if(!PhysicsHandle) {return ;}
        PhysicsHandle->GrabComponent(
                                        ComponentToGrab,
                                        NAME_None,
                                        ComponentToGrab->GetOwner()->GetActorLocation(),
                                        true
                                        );
        
    }
    
}

void UGrabber::Release() {
    UE_LOG(LogTemp, Warning, TEXT("Grab released"));
    // release physics handle
    if(!PhysicsHandle) {return; }
    PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    //if(!PhysicsHandle) {return ;}
    // if the physics handle is attached
    if(PhysicsHandle->GrabbedComponent) {
        // move the object that we're holding
        PhysicsHandle->SetTargetLocation(GetReachLineEnd());
    }
    
    
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
    // Ray-cast out to reach distance
    FCollisionQueryParams TraceParameter(FName(TEXT("")), false, GetOwner());
    FHitResult HitResult;
    
    GetWorld()->LineTraceSingleByObjectType(
                                            HitResult,
                                            GetReachLineStart(),
                                            GetReachLineEnd(),
                                            FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
                                            TraceParameter
                                            );
    
    //See what we hit
    AActor* ActorHit = HitResult.GetActor();
    if(ActorHit) {
        UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()));
    }
    return HitResult;
}

FVector UGrabber::GetReachLineStart() const
{
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
    
    return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd() const
{
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
    
    return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}
