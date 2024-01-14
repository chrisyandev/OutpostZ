#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Outpost.generated.h"

UCLASS()
class ZOMBIETOWERDEFENSE_API AOutpost : public AActor
{
	GENERATED_BODY()
	
public:	
	AOutpost();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};
