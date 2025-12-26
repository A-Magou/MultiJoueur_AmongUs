// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SabotageCountDownWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_AMONGUS_API USabotageCountDownWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(visibleAnywhere, BlueprintReadOnly)
	int RemainingTime = -1;
};
