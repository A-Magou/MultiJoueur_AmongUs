// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MiniGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_AMONGUS_API UMiniGameWidget : public UUserWidget
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	void CheckPassword(FString str);
};
