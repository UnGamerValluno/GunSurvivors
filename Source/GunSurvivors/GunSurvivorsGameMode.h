#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "GunSurvivorsGameMode.generated.h"

UCLASS()
class GUNSURVIVORS_API AGunSurvivorsGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int Score = 0;

	AGunSurvivorsGameMode();

	virtual void BeginPlay() override;

	void SetScore(int NewScore);

	void AddScore(int AmountToAdd);
};
