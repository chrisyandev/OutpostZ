#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TowerUpgradeMenu.generated.h"

UCLASS()
class ZOMBIETOWERDEFENSE_API UTowerUpgradeMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UButton* BTN_Upgrade;

	UPROPERTY(meta = (BindWidget))
	class UButton* BTN_Destroy;
};
