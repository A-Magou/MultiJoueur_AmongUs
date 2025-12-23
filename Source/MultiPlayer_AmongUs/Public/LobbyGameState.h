// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LobbyGameState.generated.h"

/**
 * 
 */

UCLASS()
class MULTIPLAYER_AMONGUS_API ALobbyGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION()
	void initRemainingTime(int startTime);
	
	UFUNCTION()
	void OnRep_CountDownStartTime_Server();


	
public:
	UPROPERTY(ReplicatedUsing = OnRep_CountDownStartTime_Server)
	int CountDownStartTime_Server = -1;

	UPROPERTY(Replicated)
	int CountDownDuration = -1;
	

};
