// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveGameManager.h"

#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "SimoneameSaveGame.h"

// Sets default values
ASaveGameManager::ASaveGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASaveGameManager::BeginPlay()
{
	Super::BeginPlay();

	InitSavedGameSlot();
}

// Called every frame
void ASaveGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASaveGameManager::InitSavedGameSlot()
{
	// We check if a previous saved game instance exists, in the case it exists, we load it and save it
	if (UGameplayStatics::DoesSaveGameExist(USimoneameSaveGame::SaveSlotName, USimoneameSaveGame::UserIndex))
	{
		SavedGameInstance = Cast<USimoneameSaveGame>(UGameplayStatics::LoadGameFromSlot(SavedGameInstance->SaveSlotName, SavedGameInstance->UserIndex));
	}
	// In case it doesn't exist it gets the default data set in the initialization of the SimoneameSaveGame constructor
	else
	{
		SavedGameInstance = Cast<USimoneameSaveGame>(UGameplayStatics::CreateSaveGameObject(USimoneameSaveGame::StaticClass()));
		UGameplayStatics::SaveGameToSlot(SavedGameInstance, USimoneameSaveGame::SaveSlotName, USimoneameSaveGame::UserIndex);
	}
}

void ASaveGameManager::SaveScore(int NewScore)
{
	TArray<int> NewScoreArray;

	//Initialization workaround for avoiding reallocating memory errors
	NewScoreArray.Init(0, SavedGameInstance->MAX_NUMBER_SAVEDSCORES + 1);

	for (int i = 0; i < SavedGameInstance->MAX_NUMBER_SAVEDSCORES; i++)
	{
		NewScoreArray[i] = SavedGameInstance->Scores[i];
	}

	NewScoreArray[SavedGameInstance->MAX_NUMBER_SAVEDSCORES] = NewScore;

	// Sort the scores with the new one added
	NewScoreArray.Sort([](const int& A, const int& B) {
		return A > B;
	});

	// Since we can reallocate data in the Scores of SavedGameInstance because it will cause an access violation
	// we just use a simple for loop for reassignating the new scores
	for (int i = 0; i < SavedGameInstance->MAX_NUMBER_SAVEDSCORES; i++)
	{
		SavedGameInstance->Scores[i] = NewScoreArray[i];
	}
	
	UGameplayStatics::SaveGameToSlot(SavedGameInstance, USimoneameSaveGame::SaveSlotName, USimoneameSaveGame::UserIndex);
}

TArray<int> ASaveGameManager::LoadScores()
{
	TArray<int> ScoresArray;
	ScoresArray.Append(SavedGameInstance->Scores);
	
	return ScoresArray;
}

