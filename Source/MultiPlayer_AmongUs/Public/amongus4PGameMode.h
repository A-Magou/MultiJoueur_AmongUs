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


protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

public:
	void GameOver();

	
	

public:
	int maxConnectionCount = 4;

	// if we have 4 players, then at the beginning of the game, we have
	// initialNbTaskMultiplier * nbPlayers = nbTasksRemaining
	// 2 * 4 = 8 tasks remaining
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tasks")
	int initialNbTaskMultiplier = 2;
	
	UPROPERTY()
	int ReturnLobbyCountDownDuration = 60;

	FTimerHandle ReturnLobbyTimerHandle;




	
};
