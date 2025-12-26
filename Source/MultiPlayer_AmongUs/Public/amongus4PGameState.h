// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "amongusPlayerState.h"

#include "amongus4PGameState.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTaskChanged, int32, newNbTask);


UCLASS()
class MULTIPLAYER_AMONGUS_API Aamongus4PGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:

	UFUNCTION()
	void InitializeEtat();

	UFUNCTION()
	void InitializeTask(int multiplier);


	UFUNCTION()
	void onRep_nbTaskRemaning();

	UFUNCTION()
	void setNbTaskRemaning(int newNbTR);

	UFUNCTION()
	void OnRep_GameCountDownStartTime_Server();

	UFUNCTION()
	void OnRep_SabotageAblityEndTime_Server();

	UFUNCTION()
	void OnRep_SabotageAblityStartTime_Server();

public:
	// GAME
	
	/// tasks ///
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = onRep_nbTaskRemaning)
	int nbTaskRemaning = 9999;
	
	UPROPERTY()
	FOnTaskChanged OnTaskChanged;

	/// Crew ///
	UPROPERTY(ReplicatedUsing = OnRep_GameCountDownStartTime_Server)
	int GameCountDownStartTime_Server = -1;
	
	UPROPERTY(Replicated)
	int CountDownDuration = -1;
	
	/// Imposter ///
	UPROPERTY(ReplicatedUsing = OnRep_SabotageAblityEndTime_Server)
	float SabotageAblityEndTime_Server = -1; // when sabotage ends (fixed by crew), start ability cooldown
	
	UPROPERTY(ReplicatedUsing = OnRep_SabotageAblityStartTime_Server)
	float SabotageAblityStartTime_Server = -1;	// when sabotage ends (ends itself, never fixed by crew), imposter win

	UFUNCTION()
	bool IsSabotageAbilityReady();
};
