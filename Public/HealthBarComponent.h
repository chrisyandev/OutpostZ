#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

UCLASS()
class ZOMBIETOWERDEFENSE_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetHealthPercent(float Percent);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UHealthBar* HealthBar;


private:
};
