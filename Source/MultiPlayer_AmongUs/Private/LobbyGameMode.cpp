// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"


#include "amongusPlayerState.h"

ALobbyGameMode::ALobbyGameMode()
{
	bUseSeamlessTravel = true;
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	CheckAllPlayerReady();	// the new one is not ready so timer will be cleared
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	CheckAllPlayerReady();	// just to make sure
}

void ALobbyGameMode::ChangeLevel()
{
	UE_LOG(LogTemp, Warning, TEXT("Count down over, changing level!"));
	// /Script/Engine.World'/Game/AmongUs/Maps/LVL_Game.LVL_Game'
	///Script/Engine.Blueprint'/Game/AmongUs/GameMode/GM_Game.GM_Game'
	GetWorld()->ServerTravel(
TEXT("/Game/AmongUs/Maps/LVL_Game?listen?game=/Game/AmongUs/GameMode/GM_Game.GM_Game_C"),
	false,	// it's seemless
	false	// players are notified
	);
}

void ALobbyGameMode::CheckAllPlayerReady()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	
	ALobbyGameState* LGS = GetGameState<ALobbyGameState>();
	if (LGS)
	{
		AamongusPlayerState* AmPS;
		
		TArray<APlayerState*> AllPS = LGS->PlayerArray;
		for (int i = 0; i < AllPS.Num(); i++)
		{
			AmPS = Cast<AamongusPlayerState>(AllPS[i]);
			if (AmPS)
			{
				// If 1 player is not ready, then we don't want to travel
				if (! AmPS->bIsReady)
				{
					// If already counting, then stop and clear the timer
					if (TimerManager.TimerExists(ChangeLevelTimerHandle))
					{
						TimerManager.ClearTimer(ChangeLevelTimerHandle);
					}
					UE_LOG(LogTemp, Warning, TEXT("at least 1 player is not ready"));
					return;
				}
			}
			else UE_LOG(LogTemp, Warning, TEXT("amPS INVALID fo some reason"));
		}

		//LGS->startLoseTimeTimer();
		LGS->CountDownStartTime_Server = GetWorld()->GetTimeSeconds();
		LGS->OnRep_CountDownStartTime_Server();
		LGS->CountDownDuration = ChangeLevelCountDownDuration;

		UE_LOG(LogTemp, Warning, TEXT("ALL PLAYER READY, STARTING TIMER..."));
		if (TimerManager.IsTimerActive(ChangeLevelTimerHandle)) return;	// idk, just for safety?
		// set a timer of ChangeLevelCountDownDuration (let's say 30 sec). After 30sec, change level.
		TimerManager.SetTimer(
			ChangeLevelTimerHandle,
			this,
			&ALobbyGameMode::ChangeLevel,
			ChangeLevelCountDownDuration,
			false
		);
	}
	else return; // lobby GameState invalid

}
