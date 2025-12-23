// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ServerRewind/RewindableComponent.h"
#include "amongusPlayerState.h"
#include "MultiPlayer_AmongUsCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class AMultiPlayer_AmongUsCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	URewindableComponent* RewindComponent;
	
protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* ChangeSkinAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* SetReadyAction;

	virtual void BeginPlay() override;
	

public:

	/** Constructor */
	AMultiPlayer_AmongUsCharacter();	

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

	UFUNCTION(Server, Unreliable)
	void ServerChangeSkin();

	UFUNCTION()
	void SetReady();
	
	UFUNCTION()
	void DoChangeSkin(UMaterialInterface* skin);

	UFUNCTION()
    void OnRep_SkinChanged();

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }



public:
	UPROPERTY()
	AamongusPlayerState* PS;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap <int32, UMaterialInterface*> Skins;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentSkinIndex = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_SkinChanged)
	UMaterialInterface* CurrentSkin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LineTraceDistance = 2000;

private:
	UFUNCTION(Server, Reliable)
	void Server_CheckLineTrace(float ServerTime, FVector TraceStart, /*FRotator TraceRot,*/ AMultiPlayer_AmongUsCharacter* TouchedActor, AMultiPlayer_AmongUsCharacter* FiredActor);

	UFUNCTION(Client, Reliable)
	void ConfirmHit();
	
public:
	UFUNCTION(BlueprintCallable, Category="LineTrace")
	void DoLineTrace();


	
	UFUNCTION()
	URewindableComponent* GetRewindComponent() {return this->RewindComponent;}

public:
	UFUNCTION()
	void UpdateSkinFromIndex(int NewSkinIndex);

	virtual void OnRep_PlayerState() override;
};

