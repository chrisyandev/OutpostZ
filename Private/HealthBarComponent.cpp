#include "HealthBarComponent.h"
#include "HealthBar.h"
#include "Components/ProgressBar.h"

void UHealthBarComponent::SetHealthPercent(float Percent)
{
	if (!HealthBar)
	{
		HealthBar = Cast<UHealthBar>(GetUserWidgetObject());
	}
	
	if (HealthBar && HealthBar->HealthProgressBar)
	{
		HealthBar->HealthProgressBar->SetPercent(Percent);
	}
}