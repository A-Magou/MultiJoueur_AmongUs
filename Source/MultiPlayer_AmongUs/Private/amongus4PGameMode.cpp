// Fill out your copyright notice in the Description page of Project Settings.


#include "amongus4PGameMode.h"

void Aamongus4PGameMode::BeginPlay()
{
	Super::BeginPlay();

	Aamongus4PGameState* AmGS = GetGameState<Aamongus4PGameState>();
	if (AmGS)
	{
		AmGS->CountDownStartTime_Server = GetWorld()->GetTimeSeconds();
		AmGS->CountDownDuration = ReturnLobbyCountDownDuration;
		
		AmGS->InitializeEtat();
		AmGS->InitializeSkin();
		AmGS->InitializeTask(initialNbTaskMultiplier);

		// set a timer of ReturnLobbyCountDownDuration (60sec). After 60sec, change level.
		GetWorldTimerManager().SetTimer(
			ReturnLobbyTimerHandle,
			this,
			&Aamongus4PGameMode::GameOver,
			ReturnLobbyCountDownDuration,
			false
		);
	}
}

void Aamongus4PGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}


void Aamongus4PGameMode::GameOver()
{
	GetWorld()->ServerTravel(
	TEXT("/Game/AmongUs/Level/LVL_Lobby?listen?game=/Game/AmongUs/GameMode/GM_Lobby.GM_Lobby"),
	false,	// it's seemless
	false	// players are notified
	);
}
