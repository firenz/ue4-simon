// Fill out your copyright notice in the Description page of Project Settings.

#include "SimoneameGameModeBase.h"
#include "SimoneamePlayerController.h"
#include "MouseInputHandlerPawn.h"

ASimoneameGameModeBase::ASimoneameGameModeBase()
{
	// no pawn by default
	DefaultPawnClass = AMouseInputHandlerPawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = ASimoneamePlayerController::StaticClass();
}


