// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Array.h"
#include "Containers/Queue.h"
#include "ESimonBlockType.h"
#include "SimonManager.generated.h"

class ASimonBlock;
class AScoreCountManager;

UCLASS()
class SIMONEAME_API ASimonManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimonManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	//References
	TWeakObjectPtr<AScoreCountManager> ReferenceScoreCountManager;

	UPROPERTY(EditAnywhere, Category = "SimonManager", meta = (UIMin = "1"))
	int StartingSequenceLength;
	int CurrentSequenceLength;

	//References to Simon blocks
	UPROPERTY(EditAnywhere, Category = "SimonManager|Blocks")
	TWeakObjectPtr<ASimonBlock> RedSimonBlock;
	UPROPERTY(EditAnywhere, Category = "SimonManager|Blocks")
	TWeakObjectPtr<ASimonBlock> BlueSimonBlock;
	UPROPERTY(EditAnywhere, Category = "SimonManager|Blocks")
	TWeakObjectPtr<ASimonBlock> GreenSimonBlock;
	UPROPERTY(EditAnywhere, Category = "SimonManager|Blocks")
	TWeakObjectPtr<ASimonBlock> YellowSimonBlock;

	//Reference to widgets
	UPROPERTY(EditAnywhere, Category = "SimonManager|Widgets")
	TSubclassOf<class UUserWidget> wGameOverPanel;

	class USoundWave* CorrectResultSound;
	class USoundWave* WrongResultSound;

	//Data structures to store the simon sequences
	// The first one is used for comparing player input
	// The second for displaying the block color sequences
	TArray<ESimonBlockColorEnum> SimonSequence;
	TQueue<ESimonBlockColorEnum> EvaluatePlayerInputSimonSequence;
	TQueue<ESimonBlockColorEnum> DisplayingSimonSequence;

	void InitSimon();
	void GenerateSequenceStep();
	void DisplayNextSequenceStep();

	void EvaluateSequenceResult(bool bCorrect);
	void GameOver();

	void StartDisplaySequence();
	void EndDisplaySequence();
	void DisplaySequenceResult(bool bCorrect);
	void DisplayGameOver();

	void EnablePlayerInput(bool bEnable);

	// Booleans used for controlling/starting the timers made in the Tick method
	bool bIsWaitingSequenceStart;
	bool bIsWaitingDisplayNextStepSequence;
	bool bIsWaitingGameOver;

	bool bIsDisplayingSequence;
	bool bIsPlayerSuccesfullyGuessedSequence;
	bool bIsDisplayingResult;
	bool bIsWaitingForPlayerInput;

	UPROPERTY(EditAnywhere, Category = "SimonManager|TimeSettings")
	float WaitingTimeUntilSequenceStart;
	UPROPERTY(EditAnywhere, Category = "SimonManager|TimeSettings")
	float WaitingTimeBetweenDisplayingSequenceSteps;
	UPROPERTY(EditAnywhere, Category = "SimonManager|TimeSettings")
	float WaitingTimeUntilGameOver;
	float TimeDisplayingResult;

	float AccumulatedDeltaTime;
	
public:

	void EvaluateNextSequenceStep();
	void EvaluatePlayerInput(ESimonBlockColorEnum SimonBlockColor);

	bool IsAnySimonBlockHighlighted();
	
	FORCEINLINE bool IsDisplayingSequence() const { return bIsDisplayingSequence; }
	FORCEINLINE bool IsDisplayingResult() const { return bIsDisplayingResult; }
	FORCEINLINE bool IsWaitingForPlayerInput() const { return bIsWaitingForPlayerInput; }
};
