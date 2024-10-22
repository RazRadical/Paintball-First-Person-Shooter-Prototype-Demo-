// Copyright Epic Games, Inc. All Rights Reserved.

#include "PaintBallRazWillrichGameMode.h"
#include "PaintBallRazWillrichHUD.h"
#include "PaintBallRazWillrichCharacter.h"
#include "UObject/ConstructorHelpers.h"

APaintBallRazWillrichGameMode::APaintBallRazWillrichGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = APaintBallRazWillrichHUD::StaticClass();
}
