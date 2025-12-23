// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "amongusPlayerState.h"
#include "Enum/playerEnum.h"

#include "EtatWidget.h"
#include "TaskWidget.h"
#include "TimerWidget.h"

#include "amongusHUD.generated.h"

/**
 * 
 */


UCLASS()
class MULTIPLAYER_AMONGUS_API AamongusHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	void UpdateEtat(AamongusPlayerState* changedPS, EEtatJoueur etat);

	void UpdateNbTask(int newNbTask);

	void InitializeWidgets();

	void ShowTimerWidget();

	void UpdateTimerWidget(int NewTime);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UEtatWidget> etatWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UTaskWidget> nbTaskWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UTimerWidget> timerWidgetClass;

	UPROPERTY()
	UEtatWidget* etatWidgetInstance;
	UPROPERTY()
	UTaskWidget* nbTaskWidgetInstance;
	UPROPERTY()
	UTimerWidget* timerWidgetInstance;
	
};
