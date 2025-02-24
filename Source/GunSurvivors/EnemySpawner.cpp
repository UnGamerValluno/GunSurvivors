#include "EnemySpawner.h"
#include "Kismet/GameplayStatics.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	SetUpGame();
	StartSpawning();
}

void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemySpawner::SetUpGame()
{
	AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(GetWorld());
	if (GameMode)
	{
		GameMode = Cast<AGunSurvivorsGameMode>(GameModeBase);
		check(GameMode);
	}

	AActor* PlayerActor = UGameplayStatics::GetActorOfClass(GetWorld(), ATopDownCharacter::StaticClass());
	if (PlayerActor)
	{
		Player = Cast<ATopDownCharacter>(PlayerActor);
	}
}

void AEnemySpawner::StartSpawning()
{
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &AEnemySpawner::OnSpawnTimerTimeout, 1.f, true, SpawnTime);
}

void AEnemySpawner::StopSpawning()
{
	GetWorldTimerManager().ClearTimer(SpawnTimer);
}

void AEnemySpawner::OnSpawnTimerTimeout()
{
	SpawnEnemy();
	CheckDifficulty();
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

	AEnemy* Enemy = GetWorld()->SpawnActor<AEnemy>(EnemyClass, EnemyLocation, FRotator::ZeroRotator);

	SetUpEnemy(Enemy);
}

void AEnemySpawner::SetUpEnemy(AEnemy* Enemy)
{
	if (Enemy)
	{
		EnemyCount++;
		Enemy->Player = Player;
		Enemy->CanFollow = true;
		Enemy->EnemyDiedDelegate.AddDynamic(this, &AEnemySpawner::OnEnemyDied);
	}
}

void AEnemySpawner::CheckDifficulty()
{
	if(EnemyCount % DifficultySpikeInterval == 0 && SpawnTime >= SpawnTimeMinimumLimit)
	{
		SpawnTime = std::max(SpawnTime - SpawnTimerDecreaseInterval, SpawnTimeMinimumLimit);
		StopSpawning();
		StartSpawning();
	}
}

void AEnemySpawner::OnEnemyDied()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Another One Bites The Dust"));
}
