#include "EnemySpawner.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	StartSpawning();
}

void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemySpawner::StartSpawning()
{
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &AEnemySpawner::SpawnEnemy, 1.f, true, SpawnTime);
}

void AEnemySpawner::StopSpawning()
{
	GetWorldTimerManager().ClearTimer(SpawnTimer);
}

void AEnemySpawner::SpawnEnemy()
{
	// FMath::VRand gives a random position in a sphere of radius 1. Passing it to the FVector2D automatically takes only X and Y
	FVector2D RandomPosition = FVector2D(FMath::VRand());

	// Since the random position is always of radius 1 or less, normalizing it pushes it to the edge of the circle
	RandomPosition.Normalize();

	// After normalization, the vector distance will be 1 so multiplying it by SpawnDistance pushes it to where we want the enemy to spawn
	RandomPosition *= SpawnDistance;

	// The game takes place in the X,Z plane so only those are added to the location vector
	FVector EnemyLocation = GetActorLocation() + FVector(RandomPosition.X, 0.f, RandomPosition.Y);

	GetWorld()->SpawnActor<AEnemy>(EnemyClass, EnemyLocation, FRotator::ZeroRotator);
}
