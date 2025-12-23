// Fill out your copyright notice in the Description page of Project Settings.
#include "amongusHUD.h"
#include "amongusPlayerController.h"


void AamongusHUD::UpdateEtat(AamongusPlayerState* changedPS, EEtatJoueur etat)
{
	// changer la valeur d'etat du widget selon le pointeur de PS
	if (etatWidgetInstance)
	{
		etatWidgetInstance->etat = UEnum::GetDisplayValueAsText(etat).ToString();
	}
	
}

void AamongusHUD::UpdateNbTask(int newNbTask)
{
	// changer la valeur de nb de tache du widget
	if (nbTaskWidgetInstance)
	{
		nbTaskWidgetInstance->nbTask = newNbTask;
	}
}

void AamongusHUD::InitializeWidgets()
{
	if (etatWidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("EtatWidgetClass is null"));
		if (nbTaskWidgetClass == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("nbTaskWidgetClass is null"));
		}
		return;
	}
	if (etatWidgetClass && nbTaskWidgetClass)
	{
		
		UEtatWidget* etatWidget = CreateWidget<UEtatWidget>(GetWorld(), etatWidgetClass);
        UTaskWidget* taskWidget = CreateWidget<UTaskWidget>(GetWorld(), nbTaskWidgetClass);

		if (etatWidget && taskWidget)
		{
	        etatWidgetInstance = etatWidget;
            nbTaskWidgetInstance = taskWidget;
        
            etatWidget->AddToViewport();
            taskWidget->AddToViewport();
		}

	}


	
}

void AamongusHUD::ShowTimerWidget()
{
	if (timerWidgetClass)
	{
		UTimerWidget* TimerWidget = CreateWidget<UTimerWidget>(GetWorld(), timerWidgetClass);
		if (TimerWidget)
		{
			timerWidgetInstance = TimerWidget;
			TimerWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("TimerWidget is null"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("timerWidgetClass is null"));
	}
}

void AamongusHUD::UpdateTimerWidget(int NewTime)
{
	if (timerWidgetInstance)
	{
		timerWidgetInstance->RemainingTime = NewTime;
	}
}

void AamongusHUD::BeginPlay()
{
	Super::BeginPlay();
	InitializeWidgets();
}
