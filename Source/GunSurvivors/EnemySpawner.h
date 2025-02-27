#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "GameFramework/Actor.h"
#include "GunSurvivorsGameMode.h"
#include "Engine/TimerHandle.h"
#include "TopDownCharacter.h"

#include "EnemySpawner.generated.h"

UCLASS()
class GUNSURVIVORS_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemy> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnTime = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnDistance = 400.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int Score = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int EnemyCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DifficultySpikeInterval = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnTimeMinimumLimit = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnTimerDecreaseInterval = 0.05f;

	FTimerHandle SpawnTimer;

	ATopDownCharacter* Player;

	AGunSurvivorsGameMode* GameMode;

	AEnemySpawner();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void SetUpGame();

	void StartSpawning();

	void StopSpawning();

	void OnSpawnTimerTimeout();

	void SpawnEnemy();

	void SetUpEnemy(AEnemy* Enemy);

	void CheckDifficulty();

	UFUNCTION()
	void OnEnemyDied();
};
