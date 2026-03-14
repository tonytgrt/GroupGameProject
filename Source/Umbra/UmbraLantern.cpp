// Umbra - Light & Shadow Puzzle Game

#include "UmbraLantern.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "UObject/ConstructorHelpers.h"

AUmbraLantern::AUmbraLantern()
{
	PrimaryActorTick.bCanEverTick = false;

	// Collision for mouse hit-testing
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->InitSphereRadius(40.f);
	CollisionSphere->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	CollisionSphere->SetSimulatePhysics(false);
	SetRootComponent(CollisionSphere);

	// Visual mesh
	LanternMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LanternMesh"));
	LanternMesh->SetupAttachment(CollisionSphere);
	LanternMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LanternMesh->SetCastShadow(false);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereMesh.Succeeded())
	{
		LanternMesh->SetStaticMesh(SphereMesh.Object);
		LanternMesh->SetRelativeScale3D(FVector(0.4f));
	}

	// Point light
	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLight->SetupAttachment(CollisionSphere);
	PointLight->SetIntensity(5000.f);
	PointLight->SetAttenuationRadius(1000.f);
	PointLight->SetCastShadows(true);

	LockedZ = 0.f;
}

void AUmbraLantern::OnDragStart_Implementation(FVector WorldPos)
{
	LockedZ = GetActorLocation().Z;
}

void AUmbraLantern::OnDragUpdate_Implementation(FVector WorldPos)
{
	FVector Desired(WorldPos.X, WorldPos.Y, LockedZ);
	SetActorLocation(Desired, true);  // sweep = true, stops at collision
}

void AUmbraLantern::OnDragEnd_Implementation()
{
	// Nothing needed for now
}
