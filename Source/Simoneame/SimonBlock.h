// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ESimonBlockType.h"
#include "SimonBlock.generated.h"

class ASimonManager;

UCLASS()
class SIMONEAME_API ASimonBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimonBlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	void SetColor(ESimonBlockColorEnum NewColor);

	// Handle the block being clicked
	UFUNCTION()
		void BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);

	// Handle the block being touched 
	UFUNCTION()
		void OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

	void OnSelected();

	void Highlight(bool bOn);
	void DisplayHighlightResult(bool bCorrect, float HighlightResultTime);

	void EnablePlayerInput(bool bEnable);

private:
	// References
	UPROPERTY()
	TWeakObjectPtr<ASimonManager> ReferenceSimonManager;

	UPROPERTY(EditAnywhere, Category = "SimonBlock")
	ESimonBlockColorEnum BlockColor;

	// Dummy root component
	TWeakObjectPtr<USceneComponent> RootReference;

	// StaticMesh component for the clickable block
	TWeakObjectPtr<UStaticMeshComponent> BlockMesh;

	class USoundWave* SimonSound;

	// Pointer to white material used on inactive block
	class UMaterial* CurrentColorMaterial;
	
	// Pointer to basic material used on inactive block
	class UMaterial* BaseMaterial;
	// Pointer to color material used on active blocks
	class UMaterial* ColorMaterial;
	// Pointer to wrong color material used on highlighted wrong answer blocks
	class UMaterial* WrongColorMaterial;
	// Pointer to correct color material used on highlighted correct answer blocks
	class UMaterial* CorrectColorMaterial;

	//UPROPERTY(EditAnywhere, Category = "SimonBlock")
	float HighlightTimeInSeconds;
	float AccumulatedDeltaTime;

	bool bIsHighlighted;
	bool bCanPlaySimonSound;
	bool bEnabledPlayerInput;

public:

	// Returns Simon Block Color Type
	FORCEINLINE ESimonBlockColorEnum GetColor() const { return BlockColor; }
	// Returns is Simon Block is highlighted
	FORCEINLINE bool IsHighlighted() const { return bIsHighlighted; }
	// Returns how many seconds the Simon Block is highlighted
	FORCEINLINE float GetHighlightTime() const { return HighlightTimeInSeconds; }

};
