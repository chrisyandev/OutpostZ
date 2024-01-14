#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Zombie.generated.h"

UCLASS()
class ZOMBIETOWERDEFENSE_API AZombie : public ACharacter
{
	GENERATED_BODY()

public:
	AZombie();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UHealthBarComponent* HealthBarComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Health = MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool IsAlive = true;

	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable)
	void UpdateHealthBar();

public:
	UFUNCTION()
	bool GetIsAlive()
	{
		return IsAlive;
	}

	UFUNCTION(BlueprintImplementableEvent)
	void OnDie();
};
