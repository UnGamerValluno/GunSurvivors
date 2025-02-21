#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "GameFramework/Actor.h"
#include "Engine/TimerHandle.h"

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

	FTimerHandle SpawnTimer;

	AEnemySpawner();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void StartSpawning();

	void StopSpawning();

	void SpawnEnemy();
};
