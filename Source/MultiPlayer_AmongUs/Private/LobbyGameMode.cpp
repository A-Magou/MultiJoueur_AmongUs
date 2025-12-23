// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"


#include "amongusPlayerState.h"

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	ALobbyGameState* LGS = GetGameState<ALobbyGameState>();
	if (LGS)
	{
		//LGS->initRemainingTime(ChangeLevelCountDownDuration);
		//LGS->OnRemainingTimeChangedHandle.AddUObject(this, &ALobbyGameMode::OnRemainingTimeTimerUpdated);
	}
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	CheckAllPlayerReady();	// the new one is not ready so timer will be cleared
	/*
	ALobbyGameState* LGS = GetGameState<ALobbyGameState>();
	if (LGS)
	{
		if (LGS->PlayerArray.Num() == StartTimerConnectionCount)
		{
			UE_LOG(LogTemp, Warning, TEXT("GM start count down"));
			//LGS->startLoseTimeTimer();
			LGS->CountDownStartTime_Server = GetWorld()->GetTimeSeconds();
			LGS->CountDownDuration = ChangeLevelCountDownDuration;

			// set a timer of ChangeLevelCountDownDuration (30sec). After 30sec, change level.
			GetWorldTimerManager().SetTimer(
				ChangeLevelTimerHandle,
				this,
				&ALobbyGameMode::ChangeLevel,
				ChangeLevelCountDownDuration,
				false
			);
		}
	}
	*/
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	CheckAllPlayerReady();	// just to make sure
}

void ALobbyGameMode::ChangeLevel()
{
	UE_LOG(LogTemp, Warning, TEXT("Count down over, changing level!"));

	GetWorld()->ServerTravel(
TEXT("/Game/AmongUs/Level/LVL_4PLevel?listen?game=/Game/AmongUs/GameMode/GM_amongus4P.GM_amongus4P_C"),
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
					return;
				}
			}
		}
	}

	
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
