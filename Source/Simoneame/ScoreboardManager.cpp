// Fill out your copyright notice in the Description page of Project Settings.

#include "ScoreboardManager.h"

#include "Engine.h"
#include "Blueprint/UserWidget.h"
#include "TextWidgetTypes.h"
#include "VerticalBox.h"
#include "TextBlock.h"
#include "Border.h"

#include "SaveGameManager.h"

// Sets default values
AScoreboardManager::AScoreboardManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AScoreboardManager::BeginPlay()
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

	DisplayScoreboard();
	DisplayScoreboardElements();
}

// Called every frame
void AScoreboardManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AScoreboardManager::DisplayScoreboard()
{
	TWeakObjectPtr<UUserWidget> pScoreboard = CreateWidget<UUserWidget>(GetGameInstance(), wScoreboard);

	if (pScoreboard.IsValid())
	{
		pScoreboard->AddToViewport();

		// Focus it and don't lock the mouse cursor
		FInputModeUIOnly Mode;
		Mode.SetWidgetToFocus(pScoreboard->GetCachedWidget());
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		GetWorld()->GetFirstPlayerController()->SetInputMode(Mode);

		pScoreboardPanel = (UVerticalBox*) pScoreboard->GetWidgetFromName("ScoresPanel");
	}
}

void AScoreboardManager::DisplayScoreboardElements()
{
	TArray<int> ScoresArray = ReferenceSaveGameManager->LoadScores();

	for (int i = 0; i < ScoresArray.Num(); i++)
	{
		TWeakObjectPtr<UUserWidget> pScoreboardElement = CreateWidget<UUserWidget>(GetGameInstance(), wScoreboardElement);

		if (pScoreboardElement.IsValid())
		{
			TWeakObjectPtr<UBorder> pScoreboardElementBorder = (UBorder*) pScoreboardElement->GetWidgetFromName("ScoreElement");
			TWeakObjectPtr<UTextBlock> pScoreboardElementScoreText = (UTextBlock*) pScoreboardElement->GetWidgetFromName("ScoreText");

			pScoreboardPanel->AddChildToVerticalBox(pScoreboardElementBorder.Get());

			pScoreboardElementScoreText->SetText(FText::AsNumber(ScoresArray[i]));
		}
	}
}


