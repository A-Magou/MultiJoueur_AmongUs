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
		UE_LOG(LogTemp, Warning, TEXT("copy properties DONE"));
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

void AamongusPlayerState::SetEtat(EEtatJoueur newEtat)
{
	this->Etat = newEtat;
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


// SKIN

void AamongusPlayerState::OnRep_SkinIndexChanged()
{
	APawn* Pawn = GetPawn();
	if (Pawn)
	{
		AMultiPlayer_AmongUsCharacter* amCharacter = Cast<AMultiPlayer_AmongUsCharacter>(Pawn);
		if (amCharacter)
		{
			amCharacter->UpdateSkinVisual(SkinIndex);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Pawn is null"));
	}
}

void AamongusPlayerState::ServerSetSkinIndex_Implementation(int32 newSkinIndex)
{
	SkinIndex = newSkinIndex;

	OnRep_SkinIndexChanged();
}

// SKIN END

