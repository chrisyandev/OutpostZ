#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZombieAIController.generated.h"

UCLASS()
class ZOMBIETOWERDEFENSE_API AZombieAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void OnMoveCompleted(const FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnAttackOutpost();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AcceptanceRadius = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UAnimMontage* DeathMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
	class AZombie* ControlledZombie;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Debug")
	class AOutpost* TargetOutpost;

	UFUNCTION()
	virtual void HandleMontageEnded(class UAnimMontage* Montage, bool bInterrupted);
};
