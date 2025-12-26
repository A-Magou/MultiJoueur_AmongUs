#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "widgetTypeEnum.generated.h"

UENUM()
enum class EWidgetType : uint8
{
	Global,
	Sabotage,
	SabotageRemain,
	Kill,
	DoTask,
	BeInteracted,
	Unset
};