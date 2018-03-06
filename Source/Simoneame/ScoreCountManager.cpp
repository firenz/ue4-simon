// Fill out your copyright notice in the Description page of Project Settings.

#include "ScoreCountManager.h"

#include "Engine.h"
#include "Blueprint/UserWidget.h"
#include "TextWidgetTypes.h"
#include "TextBlock.h"

#include "SaveGameManager.h"

// Sets default values
AScoreCountManager::AScoreCountManager()
	: ScoreCount(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AScoreCountManager::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ASaveGameManager> ActorItr(GetWorld(), ASaveGameManager::StaticClass()); ActorItr; ++ActorItr)
	{
		ReferenceSaveGameManager = *ActorItr;

		if (ReferenceSaveGameManager.IsValid())
		{
			break;
		}
	}

	InitDisplayScoreCount();
}

// Called every frame
void AScoreCountManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AScoreCountManager::InitDisplayScoreCount()
{
	TWeakObjectPtr<UUserWidget> pScoreCountPanel = CreateWidget<UUserWidget>(GetGameInstance(), wScoreCount);

	if (pScoreCountPanel.IsValid())
	{
		pScoreCountPanel->AddToViewport();
		pScoreCountText = (UTextBlock*) pScoreCountPanel->GetWidgetFromName("ScoreText");

		// Focus it and don't lock the mouse cursor
		FInputModeGameAndUI Mode;
		Mode.SetWidgetToFocus(pScoreCountPanel->GetCachedWidget());
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		GetWorld()->GetFirstPlayerController()->SetInputMode(Mode);
	}

	UpdateDisplayScoreCount();
}

void AScoreCountManager::UpdateDisplayScoreCount()
{
	pScoreCountText->SetText(FText::AsNumber(ScoreCount));
}

void AScoreCountManager::AddScoreCount()
{
	ScoreCount++;
	UpdateDisplayScoreCount();
}

void AScoreCountManager::SaveScore()
{
	if (ReferenceSaveGameManager.IsValid())
	{
		ReferenceSaveGameManager->SaveScore(ScoreCount);
	}

}

