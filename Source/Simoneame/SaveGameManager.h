// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Array.h"
#include "SaveGameManager.generated.h"


UCLASS()
class SIMONEAME_API ASaveGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASaveGameManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	class USimoneameSaveGame* SavedGameInstance;

	void InitSavedGameSlot();

public:

	void SaveScore(int NewScore);
	TArray<int> LoadScores();
	
};
