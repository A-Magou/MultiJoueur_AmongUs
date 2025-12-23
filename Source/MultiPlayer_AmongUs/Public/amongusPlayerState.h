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
	EEtatJoueur Etat = EEtatJoueur::Crew;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_SkinJoueur)
	int SkinIndex = -1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_ReadyJoueur)
	bool bIsReady = false;
	
	UPROPERTY()
	FString name = "Player";


public:
	UFUNCTION()
	void OnRep_EtatJoueur();

	UFUNCTION()
	void OnRep_SkinJoueur();

	UFUNCTION()
	void OnreP_ReadyJoueur();


	UFUNCTION()
	void SetEtat(EEtatJoueur newEtat);

	UFUNCTION()
	void SetSkin(int NewSkinIndex);

	UFUNCTION(Server, Reliable)
	void SetReady();

	
	
};
