#include "Zombie.h"
#include "Outpost.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HealthBarComponent.h"
#include "GameFramework/PawnMovementComponent.h"

AZombie::AZombie()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	HealthBarComp = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Health Bar"));
	HealthBarComp->SetupAttachment(RootComponent);
}

void AZombie::BeginPlay()
{
	Super::BeginPlay();

	if (HealthBarComp)
	{
		HealthBarComp->SetHealthPercent(Health / MaxHealth);
	}

	OnTakeAnyDamage.AddDynamic(this, &AZombie::HandleTakeAnyDamage);
}

void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AZombie::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser)
{
	if (!IsAlive) { return; }

	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);

	if (HealthBarComp)
	{
		HealthBarComp->SetHealthPercent(Health / MaxHealth);
	}

	if (Health == 0.0f)
	{
		IsAlive = false;
		OnDie();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMovementComponent()->StopMovementImmediately();
	}
}

void AZombie::UpdateHealthBar()
{
	if (HealthBarComp)
	{
		HealthBarComp->SetHealthPercent(Health / MaxHealth);
	}
}