// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "Enum/playerEnum.h"

#include "amongusPlayerState.generated.h"

/**
 * 
 */



UCLASS()
class MULTIPLAYER_AMONGUS_API AamongusPlayerState : public APlayerState
{
	GENERATED_BODY()

	AamongusPlayerState();
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void CopyProperties(APlayerState* PlayerState) override;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_EtatJoueur)
	EEtatJoueur Etat = EEtatJoueur::Mort;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_ReadyJoueur)
	bool bIsReady = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_SkinIndexChanged)
	int32 SkinIndex = 0;
	
	UPROPERTY()
	FString name = "Player";


public:
	UFUNCTION()
	void OnRep_EtatJoueur();

	UFUNCTION()
	void OnreP_ReadyJoueur();

	UFUNCTION()
	void OnRep_SkinIndexChanged();


	UFUNCTION()
	void SetEtat(EEtatJoueur newEtat);

	UFUNCTION(Server, Reliable)
	void SetReady();

	UFUNCTION(Server, Reliable)
	void ServerSetSkinIndex(int32 newSkinIndex);
	
	
	
};
