// Umbra - Light & Shadow Puzzle Game

#include "UmbraPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"

AUmbraPawn::AUmbraPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// Configure the capsule (inherited from ACharacter)
	GetCapsuleComponent()->InitCapsuleSize(30.f, 30.f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	// Visible mesh — attach to root capsule
	PawnMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PawnMesh"));
	PawnMesh->SetupAttachment(GetRootComponent());
	PawnMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PawnMesh->SetCastShadow(false);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereMesh.Succeeded())
	{
		PawnMesh->SetStaticMesh(SphereMesh.Object);
		PawnMesh->SetRelativeScale3D(FVector(0.6f));
	}

	// Angled camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	CameraBoom->TargetArmLength = 1500.f;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;

	TopDownCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Configure CharacterMovement (inherited from ACharacter)
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	MoveComp->MaxWalkSpeed = 400.f;
	MoveComp->MaxAcceleration = 2000.f;
	MoveComp->BrakingDecelerationWalking = 4000.f;
	MoveComp->bCanWalkOffLedges = false;
	MoveComp->bCanWalkOffLedgesWhenCrouching = false;
	MoveComp->SetWalkableFloorAngle(50.f);
	MoveComp->GravityScale = 1.f;
	MoveComp->PerchRadiusThreshold = 30.f;
	MoveComp->PerchAdditionalHeight = 30.f;
	MoveComp->LedgeCheckThreshold = 10.f;

	// Don't rotate the pawn to face movement direction
	bUseControllerRotationYaw = false;
	MoveComp->bOrientRotationToMovement = false;
}

void AUmbraPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!CurrentMoveInput.IsNearlyZero())
	{
		FVector Direction(CurrentMoveInput.X, CurrentMoveInput.Y, 0.f);
		Direction = Direction.GetClampedToMaxSize(1.f);
		AddMovementInput(Direction);
	}
}

void AUmbraPawn::SetMoveInput(FVector2D Input)
{
	CurrentMoveInput = Input;
}

void AUmbraPawn::PerformShadowCheck()
{
	// TODO: Phase 1 — raycast to each light, check if pawn is in shadow over void
}
