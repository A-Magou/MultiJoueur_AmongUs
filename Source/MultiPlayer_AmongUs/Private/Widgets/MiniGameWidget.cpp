// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MiniGameWidget.h"


#include "amongus4PGameState.h"

void UMiniGameWidget::CheckPassword(FString str)
{
	if (str == "TianlZhakPhil")
	{
		Aamongus4PGameState* amGS = Cast<Aamongus4PGameState>(GetWorld()->GetGameState());
		if (amGS)
		{
			amGS->nbTaskRemaning = amGS->nbTaskRemaning - 1;
			amGS->onRep_nbTaskRemaning();
		}
	}
	RemoveFromParent();
}
