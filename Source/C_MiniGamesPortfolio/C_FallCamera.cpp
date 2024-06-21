// Fill out your copyright notice in the Description page of Project Settings.

#include "C_FallCamera.h"
#include "C_PlayerController.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "C_MiniGamesPortfolioGameModeBase.h"
#include "C_FinishCamera.h"


void AC_FallCamera::SetCameraLocation(FVector v_PosRelativeTo)
{
	double x = v_PosRelativeTo.X;
	double y = v_PosRelativeTo.Y;
	double z = v_PosRelativeTo.Z + 1000.0;

	SetActorLocation(
		FVector(x, y, z)
	);
}

void AC_FallCamera::BeginPlay()
{
	Super::BeginPlay();

	SetActorRotation(
		FRotator(-90.0, 0.0, 0.0)
	);

	SetActorTickEnabled(false);
}

void AC_FallCamera::StartFadeToReloadLevel()
{
	if (AC_PlayerController* pc_Controller = Cast<AC_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		pc_Controller->ClientSetCameraFade_Implementation(true, FColor::Black, FVector2D(0.0, 1.0), 1.0f, false, true);
	}

	if (AC_MiniGamesPortfolioGameModeBase* MiniGameMode = Cast<AC_MiniGamesPortfolioGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		MiniGameMode->FadeStopBGM(1.0f);
	}

	FTimerHandle th_TimerHandle;
	GetWorldTimerManager().SetTimer(th_TimerHandle, this, &AC_FallCamera::ReloadLevel, 1.1f);
}

void AC_FallCamera::StartFadeLevelVictory()
{
	if (AC_PlayerController* pc_Controller = Cast<AC_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		pc_Controller->ClientSetCameraFade_Implementation(true, FColor::Silver, FVector2D(0.0, 1.0), 2.0f, false, true);
	}

	FTimerHandle th_TimerHandle;
	GetWorldTimerManager().SetTimer(th_TimerHandle, this, &AC_FallCamera::OpenResultsScreen, 2.5f);
}

void AC_FallCamera::StartFadeToRestartGame()
{
	if (AC_PlayerController* pc_Controller = Cast<AC_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		pc_Controller->ClientSetCameraFade_Implementation(true, FColor::Black, FVector2D(0.0, 1.0), 1.0f, true, true);
	}

	FTimerHandle th_TimerHandle;
	GetWorldTimerManager().SetTimer(th_TimerHandle, this, &AC_FallCamera::RestartGame, 1.1f);
}

void AC_FallCamera::StartFadeToQuit()
{
	if (AC_PlayerController* pc_Controller = Cast<AC_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		pc_Controller->ClientSetCameraFade_Implementation(true, FColor::Black, FVector2D(0.0, 1.0), 1.0f, true, true);
	}

	FTimerHandle th_TimerHandle;
	GetWorldTimerManager().SetTimer(th_TimerHandle, this, &AC_FallCamera::QuitTheGame, 1.1f);
}

void AC_FallCamera::ReloadLevel()
{
	if (AC_MiniGamesPortfolioGameModeBase* MiniGameMode = Cast<AC_MiniGamesPortfolioGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		MiniGameMode->LoadStreamedLevel();
	}
}

void AC_FallCamera::RestartGame()
{
	if (UC_GameInstance* gInstance = GetWorld()->GetGameInstanceChecked<UC_GameInstance>())
	{
		gInstance->ResetGameStateToMainMenu();

		UGameplayStatics::OpenLevel(GetWorld(), FName(UGameplayStatics::GetCurrentLevelName(GetWorld())));
	}
}

void AC_FallCamera::OpenResultsScreen()
{
	if (AC_MiniGamesPortfolioGameModeBase* MiniGameMode = Cast<AC_MiniGamesPortfolioGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		MiniGameMode->GameHUDOpenResults();
		MiniGameMode->SaveAsyncGameData();

		if (AC_PlayerController* pc_Controller = Cast<AC_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
		{
			if (AActor* cam_Finish = UGameplayStatics::GetActorOfClass(GetWorld(), AC_FinishCamera::StaticClass()))
			{
				pc_Controller->SetViewTarget(cam_Finish);
				pc_Controller->ClientSetCameraFade_Implementation(true, FColor::Silver, FVector2D(1.0, 0.0), 1.0f, false, true);
			}
		}
	}
}

void AC_FallCamera::QuitTheGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);
}
