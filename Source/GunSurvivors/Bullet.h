#pragma once

#include "Components/SphereComponent.h"
#include "CoreMinimal.h"
#include "Engine/TimerHandle.h"
#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"

#include "Bullet.generated.h"

UCLASS()
class GUNSURVIVORS_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent* BulletSprite;

	UPROPERTY(BlueprintReadWrite)
	FVector2D MovementDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsLaunched = false;

	FTimerHandle DeleteTimer;

	ABullet();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void Move(float DeltaTime);

	void Launch(FVector2D Direction, float Speed);

	void OnDeleteTimerTimeout();
};
