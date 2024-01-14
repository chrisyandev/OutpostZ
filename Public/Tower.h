#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tower.generated.h"

UCLASS()
class ZOMBIETOWERDEFENSE_API ATower : public AActor
{
	GENERATED_BODY()
	
public:	
	ATower();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorOnClicked(FKey ButtonPressed = EKeys::LeftMouseButton) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UWidgetComponent* TowerUpgradeMenuWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
	TArray<AActor*> AllZombies;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
	class AZombie* NearestZombie = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
	int UpgradeLevel = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
	float DistanceToNearestZombie = INFINITY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float ProjectileDamage = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Range = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float FireRate = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrades")
	TArray<UStaticMesh*> MeshUpgrades;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrades")
	TArray<float> DamageUpgrades;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrades")
	TArray<float> RangeUpgrades;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrades")
	TArray<float> FireRateUpgrades;

	FTimerHandle FireRateTimerHandle;

	void RotateTurret(FVector LookAtTarget);

	virtual void Fire();

	TArray<AActor*> GetAllZombies();

	class AZombie* GetNearestZombie(float& Distance);

	bool IsNearestZombieInRange();

	void CheckFireCondition();

	float TimeToIntercept(FVector DeltaPos, FVector DeltaVel, float ProjectileSpeed);

	bool CalculateAimLocation(FVector& AimLocation);

	UFUNCTION(BlueprintCallable)
	void Upgrade();

	UFUNCTION(BlueprintCallable)
	void DestroySelf();
};
