#include "Enemy.h"
#include "Kismet/GameplayStatics.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);

	EnemyFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("EnemyFlipbook"));
	EnemyFlipbook->SetupAttachment(RootComponent);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (!Player)
	{
		AActor* PlayerActor = UGameplayStatics::GetActorOfClass(GetWorld(), ATopDownCharacter::StaticClass());
		if (PlayerActor)
		{
			Player = Cast<ATopDownCharacter>(PlayerActor);
			CanFollow = true;
		}
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(DeltaTime);
	FacePlayer();
}

void AEnemy::Move(float DeltaTime)
{
	if (IsAlive && CanFollow && Player)
	{
		FVector CurrentLocation = GetActorLocation();
		FVector DirectionToPlayer = Player->GetActorLocation() - CurrentLocation;

		if (DirectionToPlayer.Length() > StopDistance)
		{
			DirectionToPlayer.Normalize();
			FVector NewLocation = CurrentLocation + (DirectionToPlayer * MovementSpeed * DeltaTime);
			SetActorLocation(NewLocation);
		}
	}
}

void AEnemy::FacePlayer()
{
	float FlipbookScale = EnemyFlipbook->GetComponentScale().X;
	float DirectionScale = (Player->GetActorLocation().X - GetActorLocation().X >= 0.f) ? 1.f : -1.f;
	
	if (DirectionScale != FlipbookScale)
	{
		EnemyFlipbook->SetWorldScale3D(FVector(DirectionScale, 1.0f, 1.0f));
	}
}

void AEnemy::Die()
{
	if (IsAlive)
	{
		IsAlive = false;
		CanFollow = false;
		EnemyFlipbook->SetFlipbook(DeadFlipbook);
		EnemyFlipbook->SetTranslucentSortPriority(4);

		// Destroy the enemy after 10 seconds of its death
		GetWorldTimerManager().SetTimer(DestroyTimer, this, &AEnemy::OnDestroyTimerTimeout, 1.f, false, 10.f);
	}
}

void AEnemy::OnDestroyTimerTimeout()
{
	Destroy();
}
