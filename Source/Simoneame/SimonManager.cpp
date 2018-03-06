// Fill out your copyright notice in the Description page of Project Settings.

#include "SimonManager.h"

#include "Engine.h"
#include "Sound/SoundWave.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Border.h"

#include "SimonBlock.h"
#include "ScoreCountManager.h"


// Sets default values
ASimonManager::ASimonManager()
	: StartingSequenceLength(1),
	bIsDisplayingSequence(false), bIsWaitingSequenceStart(true), bIsWaitingDisplayNextStepSequence(false), 
	bIsDisplayingResult(false), bIsWaitingForPlayerInput(false), bIsPlayerSuccesfullyGuessedSequence(false), bIsWaitingGameOver(false),
	WaitingTimeUntilSequenceStart(3.0f), WaitingTimeBetweenDisplayingSequenceSteps(0.5f), WaitingTimeUntilGameOver(3.0f), TimeDisplayingResult(3.0f), AccumulatedDeltaTime(0.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CorrectResultSound = Cast<USoundWave>(StaticLoadObject(USoundWave::StaticClass(), nullptr, TEXT("SoundWave'/Game/Sounds/Correct.Correct'")));
	WrongResultSound = Cast<USoundWave>(StaticLoadObject(USoundWave::StaticClass(), nullptr, TEXT("SoundWave'/Game/Sounds/Wrong.Wrong'")));
}

// Called when the game starts or when spawned
void ASimonManager::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AScoreCountManager> ActorItr(GetWorld(), AScoreCountManager::StaticClass()); ActorItr; ++ActorItr)
	{
		ReferenceScoreCountManager = *ActorItr;

		if (ReferenceScoreCountManager.IsValid())
		{
			break;
		}
	}

	if (RedSimonBlock.IsValid()
		&& BlueSimonBlock.IsValid()
		&& GreenSimonBlock.IsValid()
		&& YellowSimonBlock.IsValid())
	{
		InitSimon();
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(0, 2.5f, FColor::Red, TEXT("SIMON INITIALIZATION ABORTED SINCE REFERENCES TO THE BLOCKS ARE MISSING"));
		}
	}
}

// Called every frame
void ASimonManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Timer controlling the waiting time duration until the next displaying simon sequence starts
	if (bIsWaitingSequenceStart)
	{
		AccumulatedDeltaTime += DeltaTime;

		if (AccumulatedDeltaTime >= WaitingTimeUntilSequenceStart) {
			StartDisplaySequence();

			bIsWaitingSequenceStart = false;
			AccumulatedDeltaTime = 0.0f;
		}
	}

	// Timer controlling the waiting time duration between each simon block during a displaying simon sequence
	if (bIsWaitingDisplayNextStepSequence)
	{
		AccumulatedDeltaTime += DeltaTime;

		if (AccumulatedDeltaTime >= WaitingTimeBetweenDisplayingSequenceSteps) {
			DisplayNextSequenceStep();

			bIsWaitingDisplayNextStepSequence = false;
			AccumulatedDeltaTime = 0.0f;
		}
	}
	
	// Timer controlling the duration of the highlighted result
	if (bIsDisplayingResult)
	{
		AccumulatedDeltaTime += DeltaTime;

		if (AccumulatedDeltaTime >= TimeDisplayingResult) {
			if (bIsPlayerSuccesfullyGuessedSequence)
			{
				ReferenceScoreCountManager->AddScoreCount();
				bIsWaitingSequenceStart = true;
			}
			else {
				bIsWaitingGameOver = true;
			}

			bIsDisplayingResult = false;
			AccumulatedDeltaTime = 0.0f;
		}
	}

	// Timer controlling the waiting time duration after the timer of the wrong highlighted result
	// so the game over screen is not displayed instantaneously
	if (bIsWaitingGameOver)
	{
		AccumulatedDeltaTime += DeltaTime;

		if (AccumulatedDeltaTime >= WaitingTimeUntilGameOver) {
			GameOver();

			bIsWaitingGameOver = false;
			AccumulatedDeltaTime = 0.0f;
		}
	}

}

void ASimonManager::InitSimon()
{
	CurrentSequenceLength = StartingSequenceLength - 1;

	for (int i = 0; i < CurrentSequenceLength; i++)
	{
		int RandomColorIndex = FMath::RandRange(0, 3);
		ESimonBlockColorEnum RandomColor = (ESimonBlockColorEnum)RandomColorIndex;

		SimonSequence.Add(RandomColor);
	}

	bIsWaitingSequenceStart = true;
}

void ASimonManager::StartDisplaySequence()
{
	CurrentSequenceLength++;

	GenerateSequenceStep();
	DisplayNextSequenceStep();

	EnablePlayerInput(false);
	bIsDisplayingSequence = true;
}

void ASimonManager::EndDisplaySequence()
{
	bIsDisplayingSequence = false;

	EnablePlayerInput(true);
}

void ASimonManager::GenerateSequenceStep()
{
	int RandomColorIndex = FMath::RandRange(0, 3);
	ESimonBlockColorEnum RandomColor = (ESimonBlockColorEnum)RandomColorIndex;

	SimonSequence.Add(RandomColor);

	for (int i = 0; i < SimonSequence.Num(); i++) 
	{		 
		 EvaluatePlayerInputSimonSequence.Enqueue(SimonSequence[i]);
		 DisplayingSimonSequence.Enqueue(SimonSequence[i]);
	}

}

void ASimonManager::EvaluateNextSequenceStep()
{
	if (!DisplayingSimonSequence.IsEmpty())
	{
		bIsWaitingDisplayNextStepSequence = true;
	}
	else
	{
		EndDisplaySequence();
	}
}

void ASimonManager::DisplayNextSequenceStep()
{
	ESimonBlockColorEnum DisplayingSimonBlockColor;
	DisplayingSimonSequence.Dequeue(DisplayingSimonBlockColor);

	switch (DisplayingSimonBlockColor)
	{
	case ESimonBlockColorEnum::Red:
		RedSimonBlock->Highlight(true);
		break;
	case ESimonBlockColorEnum::Blue:
		BlueSimonBlock->Highlight(true);
		break;
	case ESimonBlockColorEnum::Green:
		GreenSimonBlock->Highlight(true);
		break;
	case ESimonBlockColorEnum::Yellow:
		YellowSimonBlock->Highlight(true);
		break;
	default:
		break;
	}
}

void ASimonManager::EvaluatePlayerInput(ESimonBlockColorEnum SimonBlockColor)
{
	ESimonBlockColorEnum NextStepColor;
	EvaluatePlayerInputSimonSequence.Dequeue(NextStepColor);

	if (NextStepColor != SimonBlockColor)
	{
		EvaluateSequenceResult(false);
	}
	else
	{
		if (EvaluatePlayerInputSimonSequence.IsEmpty())
		{
			EvaluateSequenceResult(true);
		}
	}
}

void ASimonManager::EnablePlayerInput(bool bEnable)
{
	bIsWaitingForPlayerInput = bEnable;

	RedSimonBlock->EnablePlayerInput(bEnable);
	BlueSimonBlock->EnablePlayerInput(bEnable);
	GreenSimonBlock->EnablePlayerInput(bEnable);
	YellowSimonBlock->EnablePlayerInput(bEnable);
}

bool ASimonManager::IsAnySimonBlockHighlighted()
{
	if (RedSimonBlock->IsHighlighted()
		|| BlueSimonBlock->IsHighlighted()
		|| GreenSimonBlock->IsHighlighted()
		|| YellowSimonBlock->IsHighlighted())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ASimonManager::EvaluateSequenceResult(bool bCorrect)
{
	bIsPlayerSuccesfullyGuessedSequence = bCorrect;
	bIsDisplayingResult = true;

	EnablePlayerInput(false);
	DisplaySequenceResult(bCorrect);
}

void ASimonManager::DisplaySequenceResult(bool bCorrect)
{
	float HighlightResultTime;

	if (bCorrect)
	{
		HighlightResultTime = CorrectResultSound->GetDuration();
		UGameplayStatics::PlaySound2D(this, CorrectResultSound);
	}
	else
	{
		HighlightResultTime = WrongResultSound->GetDuration();
		UGameplayStatics::PlaySound2D(this, WrongResultSound);
	}

	TimeDisplayingResult = HighlightResultTime;

	RedSimonBlock->DisplayHighlightResult(bCorrect, HighlightResultTime);
	BlueSimonBlock->DisplayHighlightResult(bCorrect, HighlightResultTime);
	GreenSimonBlock->DisplayHighlightResult(bCorrect, HighlightResultTime);
	YellowSimonBlock->DisplayHighlightResult(bCorrect, HighlightResultTime);
}

void ASimonManager::GameOver()
{
	ReferenceScoreCountManager->SaveScore();
	DisplayGameOver();
}

void ASimonManager::DisplayGameOver()
{
	TWeakObjectPtr<UUserWidget> pGameOverWidget = CreateWidget<UUserWidget>(GetGameInstance(), wGameOverPanel);

	if (pGameOverWidget.IsValid())
	{
		pGameOverWidget->AddToViewport();

		// Focus it and don't lock the mouse cursor
		FInputModeGameAndUI Mode;
		Mode.SetWidgetToFocus(pGameOverWidget->GetCachedWidget());
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		GetWorld()->GetFirstPlayerController()->SetInputMode(Mode);
	}
}


