// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Containers/Array.h"
#include "SimoneameSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SIMONEAME_API USimoneameSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	USimoneameSaveGame();

	//The data to be saved
	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<int> Scores;
	
	// Constant with the maximum number of scores to be saved
	const int MAX_NUMBER_SAVEDSCORES = 5; 

	// Needed info so it can be accessed in other classes
	static const FString SaveSlotName;
	static const uint32 UserIndex;

	
};
