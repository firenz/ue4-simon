// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


#include "ScoreCountManager.generated.h"

UCLASS()
class SIMONEAME_API AScoreCountManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScoreCountManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditAnywhere, Category = "ScoreCountManager")
	TSubclassOf<class UUserWidget> wScoreCount;
	TWeakObjectPtr<class UTextBlock> pScoreCountText;
	
	TWeakObjectPtr<class ASaveGameManager> ReferenceSaveGameManager;

	void InitDisplayScoreCount();
	void UpdateDisplayScoreCount();

	int ScoreCount;

public:

	void AddScoreCount();
	void SaveScore();
};
