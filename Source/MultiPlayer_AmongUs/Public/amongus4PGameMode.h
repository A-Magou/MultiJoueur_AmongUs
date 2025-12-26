// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "amongus4PGameState.h"

#include "amongus4PGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_AMONGUS_API Aamongus4PGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	Aamongus4PGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	void Initialize();


public:
	void ReturnToLobby();

	
	

public:
	// GAME
	
	//
	int maxConnectionCount = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GeneralGame")
	int ReturnToLobbyCountDownDuration;

	// if we have 4 players, then at the beginning of the game, we have
	// initialNbTaskMultiplier * nbPlayers = nbTasksRemaining
	// 2 * 4 = 8 tasks remaining
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crew")
	int initialNbTaskMultiplier = 2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crew")
	int CrewSurviveWinCountDownDuration = 600;	// after 10 minutes, if imposter don't win, then crew win

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Imposter")
	int SabotgeAbilityCoolDown = 60;	// can do sabotage 60 seconds after previous sabotage / initializatoin of the game
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Imposter")
	int SabotageImposterWinCountDownDuration = 45;	// after 45 seconds without a valid sabotage fix, imposter win

	FTimerHandle CrewSurviveWinTimerHandle;
	FTimerHandle ReturnLobbyTimerHandle;
	FTimerHandle InitializeGameTimerHandle;
	FTimerHandle SabotageTimerHandle;

	UFUNCTION()
	void StartSabotage();

	UFUNCTION()
	void GameOver(bool IsCrewWin);

	UFUNCTION(BlueprintNativeEvent)
	void CreateGameOverWidget(bool IsCrewWin);
};
