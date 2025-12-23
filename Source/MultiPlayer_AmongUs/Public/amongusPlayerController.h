// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "amongusHUD.h"
#include "amongusPlayerState.h"
#include "MultiPlayer_AmongUsCharacter.h"
#include "Enum/playerEnum.h"
#include "MultiPlayer_AmongUsPlayerController.h"

#include "amongusPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_AMONGUS_API AamongusPlayerController : public AMultiPlayer_AmongUsPlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY()
	AamongusHUD* AmongusHUD;

	UPROPERTY()
	FTimerHandle CountDownTimerHandle;

	int RemainingTime = -1;

protected:
	void BeginPlay() override;
	
public:
	UFUNCTION()
	void OnEtatChanged(AamongusPlayerState* changedPS, EEtatJoueur newEtat);
	
	UFUNCTION()
	void OnNbTaskChanged(int newNbTask);

	UFUNCTION()
	void StartCountDownTimerHUD_Client();

	UFUNCTION()
	void UpdateCountDownTimerHUD_Client();

	

///// time replication /////
	
#pragma region NetworkClockSync

protected:
	UPROPERTY(EditDefaultsOnly, Category = GameState)
	float NetworkClockUpdateFrequency = 1.0;

private:
	float ServerWorldTimeDelta = 0.0;
	TArray<float> RTTCircularBuffer;
	//float ShortestRoundTripTime = BIG_NUMBER;

public:
	UFUNCTION(BlueprintPure)
	float GetServerWorldTimeDelta() const;

	UFUNCTION(BlueprintPure)
	float GetServerWorldTime() const;

	void PostNetInit() override;

private:
	void RequestWorldTime_Internal();

	UFUNCTION(Server, Unreliable)
	void ServerRequestWorldTime(float ClientTimestamp);

	UFUNCTION(Client, Unreliable)
	void ClientUpdateWorldTime(float ClientTimestamp, float ServerTimestamp);

#pragma endregion NetworkClockSync

	
};
