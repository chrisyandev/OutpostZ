#include "Outpost.h"

AOutpost::AOutpost()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AOutpost::BeginPlay()
{
	Super::BeginPlay();
}

void AOutpost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

