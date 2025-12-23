// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameState.h"

#include "amongusPlayerController.h"

#include "Net/UnrealNetwork.h"

void ALobbyGameState::BeginPlay()
{
	Super::BeginPlay();

	
}

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGameState, CountDownStartTime_Server);
	DOREPLIFETIME(ALobbyGameState, CountDownDuration);
}

void ALobbyGameState::initRemainingTime(int startTime)
{
	if (startTime >= 0)
	{
		CountDownStartTime_Server = startTime;
	}
}

void ALobbyGameState::OnRep_CountDownStartTime_Server()
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Iterator->Get();
		if (PC)
		{
			AamongusPlayerController* AmongusPC = Cast<AamongusPlayerController>(PC);
			if (AmongusPC)
			{
				AmongusPC->StartCountDownTimerHUD_Client();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("OnRep_CountDownStartTime_Server CAST AMONGUSPC FAILED"));
			}
		}
		else
        {
        	UE_LOG(LogTemp, Error, TEXT("OnRep_CountDownStartTime_Server CAST PC FAILED"));
        }
	}

}
