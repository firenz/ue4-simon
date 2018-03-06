// Fill out your copyright notice in the Description page of Project Settings.

#include "SimoneameSaveGame.h"
#include "Simoneame.h"

USimoneameSaveGame::USimoneameSaveGame()
{
	Scores.Init(0, MAX_NUMBER_SAVEDSCORES);
}

const FString USimoneameSaveGame::SaveSlotName = "SimoneameSave";
const uint32 USimoneameSaveGame::UserIndex = 0;




