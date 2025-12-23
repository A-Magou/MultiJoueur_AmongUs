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
	DOREPLIFETIME(Aamongus4PGameState, CountDownStartTime_Server);
	DOREPLIFETIME(Aamongus4PGameState, CountDownDuration);
}

void Aamongus4PGameState::InitializeEtat()
{
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
			//players[i]->GetPlayerController()->GetPlayerState<AamongusPlayerState>();
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
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("give roles PS cast FAIL"));
			}
		}
		
	}
}

void Aamongus4PGameState::InitializeSkin()
{
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
				GM->GameOver();
			}
		}
	}
}

void Aamongus4PGameState::OnRep_CountDownStartTime_Server()
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
