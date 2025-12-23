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

UENUM()
enum class ESkinJoueur : uint8
{
	Normal,
	Red,
	Green
};