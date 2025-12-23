#pragma once

#include "CoreMinimal.h"
#include "MultiPlayer_AmongUsCharacter.h"

#include "RewindableComponent.h"

#include "RewindSubsystem.generated.h"



USTRUCT()
struct FRewindData
{
	GENERATED_BODY()

	UPROPERTY()
	float ServerTime;
	/*UPROPERTY()
	TArray<AActor*> Players;*/
	UPROPERTY()
	TArray<URewindableComponent*> Players;	// player's rewind comp
	UPROPERTY()
	TArray<FVector> RewindCompLocations; // Has same index as character rewind comp
};



UCLASS()
class MULTIPLAYER_AMONGUS_API URewindSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

protected:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Tick(float DeltaTime) override;
	
	void PutEveryOneBack(TArray<FRewindData> TempReplaceHistory);
	
public:
	void EnregistrerComponent(URewindableComponent* RewindComp);

	UFUNCTION()
	bool StartRewind(float ServerTime, FVector TraceStart,
	/*FRotator TraceRot,*/ AMultiPlayer_AmongUsCharacter* TouchedActor, AMultiPlayer_AmongUsCharacter* FiredActor);

protected:
	UPROPERTY()
	UWorld* World;
	
public:
	UPROPERTY()
	TArray<URewindableComponent*> RewindComps;

	UPROPERTY()
	TArray<FRewindData> History;
	
};
