#include "Projectile.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	SetRootComponent(BoxComp);
	BoxComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // ignore all channels then manually enable
	BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->SetupAttachment(BoxComp);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComp->MaxSpeed = Speed;
	ProjectileMovementComp->InitialSpeed = Speed;
	ProjectileMovementComp->ProjectileGravityScale = 0.0f;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentHit.AddDynamic(this, &AProjectile::HandleHit);
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::Destroyed()
{
	Super::Destroyed();

	if (ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorTransform());
	}
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}
}

void AProjectile::HandleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, this, UDamageType::StaticClass());

	Destroy();
}

float AProjectile::GetSpeed() const
{
	return Speed;
}

void AProjectile::SetDamage(float NewDamage)
{
	Damage = NewDamage;
}