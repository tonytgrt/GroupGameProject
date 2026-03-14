// Umbra - Light & Shadow Puzzle Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UmbraInteractable.h"
#include "UmbraLantern.generated.h"

class UStaticMeshComponent;
class UPointLightComponent;
class USphereComponent;

/**
 *  Draggable point light source for Umbra puzzles.
 *  Player clicks and drags to reposition on the XY plane.
 */
UCLASS()
class AUmbraLantern : public AActor, public IUmbraInteractable
{
	GENERATED_BODY()

public:
	AUmbraLantern();

protected:
	// --- IUmbraInteractable ---
	virtual void OnDragStart_Implementation(FVector WorldPos) override;
	virtual void OnDragUpdate_Implementation(FVector WorldPos) override;
	virtual void OnDragEnd_Implementation() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CollisionSphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> LanternMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPointLightComponent> PointLight;

	/** Lock the lantern's Z position so it stays on the same plane */
	float LockedZ;
};
