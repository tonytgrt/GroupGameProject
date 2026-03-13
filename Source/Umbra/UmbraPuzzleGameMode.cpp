// Umbra - Light & Shadow Puzzle Game

#include "UmbraPuzzleGameMode.h"
#include "UmbraPawn.h"
#include "UmbraPawnController.h"
#include "Umbra.h"

AUmbraPuzzleGameMode::AUmbraPuzzleGameMode()
{
	DefaultPawnClass = AUmbraPawn::StaticClass();
	PlayerControllerClass = AUmbraPawnController::StaticClass();
}

void AUmbraPuzzleGameMode::ResetLevel()
{
	// TODO: respawn pawn, reset all actors to initial state
	UE_LOG(LogUmbra, Log, TEXT("ResetLevel called"));
}