// Umbra - Light & Shadow Puzzle Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "UmbraPawn.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UFloatingPawnMovement;

UCLASS()
class AUmbraPawn : public APawn
{
	GENERATED_BODY()

public:
	AUmbraPawn();

	virtual void Tick(float DeltaSeconds) override;

	/** Called by controller each tick with WASD input */
	void SetMoveInput(FVector2D Input);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CollisionSphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> PawnMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> TopDownCamera;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFloatingPawnMovement> FloatingMovement;

	/** How far down to trace for ground */
	UPROPERTY(EditAnywhere, Category = "Movement")
	float GroundTraceDepth = 200.f;

	/** Cached input from controller */
	FVector2D CurrentMoveInput;

	/** Trace downward to snap to floor; returns true if floor found */
	bool SnapToGround();

	/** Check if there is floor at the given world position */
	bool HasFloorAt(FVector Location) const;

	// --- Shadow check (Phase 1) ---
	void PerformShadowCheck();

	/** Is the pawn currently over a void volume? */
	bool bIsOverVoid = false;
};
