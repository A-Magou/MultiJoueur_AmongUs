// Fill out your copyright notice in the Description page of Project Settings.


#include "amongus4PGameState.h"

#include "GameFramework/GameState.h"

#include "amongus4PGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "amongusPlayerController.h"

#include "Net/UnrealNetwork.h"

void Aamongus4PGameState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(Aamongus4PGameState, nbTaskRemaning);

	// level duration, changing level
	DOREPLIFETIME(Aamongus4PGameState, GameCountDownStartTime_Server);
	DOREPLIFETIME(Aamongus4PGameState, CountDownDuration);
	DOREPLIFETIME(Aamongus4PGameState, SabotageAblityEndTime_Server);
	DOREPLIFETIME(Aamongus4PGameState, SabotageAblityStartTime_Server);
}

void Aamongus4PGameState::InitializeEtat()
{
	//UE_LOG(LogTemp, Warning, TEXT("InitializeEtat called"));
	if (HasAuthority())
	{
		TArray players = PlayerArray;
		
		int playerNumber = PlayerArray.Num();
		if (playerNumber <= 1)
		{
			UE_LOG(LogTemp, Error, TEXT("Player number is negative"));
			return;
		}

		// calculate how many imposters we need in this game
		int imposterNumber = FMath::Floor(playerNumber / 4);
		if (imposterNumber <= 0)
		{
			imposterNumber = 1;
		}
		UE_LOG(LogTemp, Warning, TEXT("Imposter number is %d"), imposterNumber);

		// pick index for random imposters
		TArray<int> impostersIndex;
		int randomIndex = -1;
		bool isRandomValid = false;
		for (int i = 0; i < imposterNumber; i++)
		{
			while (!isRandomValid)
			{
				randomIndex = FMath::RandRange(0, (playerNumber - 1));
				// the random index will be valid only if it was not picked yet
				// avoiding picking same player as imposter
				if (!impostersIndex.Contains(randomIndex))
				{
					isRandomValid = true;
				}
			}
			impostersIndex.Add(randomIndex);

			isRandomValid = false;
		}

		// give roles
		for (int i = 0; i < playerNumber; i++)
		{
			AamongusPlayerState* PS = Cast<AamongusPlayerState>(players[i]);
			if (PS)
			{
				if (impostersIndex.Contains(i))
                {
                	PS->SetEtat(EEtatJoueur::Impostor);
                }
				else
				{
					PS->SetEtat(EEtatJoueur::Crew);
				}
				PS->OnRep_EtatJoueur();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("give roles PS cast FAIL"));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GS has NO authority"));
	}
}

void Aamongus4PGameState::InitializeTask(int multiplier)
{
	// see initialNbTaskMultiplier in GameMode (amongus4PGameMode.h)
	int playerNumber = PlayerArray.Num();
	nbTaskRemaning = multiplier * playerNumber;
	UE_LOG(LogTemp, Warning, TEXT("nbTaskRemaning is %d"), nbTaskRemaning);
}

void Aamongus4PGameState::onRep_nbTaskRemaning()
{
	//UE_LOG(LogTemp, Warning, TEXT("TASK REMAINING : %d"), nbTaskRemaning);
	OnTaskChanged.Broadcast(this->nbTaskRemaning);
	if (nbTaskRemaning <= 0)
	{
		AGameModeBase*  CurrentGM = UGameplayStatics::GetGameMode(GetWorld());
		if (CurrentGM)
		{
			Aamongus4PGameMode* GM = Cast<Aamongus4PGameMode>(CurrentGM);
			if (GM)
			{
				GM->GameOver(true);
			}
		}
	}
}

void Aamongus4PGameState::setNbTaskRemaning(int newNbTR)
{
	this->nbTaskRemaning = newNbTR;
	if (nbTaskRemaning <= 0)
	{
		AGameModeBase*  CurrentGM = UGameplayStatics::GetGameMode(GetWorld());
		if (CurrentGM)
		{
			Aamongus4PGameMode* GM = Cast<Aamongus4PGameMode>(CurrentGM);
			if (GM)
			{
				GM->ReturnToLobby();
			}
		}
	}
}

void Aamongus4PGameState::OnRep_GameCountDownStartTime_Server()
{
	//UE_LOG(LogTemp, Warning, TEXT("OnRep_CountDownStartTime_Server"));
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



/// Sabotage ///
void Aamongus4PGameState::OnRep_SabotageAblityEndTime_Server()
{
	APlayerController* PC = GetGameInstance()->GetFirstLocalPlayerController();
 	if (PC)
 	{
 		AamongusPlayerController* amPC = Cast<AamongusPlayerController>(PC);
 		if (amPC)
 		{
 			//UE_LOG(LogTemp, Warning, TEXT("amGS try to call amPC UpdateSabotageCountDownTimerHUD_Client"));
 			amPC->UpdateSabotageCountDownTimerHUD_Client(SabotageAblityEndTime_Server);
 		}
 		else
 		{
 			UE_LOG(LogTemp, Error, TEXT("OnRep_SabotageAblityEndTime_Server amPC FAILED"));
 		}
 	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OnRep_SabotageAblityEndTime_Server PC FAILED"));
	}
}

void Aamongus4PGameState::OnRep_SabotageAblityStartTime_Server()
{
}

bool Aamongus4PGameState::IsSabotageAbilityReady()
{
	if (SabotageAblityEndTime_Server == -1) return true;
	
	return false;
	//float now = GetWorld()->GetTimeSeconds();
	//return (FMath::Abs(now - SabotageAblityStartTime_Server) > 0.1);
}
/// Sabotage end ///


