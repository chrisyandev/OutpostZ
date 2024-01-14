#include "ZombieAIController.h"
#include "Zombie.h"
#include "Outpost.h"
#include "Kismet/GameplayStatics.h"
#include "AITypes.h"
#include "Navigation/PathFollowingComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

void AZombieAIController::BeginPlay()
{
	Super::BeginPlay();

	MoveToActor(TargetOutpost, AcceptanceRadius);
}

void AZombieAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AZombieAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (GetPawn() != nullptr)
	{
		ControlledZombie = Cast<AZombie>(GetPawn());
	}
	
	if (ControlledZombie != nullptr)
	{
		ControlledZombie->GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &AZombieAIController::HandleMontageEnded);
	}
	
	TargetOutpost = Cast<AOutpost>(UGameplayStatics::GetActorOfClass(GetWorld(), AOutpost::StaticClass()));
}

void AZombieAIController::OnMoveCompleted(const FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (ControlledZombie != nullptr && ControlledZombie->GetIsAlive())
	{
		if (AttackMontage)
		{

			ControlledZombie->PlayAnimMontage(AttackMontage);
		}
	}
	else
	{
		if (ControlledZombie != nullptr && DeathMontage)
		{

			ControlledZombie->PlayAnimMontage(DeathMontage);
		}
	}
}

void AZombieAIController::HandleMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == AttackMontage)
	{
		MoveToActor(TargetOutpost, AcceptanceRadius);
		OnAttackOutpost();
	}
}