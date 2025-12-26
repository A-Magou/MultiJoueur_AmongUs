// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "amongusPlayerState.h"
#include "Enum/playerEnum.h"

#include "Widgets/EtatWidget.h"
#include "Widgets/TaskWidget.h"
#include "Widgets/TimerWidget.h"
#include "Widgets/SabotageCountDownWidget.h"
#include "Widgets/MiniGameWidget.h"

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
	void UpdateEtat(EEtatJoueur etat);

	void UpdateNbTask(int newNbTask);

	void InitializeWidgets();

	void CreateGlobalTimerWidget();

	void UpdateTimerWidget(int NewTime);

	void CreateSabotageCountDownWidget();

	void UpdateSabotageCountDownWidget(int NewTime);

	void CreateMiniGameWidget();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UEtatWidget> etatWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UTaskWidget> nbTaskWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UTimerWidget> timerWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<USabotageCountDownWidget> SabotageCountDownWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UMiniGameWidget> MiniGameWidgetClass;
	
	UPROPERTY()
	UEtatWidget* etatWidgetInstance;
	UPROPERTY()
	UTaskWidget* nbTaskWidgetInstance;
	UPROPERTY()
	UTimerWidget* timerWidgetInstance;
	UPROPERTY()
	USabotageCountDownWidget* sabotageCountDownWidgetInstance;
	UPROPERTY()
	UMiniGameWidget* MiniGameWidgetInstance;
	
};
