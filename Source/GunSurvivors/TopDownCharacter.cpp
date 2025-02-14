#include "TopDownCharacter.h"

ATopDownCharacter::ATopDownCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);

	CharacterFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("CharacterFlipbook"));
	CharacterFlipbook->SetupAttachment(RootComponent);
}

void ATopDownCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void ATopDownCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CanMove && MovementDirection.Length() > 0.f)
	{
		if (MovementDirection.Length() > 1.f)
		{
			MovementDirection.Normalize();
		}

		FVector2D DistanceToMove = MovementDirection * MovementSpeed * DeltaTime;

		// The 2D game is actually rendered in the X,Z plane; NOT the X,Y plane. This is why the Y value is assigned to Z
		FVector NewLocation = GetActorLocation() + FVector(DistanceToMove.X, 0.f, 0.f);
		if (!IsInMapBoundsHorizontal(NewLocation.X))
		{
			NewLocation -= FVector(DistanceToMove.X, 0.f, 0.f);
		}

		NewLocation += FVector(0.f, 0.f, DistanceToMove.Y);
		if (!IsInMapBoundsVertical(NewLocation.Z))
		{
			NewLocation -= FVector(0.f, 0.f, DistanceToMove.Y);
		}

		SetActorLocation(NewLocation);
	}
}

void ATopDownCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopDownCharacter::MoveTriggered);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ATopDownCharacter::MoveCompleted);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, this, &ATopDownCharacter::MoveCompleted);

		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ATopDownCharacter::Shoot);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ATopDownCharacter::Shoot);
	}
}

void ATopDownCharacter::MoveTriggered(const FInputActionValue& Value)
{
	FVector2D MoveActionValue = Value.Get<FVector2D>();
	if (CanMove)
	{
		MovementDirection = MoveActionValue;
		CharacterFlipbook->SetFlipbook(RunFlipbook);
		FVector FlipbookScale = CharacterFlipbook->GetComponentScale();
		float DirectionScale = (MovementDirection.X < 0.0f) ? -1.f : 1.f;

		if (MovementDirection.X < 0.0f && FlipbookScale.X > 0.0f || MovementDirection.X > 0.0f && FlipbookScale.X < 0.0f)
		{
			CharacterFlipbook->SetWorldScale3D(FVector(DirectionScale, 1.0f, 1.0f));
		}
	}
}

void ATopDownCharacter::MoveCompleted(const FInputActionValue& Value)
{
	MovementDirection = FVector2D(0.f, 0.f);
	CharacterFlipbook->SetFlipbook(IdleFlipbook);
}

void ATopDownCharacter::Shoot(const FInputActionValue& Value)
{
}


bool ATopDownCharacter::IsInMapBoundsHorizontal(float XPosition)
{
	return (XPosition > HorizontalLimits.X) && (XPosition < HorizontalLimits.Y);
}

bool ATopDownCharacter::IsInMapBoundsVertical(float ZPosition)
{
	return (ZPosition > VerticalLimits.X) && (ZPosition < VerticalLimits.Y);
}
