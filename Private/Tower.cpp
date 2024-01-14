#include "Tower.h"
#include "Zombie.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "Components/Button.h"
#include "TowerUpgradeMenu.h"

ATower::ATower()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	SetRootComponent(CapsuleComp);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

	TowerUpgradeMenuWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Tower Upgrade Menu"));
	TowerUpgradeMenuWidget->SetupAttachment(RootComponent);
}

void ATower::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);

	TowerUpgradeMenuWidget->SetVisibility(false);

	UTowerUpgradeMenu *Menu = Cast<UTowerUpgradeMenu>(TowerUpgradeMenuWidget->GetUserWidgetObject());
	Menu->BTN_Upgrade->OnClicked.AddDynamic(this, &ATower::Upgrade);
	Menu->BTN_Destroy->OnClicked.AddDynamic(this, &ATower::DestroySelf);
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AllZombies = GetAllZombies();
	NearestZombie = GetNearestZombie(DistanceToNearestZombie);

	FVector AimLocation;

	if (CalculateAimLocation(AimLocation))
	{
		RotateTurret(AimLocation);

		DrawDebugLine(GetWorld(), GetActorLocation(), NearestZombie->GetActorLocation(), FColor::Black, false, -1, 0, 1.0f);
		DrawDebugLine(GetWorld(), GetActorLocation(), AimLocation, FColor::Orange, false, -1, 0, 1.0f);
	}
}

void ATower::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);

	TowerUpgradeMenuWidget->SetVisibility(!TowerUpgradeMenuWidget->IsWidgetVisible());
}

void ATower::RotateTurret(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - GetActorLocation();
	TurretMesh->SetWorldRotation(ToTarget.Rotation());
}

void ATower::Fire()
{
	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = TurretMesh->GetComponentRotation();
	AProjectile *Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Location, Rotation);
	Projectile->SetDamage(ProjectileDamage);
}

TArray<AActor *> ATower::GetAllZombies()
{
	TArray<AActor *> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(this, AZombie::StaticClass(), FoundActors);

	TArray<AActor *> AliveZombies;

	for (AActor *Actor : FoundActors)
	{
		AZombie *Zombie = Cast<AZombie>(Actor);
		if (Zombie && Zombie->GetIsAlive())
		{
			AliveZombies.Add(Zombie);
		}
	}
	return AliveZombies;
}

AZombie *ATower::GetNearestZombie(float &DistanceToZombie)
{
	if (AllZombies.Num() > 0)
	{
		return Cast<AZombie>(UGameplayStatics::FindNearestActor(this->GetActorLocation(), AllZombies, DistanceToZombie));
	}
	return nullptr;
}

bool ATower::IsNearestZombieInRange()
{
	if (DistanceToNearestZombie <= Range)
	{
		return true;
	}
	return false;
}

void ATower::CheckFireCondition()
{
	if (NearestZombie && IsNearestZombieInRange())
	{
		Fire();
	}
}

float ATower::TimeToIntercept(FVector DeltaPos, FVector DeltaVel, float ProjectileSpeed)
{
	float a = FVector::DotProduct(DeltaVel, DeltaVel) - ProjectileSpeed * ProjectileSpeed;
	float b = 2.0f * FVector::DotProduct(DeltaVel, DeltaPos);
	float c = FVector::DotProduct(DeltaPos, DeltaPos);

	float discriminant = b * b - 4.0f * a * c;

	if (discriminant > 0.0f)
	{
		return 2.0f * c / (FMath::Sqrt(discriminant) - b);
	}
	return -1.0f;
}

bool ATower::CalculateAimLocation(FVector &AimLocation)
{
	if (!NearestZombie)
	{
		return false;
	}

	float deltaTime = TimeToIntercept(
		NearestZombie->GetActorLocation() - GetActorLocation(),
		NearestZombie->GetVelocity() - GetVelocity(),
		Cast<AProjectile>(ProjectileClass->GetDefaultObject())->GetSpeed());

	if (deltaTime > 0.0f)
	{
		AimLocation = NearestZombie->GetActorLocation() + NearestZombie->GetVelocity() * deltaTime;
		return true;
	}
	return false;
}

void ATower::Upgrade()
{
	if (MeshUpgrades.IsValidIndex(UpgradeLevel) && MeshUpgrades[UpgradeLevel])
	{
		TurretMesh->SetStaticMesh(MeshUpgrades[UpgradeLevel]);
	}
	if (DamageUpgrades.IsValidIndex(UpgradeLevel) && DamageUpgrades[UpgradeLevel] >= 0.0f)
	{
		ProjectileDamage = DamageUpgrades[UpgradeLevel];
	}
	if (RangeUpgrades.IsValidIndex(UpgradeLevel) && RangeUpgrades[UpgradeLevel] >= 0.0f)
	{
		Range = RangeUpgrades[UpgradeLevel];
	}
	if (FireRateUpgrades.IsValidIndex(UpgradeLevel) && FireRateUpgrades[UpgradeLevel] >= 0.0f)
	{
		FireRate = FireRateUpgrades[UpgradeLevel];
		GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
	}

	UpgradeLevel++;
}

void ATower::DestroySelf()
{
	Destroy();
}