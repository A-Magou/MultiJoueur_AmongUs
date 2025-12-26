// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "LobbyGameState.h"

#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_AMONGUS_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	ALobbyGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	
public:
	void ChangeLevel();	

	UFUNCTION()
	void CheckAllPlayerReady();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MultiPlayer")
	int StartTimerConnectionCount = 2; // server itself is included
								// set to 3 cause VRAM not enough idk

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MultiPlayer")
	int ChangeLevelCountDownDuration = 15;

	FTimerHandle ChangeLevelTimerHandle;
	


	
	
};
