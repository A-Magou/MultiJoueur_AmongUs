// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimerWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_AMONGUS_API UTimerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "time")
	int RemainingTime = -1;


public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "time")
	void OnTimeChanged();
};
