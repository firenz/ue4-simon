// Fill out your copyright notice in the Description page of Project Settings.

#include "SimonBlock.h"

#include "Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Sound/SoundWave.h"
#include "Kismet/GameplayStatics.h"

#include "SimonManager.h"

// Sets default values
ASimonBlock::ASimonBlock()
	: BlockColor(ESimonBlockColorEnum::NoType), bIsHighlighted(false), bCanPlaySimonSound(true), bEnabledPlayerInput(true),
		HighlightTimeInSeconds(3.f), AccumulatedDeltaTime(0.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a root scene component and assigns a reference to it
	RootReference = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	if (RootReference.IsValid())
	{
		RootComponent = RootReference.Get();
	}

	// Create static mesh component
	 BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh"));

	 auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Meshes/SimonBlockMesh.SimonBlockMesh'"));

	 if (MeshAsset.Succeeded())
	 {
		BlockMesh->SetStaticMesh(MeshAsset.Object);
		BlockMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.25f));
		BlockMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	 }

	 BlockMesh->SetupAttachment(RootReference.Get());

	 // Assign material and save reference of it
	 auto MaterialAsset = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Game/Materials/BaseSimonBlockMaterial.BaseSimonBlockMaterial'"));

	 if (MaterialAsset.Succeeded())
	 {
		BlockMesh->SetMaterial(0, MaterialAsset.Object);
		 // Save a pointer to the base material
		 BaseMaterial = MaterialAsset.Object;
		 CurrentColorMaterial = MaterialAsset.Object;
	 }

	 // Assign material and save reference of it
	 auto CorrectMaterialAsset = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Game/Materials/CorrectSimonBlockMaterial.CorrectSimonBlockMaterial'"));

	 if (CorrectMaterialAsset.Succeeded())
	 {
		 // Save a pointer to the correct material
		 CorrectColorMaterial = CorrectMaterialAsset.Object;
	 }

	 // Assign material and save reference of it
	 auto WrongMaterialAsset = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Game/Materials/WrongSimonBlockMaterial.WrongSimonBlockMaterial'"));

	 if (WrongMaterialAsset.Succeeded())
	 {
		 // Save a pointer to the correct material
		 WrongColorMaterial = WrongMaterialAsset.Object;
	 }

	 // Adding listeners to execute when clicked or touched the mesh
	 BlockMesh->OnClicked.AddDynamic(this, &ASimonBlock::BlockClicked);
	 BlockMesh->OnInputTouchBegin.AddDynamic(this, &ASimonBlock::OnFingerPressedBlock);
}

// Called when the game starts or when spawned
void ASimonBlock::BeginPlay()
{
	Super::BeginPlay();

	SetColor(BlockColor);
	HighlightTimeInSeconds = SimonSound->GetDuration();

	for (TActorIterator<ASimonManager> ActorItr(GetWorld(), ASimonManager::StaticClass()); ActorItr; ++ActorItr)
	{
		ReferenceSimonManager = *ActorItr;

		if (ReferenceSimonManager.IsValid())
		{
			break;
		}
	}
}

// Called every frame
void ASimonBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Timer for controlling the highlight duration
	if (bIsHighlighted)
	{
		AccumulatedDeltaTime += DeltaTime;

		if (AccumulatedDeltaTime >= HighlightTimeInSeconds)
		{
			Highlight(false);

			AccumulatedDeltaTime = 0.0f;

			// If we just only displaying a new sequence, after highlighting the button, calls the method
			// to display the next sequence block (or finishes the sequence)
			if (ReferenceSimonManager->IsDisplayingSequence())
			{
				ReferenceSimonManager->EvaluateNextSequenceStep();
			}
			// In case the displaying sequence is finished and the game is waiting for player input, 
			// in this case when the highlighting (produced for the player for selecting the block) is finished,
			// calls the method to evaluate if this block selected by the player is correct block or not
			else if (ReferenceSimonManager->IsWaitingForPlayerInput())
			{
				ReferenceSimonManager->EvaluatePlayerInput(GetColor());
			}
		}
	}

}

void ASimonBlock::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	OnSelected();
}

void ASimonBlock::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	OnSelected();
}

void ASimonBlock::OnSelected()
{
	if (bEnabledPlayerInput && !ReferenceSimonManager->IsAnySimonBlockHighlighted())
	{
		Highlight(true);
	}
}

void ASimonBlock::Highlight(bool bOn)
{
	if (BlockMesh.IsValid())
	{
		if (bOn) 
		{
			BlockMesh->SetMaterial(0, CurrentColorMaterial);
			
			if (bCanPlaySimonSound)
			{
				UGameplayStatics::PlaySound2D(this, SimonSound);
			}
		}
		else
		{
			BlockMesh->SetMaterial(0, BaseMaterial);

			// Resets the simon block material and is allowed again to play its simon sounds
			CurrentColorMaterial = ColorMaterial;
			HighlightTimeInSeconds = SimonSound->GetDuration();
			bCanPlaySimonSound = true;
		}

		bIsHighlighted = bOn;
	}

}

void ASimonBlock::SetColor(ESimonBlockColorEnum NewColor)
{
	switch (NewColor)
	{
	case ESimonBlockColorEnum::Red:
		ColorMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), nullptr, TEXT("Material'/Game/Materials/RedSimonBlockMaterial.RedSimonBlockMaterial'")));
		SimonSound = Cast<USoundWave>(StaticLoadObject(USoundWave::StaticClass(), nullptr, TEXT("SoundWave'/Game/Sounds/1.1'")));
		break;
	case ESimonBlockColorEnum::Blue:
		ColorMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), nullptr, TEXT("Material'/Game/Materials/BlueSimonBlockMaterial.BlueSimonBlockMaterial'")));
		SimonSound = Cast<USoundWave>(StaticLoadObject(USoundWave::StaticClass(), nullptr, TEXT("SoundWave'/Game/Sounds/2.2'")));
		break;
	case ESimonBlockColorEnum::Green:
		ColorMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), nullptr, TEXT("Material'/Game/Materials/GreenSimonBlockMaterial.GreenSimonBlockMaterial'")));
		SimonSound = Cast<USoundWave>(StaticLoadObject(USoundWave::StaticClass(), nullptr, TEXT("SoundWave'/Game/Sounds/3.3'")));
		break;
	case ESimonBlockColorEnum::Yellow:
		ColorMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), nullptr, TEXT("Material'/Game/Materials/YellowSimonBlockMaterial.YellowSimonBlockMaterial'")));
		SimonSound = Cast<USoundWave>(StaticLoadObject(USoundWave::StaticClass(), nullptr, TEXT("SoundWave'/Game/Sounds/4.4'")));
		break;
	default:
		ColorMaterial = BaseMaterial;
		break;
	}

	CurrentColorMaterial = ColorMaterial;
}

void ASimonBlock::DisplayHighlightResult(bool bCorrect, float HighlightResultTime)
{
	if (bCorrect)
	{
		CurrentColorMaterial = CorrectColorMaterial;
		
	}
	else
	{
		CurrentColorMaterial = WrongColorMaterial;
	}

	HighlightTimeInSeconds = HighlightResultTime;
	bCanPlaySimonSound = false;
	Highlight(true);
}

void ASimonBlock::EnablePlayerInput(bool bEnable)
{
	bEnabledPlayerInput = bEnable;
}

