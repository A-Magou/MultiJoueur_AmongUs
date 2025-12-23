// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiPlayer_AmongUsCharacter.h"

#include "amongusPlayerController.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Net/UnrealNetwork.h"
#include "MultiPlayer_AmongUs.h"
//#include "amongusPlayerState.h"
#include "ServerRewind/RewindSubsystem.h"

void AMultiPlayer_AmongUsCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Initialize Skin by reading playerstate's property
	if (HasAuthority())
	{
		PS = GetPlayerState<AamongusPlayerState>();
		if (PS)
		{
			CurrentSkinIndex = PS->SkinIndex;
			UpdateSkinFromIndex(CurrentSkinIndex);
		}
	}
}

AMultiPlayer_AmongUsCharacter::AMultiPlayer_AmongUsCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)


	RewindComponent = CreateDefaultSubobject<URewindableComponent>(TEXT("RewindComponent"));
}

void AMultiPlayer_AmongUsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMultiPlayer_AmongUsCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AMultiPlayer_AmongUsCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMultiPlayer_AmongUsCharacter::Look);

		// Change Skin
		EnhancedInputComponent->BindAction(ChangeSkinAction, ETriggerEvent::Started, this, &AMultiPlayer_AmongUsCharacter::ServerChangeSkin_Implementation);

		// Set Ready
		EnhancedInputComponent->BindAction(SetReadyAction, ETriggerEvent::Started, this, &AMultiPlayer_AmongUsCharacter::ServerChangeSkin_Implementation);
	}
	else
	{
		UE_LOG(LogMultiPlayer_AmongUs, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMultiPlayer_AmongUsCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AMultiPlayer_AmongUsCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AMultiPlayer_AmongUsCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AMultiPlayer_AmongUsCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AMultiPlayer_AmongUsCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AMultiPlayer_AmongUsCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void AMultiPlayer_AmongUsCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	PS = GetPlayerState<AamongusPlayerState>();
	if (PS)
	{
		UpdateSkinFromIndex(PS->SkinIndex);
	}
}

//CHANGE SKIN
void AMultiPlayer_AmongUsCharacter::UpdateSkinFromIndex(int NewSkinIndex)
{
	if (!Skins.Contains(NewSkinIndex)) NewSkinIndex = 0;

	CurrentSkinIndex = NewSkinIndex;
	
	if (Skins.Contains(CurrentSkinIndex))
	{
		GetMesh()->SetMaterial(0, Skins[CurrentSkinIndex]);
	}
}

void AMultiPlayer_AmongUsCharacter::ServerChangeSkin_Implementation()
{
	int32 NewIndex = CurrentSkinIndex + 1;
	if (NewIndex >= Skins.Num()) NewIndex = 0;
	
	CurrentSkinIndex = NewIndex;
	
	//AamongusPlayerState* PS = GetPlayerState<AamongusPlayerState>();
	if (PS) PS->SetSkin(CurrentSkinIndex);
	else { /*ERROR LOG*/ }
	
	UpdateSkinFromIndex(CurrentSkinIndex);
}



void AMultiPlayer_AmongUsCharacter::DoChangeSkin(UMaterialInterface* skin)
{
	UE_LOG(LogTemp, Warning, TEXT("changing skin!"));
	
	if (HasAuthority())
	{
		CurrentSkin = skin;
		OnRep_SkinChanged();

		if (CurrentSkinIndex >= Skins.Num() - 1)
		{
			CurrentSkinIndex = 0;
		}
		else
		{
			CurrentSkinIndex++;
		}
	}
	else
	{
		ServerChangeSkin();
	}
}

void AMultiPlayer_AmongUsCharacter::OnRep_SkinChanged()
{
	GetMesh()->SetMaterial(0, CurrentSkin);
}
// CHANGE SKIN END

void AMultiPlayer_AmongUsCharacter::SetReady()
{
	if (PS == nullptr) /*ERROR LOG*/ return;

	PS->SetReady();
}

void AMultiPlayer_AmongUsCharacter::DoLineTrace()
{
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * LineTraceDistance;
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel
	(
		Hit,
		Start,
		End,
		ECC_Visibility,
		Params
	);

	DrawDebugLine
	(
		GetWorld(),
		Start,
		End,
		bHit ? FColor::Red : FColor::Green,
		false,
		1,
		0,
		2
	);

	if (bHit)
	{
		AMultiPlayer_AmongUsCharacter* TouchedPlayer = Cast<AMultiPlayer_AmongUsCharacter>(Hit.GetActor());
		if (TouchedPlayer)
		{
			AamongusPlayerController* amPC = Cast<AamongusPlayerController>(this->GetController());
			if (amPC)
			{
				float ServerTime = amPC->GetServerWorldTime();
                FVector TraceStart = Start;
                /*FRotator TraceRot*/
                Server_CheckLineTrace(ServerTime, TraceStart, /*TraceRot,*/ TouchedPlayer, this);
			}

		}
	}
}




void AMultiPlayer_AmongUsCharacter::Server_CheckLineTrace_Implementation(float ServerTime, FVector TraceStart,
                                                                         /*FRotator TraceRot,*/ AMultiPlayer_AmongUsCharacter* TouchedActor, AMultiPlayer_AmongUsCharacter* FiredActor)
{
	if (!HasAuthority())
	{
		// YOU SHALL NOT PASS
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		URewindSubsystem* RewindSystem = World->GetSubsystem<URewindSubsystem>();
		if (RewindSystem)
		{
			if (RewindSystem->StartRewind(ServerTime, TraceStart, TouchedActor, FiredActor))
			{
				ConfirmHit();
			}
		}
		else
		{
			// problem
		}
	}
	else
	{
		// problem
	}
	
}

void AMultiPlayer_AmongUsCharacter::ConfirmHit_Implementation()
{
	// send a confirm to client
}

void AMultiPlayer_AmongUsCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AMultiPlayer_AmongUsCharacter, CurrentSkin);
}
