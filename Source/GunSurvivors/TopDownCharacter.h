#pragma once

#include "Bullet.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SceneComponent.h"
#include "CoreMinimal.h"
#include "Engine/TimerHandle.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "Sound/SoundBase.h"

#include "TopDownCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerDiedDelegate);

UCLASS()
class GUNSURVIVORS_API ATopDownCharacter : public APawn
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* CharacterFlipbook;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* GunParent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent* GunSprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* BulletSpawnPosition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperFlipbook* IdleFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperFlipbook* RunFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D HorizontalLimits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D VerticalLimits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* ShootSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* DeathSound;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABullet> BulletActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed = 100.0f;

	UPROPERTY(BlueprintReadWrite)
	FVector2D MovementDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsAlive = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CanMove = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CanShoot = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShootCooldownInSeconds = 0.3f;

	FTimerHandle ShootCooldownTimer;

	FPlayerDiedDelegate PlayerDiedDelegate;

	ATopDownCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(float DeltaTime);

	void RotateGun();

	void MoveTriggered(const FInputActionValue& Value);

	void MoveCompleted(const FInputActionValue& Value);

	void Shoot(const FInputActionValue& Value);

	bool IsInMapBoundsHorizontal(float XPosition);

	bool IsInMapBoundsVertical(float ZPosition);

	void OnShootCooldownTimerTimeout();

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
