// Fill out your copyright notice in the Description page of Project Settings.
#include "Bouton.h"
#include "MultiPlayer_AmongUs/MultiPlayer_AmongUsCharacter.h"
#include "amongusPlayerState.h"
#include "Enum/playerEnum.h"
#include "amongus4PGameState.h"




// Sets default values
ABouton::ABouton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision; // Set it as the RootComponent
	SphereCollision->SetSphereRadius(50.0f); // Set a default radius
	SphereCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic")); // Set collision profile
    
}

// Called when the game starts or when spawned
void ABouton::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABouton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABouton::Interact(AActor* otherActor)
{
	AMultiPlayer_AmongUsCharacter* character = Cast<AMultiPlayer_AmongUsCharacter>(otherActor);
	if (character)
	{
		APlayerState* CurrentPS = character->GetPlayerState();
		if (CurrentPS)
		{
			AamongusPlayerState* PS = Cast<AamongusPlayerState>(CurrentPS);
			if (PS)
			{
				Aamongus4PGameState* AmGS = GetWorld()->GetGameState<Aamongus4PGameState>();
				if (AmGS)
				{
					if (PS->Etat == EEtatJoueur::Crew)
                    {
                    	AmGS->setNbTaskRemaning(AmGS->nbTaskRemaning - 1);
						//UE_LOG(LogTemp, Warning, TEXT("CREW UPDATE TASK"));
						UE_LOG(LogTemp, Warning, TEXT("TASK REMAINING : %d"), AmGS->nbTaskRemaning);
                    }
                    else if (PS->Etat == EEtatJoueur::Impostor)
                    {
                    	AmGS->setNbTaskRemaning(AmGS->nbTaskRemaning + 1);
                    	//UE_LOG(LogTemp, Warning, TEXT("IMPOSTER UPDATE TASK"));
                    	UE_LOG(LogTemp, Warning, TEXT("TASK REMAINING : %d"), AmGS->nbTaskRemaning);
                    }
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("AMGS FAIL"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("PS FAIL"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("CURRENT PS FAIL"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CHARACTER FAIL"));
	}
}

