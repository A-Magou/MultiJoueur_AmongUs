// Fill out your copyright notice in the Description page of Project Settings.


#include "amongusPlayerState.h"

#include "amongusPlayerController.h"
#include "amongus4PGameMode.h"
#include "LobbyGameMode.h"

#include "Net/UnrealNetwork.h"


AamongusPlayerState::AamongusPlayerState()
{
	bReplicates = true;
}

void AamongusPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AamongusPlayerState, Etat);
	DOREPLIFETIME(AamongusPlayerState, SkinIndex);
	DOREPLIFETIME(AamongusPlayerState, bIsReady);
}

void AamongusPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	AamongusPlayerState* NewPS = Cast<AamongusPlayerState>(PlayerState);
	if (NewPS)
	{
		NewPS->SkinIndex = this->SkinIndex;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("copy properties new ps cast FAILED"));
	}
}

void AamongusPlayerState::OnRep_EtatJoueur()
{
	AamongusPlayerController* PC = Cast<AamongusPlayerController>(GetOwner());
	if (PC)
	{
		PC->OnEtatChanged(this, Etat);
	}
}

void AamongusPlayerState::OnRep_SkinJoueur()
{
	APawn* P = GetPawn();
	if (P)
	{
		AMultiPlayer_AmongUsCharacter* Player = Cast<AMultiPlayer_AmongUsCharacter>(P);
		if (Player)
		{
			Player->UpdateSkinFromIndex(SkinIndex);
		}
	}
}



void AamongusPlayerState::SetEtat(EEtatJoueur newEtat)
{
	this->Etat = newEtat;
}

void AamongusPlayerState::SetSkin(int NewSkinIndex)
{
	this->SkinIndex = NewSkinIndex;
	UE_LOG(LogTemp, Warning, TEXT("PlayerState skin index %d"), SkinIndex);

	if (GetNetMode() != NM_Client) 
	{
		OnRep_SkinJoueur(); 
	}
}

// READY
void AamongusPlayerState::OnreP_ReadyJoueur()
{
}

void AamongusPlayerState::SetReady_Implementation()
{
	bIsReady = !bIsReady;
	
	UWorld* World = GetWorld();
	if (World)
	{
		ALobbyGameMode* GM = Cast<ALobbyGameMode>(World->GetAuthGameMode());
		if (GM)
		{
			GM->CheckAllPlayerReady();
		}
	}
	
	if (GetNetMode() != NM_DedicatedServer)
	{
		OnreP_ReadyJoueur();
	}
}
// READY END
