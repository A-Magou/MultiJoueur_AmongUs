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
	void InitializeSkin();

	UFUNCTION()
	void InitializeTask(int multiplier);


	UFUNCTION()
	void onRep_nbTaskRemaning();

	UFUNCTION()
	void setNbTaskRemaning(int newNbTR);

	UFUNCTION()
	void OnRep_CountDownStartTime_Server();


public:
	/// tasks ///
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = onRep_nbTaskRemaning)
	int nbTaskRemaning = 9999;
	
	UPROPERTY()
	FOnTaskChanged OnTaskChanged;

	/// return to lobby ///
	UPROPERTY(ReplicatedUsing = OnRep_CountDownStartTime_Server)
	int CountDownStartTime_Server = -1;
	
	UPROPERTY(Replicated)
	int CountDownDuration = -1;
	
};
