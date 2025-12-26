#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "playerEnum.generated.h"

UENUM()
enum class EEtatJoueur : uint8
{
	Crew,
	Impostor,
	Mort
};