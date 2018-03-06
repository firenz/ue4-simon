// Fill out your copyright notice in the Description page of Project Settings.

#include "MouseInputHandlerPawn.h"


// Sets default values
AMouseInputHandlerPawn::AMouseInputHandlerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMouseInputHandlerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMouseInputHandlerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMouseInputHandlerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

