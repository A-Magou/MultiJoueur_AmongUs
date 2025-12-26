// Fill out your copyright notice in the Description page of Project Settings.


#include "amongus4PGameMode.h"



#include "Enum/playerEnum.h"

Aamongus4PGameMode::Aamongus4PGameMode()
{
	bUseSeamlessTravel = true;
}

void Aamongus4PGameMode::BeginPlay()
{
	Super::BeginPlay();

	// delay a bit to make sure everything is well initialized
	GetWorldTimerManager().SetTimer(
		InitializeGameTimerHandle,
		this,
		&Aamongus4PGameMode::Initialize,
		1.5,
		false
	);
}

void Aamongus4PGameMode::Initialize()
{
	Aamongus4PGameState* AmGS = GetGameState<Aamongus4PGameState>();
 	if (AmGS)
 	{
 		AmGS->GameCountDownStartTime_Server = GetWorld()->GetTimeSeconds();
 		AmGS->CountDownDuration = CrewSurviveWinCountDownDuration;
 		AmGS->OnRep_GameCountDownStartTime_Server();
 		
 		AmGS->InitializeEtat();
 		AmGS->InitializeTask(initialNbTaskMultiplier);
 
 		// set a timer of ReturnLobbyCountDownDuration (600sec). After 600sec, crew survive and win.
 		GetWorldTimerManager().SetTimer(
 			CrewSurviveWinTimerHandle,
 			this,
 			&Aamongus4PGameMode::ReturnToLobby,
 			CrewSurviveWinCountDownDuration,
 			false
 		);
 	}
}

void Aamongus4PGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void Aamongus4PGameMode::ReturnToLobby()
{
	GetWorld()->ServerTravel(
	TEXT("/Game/AmongUs/Level/LVL_Lobby?listen?game=/Game/AmongUs/GameMode/GM_Lobby.GM_Lobby"),
	false,	// it's seemless
	false	// players are notified
	);
}

void Aamongus4PGameMode::StartSabotage()
{
	FTimerDelegate GameOverDelegate;
	GameOverDelegate.BindUFunction(this, "GameOver", false);

	Aamongus4PGameState* amGS = GetGameState<Aamongus4PGameState>();
	if (amGS)
	{
		amGS->SabotageAblityEndTime_Server = (GetWorld()->GetTimeSeconds() + SabotageImposterWinCountDownDuration);
		amGS->OnRep_SabotageAblityEndTime_Server();
	}
	
	GetWorldTimerManager().SetTimer(
		ReturnLobbyTimerHandle,
		GameOverDelegate,
		SabotageImposterWinCountDownDuration,
		false
	);
}

void Aamongus4PGameMode::GameOver(bool IsCrewWin)
{
	if (IsCrewWin)
	{
		// create crew win widget
		
	}
	else
	{
		// create imposter win widget
	}
	
	CreateGameOverWidget(IsCrewWin);	// no time left
	
	// create count down widget
	// wait for some sec and return to lobby
	GetWorldTimerManager().SetTimer(
		ReturnLobbyTimerHandle,
		this,
		&Aamongus4PGameMode::ReturnToLobby,
		ReturnToLobbyCountDownDuration,
		false
	);
}

void Aamongus4PGameMode::CreateGameOverWidget_Implementation(bool IsCrewWin)
{
}

