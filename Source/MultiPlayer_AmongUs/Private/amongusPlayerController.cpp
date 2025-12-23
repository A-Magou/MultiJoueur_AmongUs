// Fill out your copyright notice in the Description page of Project Settings.
#include "amongusPlayerController.h"
#include "amongus4PGameState.h"
#include "LobbyGameState.h"


void AamongusPlayerController::BeginPlay()
{
	Super::BeginPlay();

	AmongusHUD = Cast<AamongusHUD>(GetHUD());
	if (AmongusHUD)
	{
		AmongusHUD->InitializeWidgets();
	}
	

	if (Aamongus4PGameState* GS = GetWorld()->GetGameState<Aamongus4PGameState>())
	{
		GS->OnTaskChanged.AddDynamic(this, &AamongusPlayerController::OnNbTaskChanged);
	}
}

void AamongusPlayerController::OnNbTaskChanged(int newNbTask)
{
	if (AmongusHUD)
	{
		AmongusHUD->UpdateNbTask(newNbTask);
	}
}

void AamongusPlayerController::OnEtatChanged(AamongusPlayerState* changedPS, EEtatJoueur newEtat)
{
	AamongusPlayerState* myPS = GetPlayerState<AamongusPlayerState>();
	if (myPS == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController trying get PS but doesn't work"));
		return;	
	}	

	// si on est imposter, alors update HUD
	// OU, si on n'est PAS imposter, alors update HUD seulement si newEtat est "Mort"
	if (myPS->Etat == EEtatJoueur::Impostor || newEtat == EEtatJoueur::Mort)
	{
		// appeler HUD pour changer widget
		if (AmongusHUD)
        {
        	AmongusHUD->UpdateEtat(changedPS, newEtat);
        }
	}
	else if (myPS->Etat == EEtatJoueur::Crew)
	{
		if (AmongusHUD)
		{
			AmongusHUD->UpdateEtat(changedPS, newEtat);
		}
	}
}



void AamongusPlayerController::StartCountDownTimerHUD_Client()
{
	if (AmongusHUD)
	{
		AmongusHUD->ShowTimerWidget();
	}
	
	GetWorldTimerManager().SetTimer(
	CountDownTimerHandle,
	this,
	&AamongusPlayerController::UpdateCountDownTimerHUD_Client,
	0.5f,
	true
	);
}

void AamongusPlayerController::UpdateCountDownTimerHUD_Client()
{
	ALobbyGameState* LGS = GetWorld()->GetGameState<ALobbyGameState>();
	Aamongus4PGameState* FPGS = GetWorld()->GetGameState<Aamongus4PGameState>();
	if (LGS == nullptr && FPGS == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("AamongusPlayerController::UpdateCountDownTimerHUD_Client    cast gamestate failed"));
	}
	else if (LGS)
	{
		float ServerTime = GetServerWorldTime();
		float ElapsedTime = ServerTime - LGS->CountDownStartTime_Server;
		RemainingTime = LGS->CountDownDuration - ElapsedTime;

		if (AmongusHUD)
		{
			AmongusHUD->UpdateTimerWidget(RemainingTime);
		}
	}
	else if (FPGS)
	{
		float ServerTime = GetServerWorldTime();
		float ElapsedTime = ServerTime - FPGS->CountDownStartTime_Server;
		RemainingTime = FPGS->CountDownDuration - ElapsedTime;

		if (AmongusHUD)
		{
			AmongusHUD->UpdateTimerWidget(RemainingTime);
		}
	}
	
	//UE_LOG(LogTemp, Warning, TEXT("remaining time: %d"), RemainingTime);
}



//////// replication time ////////
float AamongusPlayerController::GetServerWorldTimeDelta() const
{
	return ServerWorldTimeDelta;
}

float AamongusPlayerController::GetServerWorldTime() const
{
	return GetWorld()->GetTimeSeconds() + ServerWorldTimeDelta;
}

void AamongusPlayerController::PostNetInit()
{
	Super::PostNetInit();

	if (GetLocalRole() != ROLE_Authority)
	{
		RequestWorldTime_Internal();
		if (NetworkClockUpdateFrequency > 0.0)
		{
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(
				TimerHandle,
				this,
				&AamongusPlayerController::RequestWorldTime_Internal,
				NetworkClockUpdateFrequency,
				true
			);
		}
	}
}

void AamongusPlayerController::RequestWorldTime_Internal()
{
	ServerRequestWorldTime(GetWorld()->GetTimeSeconds());
}

void AamongusPlayerController::ClientUpdateWorldTime_Implementation(float ClientTimestamp, float ServerTimestamp)
{
	// circular buffer discarding outliners
	const float RoundTripTime = GetWorld()->GetTimeSeconds() - ClientTimestamp;
	RTTCircularBuffer.Add(RoundTripTime);
	float AdjustedRTT= 0;
	
	if (RTTCircularBuffer.Num() == 10)
	{
		TArray<float> tmp = RTTCircularBuffer;
		tmp.Sort();
		for (int i = 1; i < 9; i++)
		{
			AdjustedRTT += tmp[i];
		}
		AdjustedRTT /= 8;
		RTTCircularBuffer.RemoveAt(0);
	}
	else
	{
		AdjustedRTT = RoundTripTime;
	}

	ServerWorldTimeDelta = ServerTimestamp - ClientTimestamp - (AdjustedRTT / 2);
	
	// shortest round trip time
	/*
	const float RoundTripTime = GetWorld()->GetTimeSeconds() - ClientTimestamp;
	if (RoundTripTime < ShortestRoundTripTime)
	{
		ShortestRoundTripTime = RoundTripTime;
		ServerWorldTimeDelta = ServerTimestamp - ClientTimestamp - (ShortestRoundTripTime / 2.0);
	}
	*/
}

void AamongusPlayerController::ServerRequestWorldTime_Implementation(float ClientTimestamp)
{
	const float Timestamp = GetWorld()->GetTimeSeconds();
	ClientUpdateWorldTime(ClientTimestamp, Timestamp);
}