// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Enum/playerEnum.h"

#include "EtatWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_AMONGUS_API UEtatWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(visibleAnywhere, BlueprintReadOnly)
	FString etat;
};
