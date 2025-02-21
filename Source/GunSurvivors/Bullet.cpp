#include "Bullet.h"
#include "Enemy.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);

	BulletSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BulletSprite"));
	BulletSprite->SetupAttachment(RootComponent);
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OverlapBegin);
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(DeltaTime);
}

void ABullet::Move(float DeltaTime)
{
	if (IsLaunched)
	{
		// The 2D game is actually rendered in the X,Z plane; NOT the X,Y plane. This is why the Y value is assigned to Z
		FVector2D DistanceToMove = MovementDirection * MovementSpeed * DeltaTime;
		FVector NewLocation = GetActorLocation() + FVector(DistanceToMove.X, 0.f, DistanceToMove.Y);
		SetActorLocation(NewLocation);
	}
}

void ABullet::Launch(FVector2D Direction, float Speed)
{
	if (!IsLaunched)
	{
		IsLaunched = true;
		MovementDirection = Direction;
		MovementSpeed = Speed;

		// Destroy the bullet after 10 seconds of its launch
		GetWorldTimerManager().SetTimer(DeleteTimer, this, &ABullet::OnDeleteTimerTimeout, 1.f, false, 10.f);
	}
}

void ABullet::OnDeleteTimerTimeout()
{
	Destroy();
}

void ABullet::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy && Enemy->IsAlive)
	{
		Disable();
		Enemy->Die();
	}
}

void ABullet::Disable()
{
	if (!IsDisabled)
	{
		IsDisabled = true;
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		BulletSprite->DestroyComponent();
	}
}
