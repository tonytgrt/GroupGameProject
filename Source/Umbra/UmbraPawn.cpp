// Umbra - Light & Shadow Puzzle Game

#include "UmbraPawn.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "UObject/ConstructorHelpers.h"

AUmbraPawn::AUmbraPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// Collision root
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->InitSphereRadius(30.f);
	CollisionSphere->SetCollisionProfileName(TEXT("Pawn"));
	SetRootComponent(CollisionSphere);

	// Visible mesh
	PawnMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PawnMesh"));
	PawnMesh->SetupAttachment(CollisionSphere);
	PawnMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PawnMesh->SetCastShadow(false);

	// Angled camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(CollisionSphere);
	CameraBoom->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	CameraBoom->TargetArmLength = 1500.f;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;

	TopDownCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Load default sphere mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereMesh.Succeeded())
	{
		PawnMesh->SetStaticMesh(SphereMesh.Object);
		PawnMesh->SetRelativeScale3D(FVector(0.6f));
	}

	// Floating movement — handles sliding along surfaces automatically
	FloatingMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingMovement"));
	FloatingMovement->MaxSpeed = 400.f;
	FloatingMovement->Acceleration = 2000.f;
	FloatingMovement->Deceleration = 4000.f;
}

void AUmbraPawn::BeginPlay()
{
	Super::BeginPlay();
}

void AUmbraPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Feed input to FloatingPawnMovement — it handles sweep + sliding
	if (!CurrentMoveInput.IsNearlyZero())
	{
		FVector Direction(CurrentMoveInput.X, CurrentMoveInput.Y, 0.f);
		Direction = Direction.GetClampedToMaxSize(1.f);

		// Check if the full movement direction has floor
		FVector Location = GetActorLocation();
		float CheckDist = CollisionSphere->GetScaledSphereRadius() + 5.f;

		if (HasFloorAt(Location + Direction * CheckDist))
		{
			AddMovementInput(Direction);
		}
		else
		{
			// Full direction blocked — try each axis independently to allow edge sliding
			FVector Allowed = FVector::ZeroVector;

			if (FMath::Abs(Direction.X) > KINDA_SMALL_NUMBER &&
				HasFloorAt(Location + FVector(Direction.X, 0.f, 0.f) * CheckDist))
			{
				Allowed.X = Direction.X;
			}
			if (FMath::Abs(Direction.Y) > KINDA_SMALL_NUMBER &&
				HasFloorAt(Location + FVector(0.f, Direction.Y, 0.f) * CheckDist))
			{
				Allowed.Y = Direction.Y;
			}

			if (!Allowed.IsNearlyZero())
			{
				AddMovementInput(Allowed);
			}
			else
			{
				FloatingMovement->StopMovementImmediately();
			}
		}
	}

	// Snap to ground each frame (handles slopes)
	SnapToGround();
}

void AUmbraPawn::SetMoveInput(FVector2D Input)
{
	CurrentMoveInput = Input;
}

bool AUmbraPawn::HasFloorAt(FVector Location) const
{
	FVector TraceStart = FVector(Location.X, Location.Y, Location.Z + 10.f);
	FVector TraceEnd = TraceStart - FVector(0.f, 0.f, GroundTraceDepth);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	return GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, Params);
}

bool AUmbraPawn::SnapToGround()
{
	FVector Location = GetActorLocation();
	FVector TraceStart = FVector(Location.X, Location.Y, Location.Z + 10.f);
	FVector TraceEnd = TraceStart - FVector(0.f, 0.f, GroundTraceDepth);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, Params))
	{
		float TargetZ = Hit.Location.Z + CollisionSphere->GetScaledSphereRadius();
		SetActorLocation(FVector(Location.X, Location.Y, TargetZ));
		return true;
	}
	return false;
}

void AUmbraPawn::PerformShadowCheck()
{
	// TODO: Phase 1 — raycast to each light, check if pawn is in shadow over void
}
