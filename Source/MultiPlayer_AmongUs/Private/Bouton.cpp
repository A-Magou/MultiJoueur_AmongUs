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
	PrimaryActorTick.bCanEverTick = false;


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

void ABouton::OnRep_bIsUsing()
{
}


