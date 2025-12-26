// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enum/widgetTypeEnum.h"





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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "widgetType")
	EWidgetType TimerType = EWidgetType::Unset;
	

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "time")
	void OnTimeChanged();
};
