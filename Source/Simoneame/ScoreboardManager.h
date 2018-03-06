// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Array.h"
#include "ScoreboardManager.generated.h"

UCLASS()
class SIMONEAME_API AScoreboardManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScoreboardManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	TWeakObjectPtr<class ASaveGameManager> ReferenceSaveGameManager;
	
	UPROPERTY(EditAnywhere, Category = "ScoreboardManager")
	TSubclassOf<class UUserWidget> wScoreboard;
	UPROPERTY(EditAnywhere, Category = "ScoreboardManager")
	TSubclassOf<class UUserWidget> wScoreboardElement;

	TWeakObjectPtr<class UVerticalBox> pScoreboardPanel;
	
	void DisplayScoreboard();
	void DisplayScoreboardElements();
	
};
