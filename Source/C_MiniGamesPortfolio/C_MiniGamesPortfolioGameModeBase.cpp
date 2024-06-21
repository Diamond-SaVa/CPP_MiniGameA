// Copyright Epic Games, Inc. All Rights Reserved.

#include "C_MiniGamesPortfolioGameModeBase.h"
#include "C_PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "C_CollectableBase.h"


AC_MiniGamesPortfolioGameModeBase::AC_MiniGamesPortfolioGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;

	audioC_BGM = CreateDefaultSubobject<UAudioComponent>(TEXT("BGM_Player"));

	if (audioC_BGM)
	{
		audioC_BGM->SetAutoActivate(false);
	}
}

void AC_MiniGamesPortfolioGameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (audioC_BGM != nullptr && soundC_BGM_Level != nullptr)
	{
		audioC_BGM->SetSound(soundC_BGM_Level);
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("WARNING. COMPONENTS MISSING."));

			if (audioC_BGM == nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("MISSING BGM PLAYER."));
			}

			if (soundC_BGM_Level == nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("MISSING BGM SONG."));
			}
		}
	}
}

void AC_MiniGamesPortfolioGameModeBase::BeginPlay()
{
	gInstance = GetWorld()->GetGameInstanceChecked<UC_GameInstance>();

	if (uWidget_BallHUDClass != nullptr)
	{
		BallGameHUD = Cast<UC_BallGameHUD>(CreateWidget<UUserWidget>(GetWorld()->GetGameInstance(), uWidget_BallHUDClass));

		if (BallGameHUD != nullptr)
		{
			BallGameHUD->AddToPlayerScreen(0);
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("WARNING. WIDGET CLASS IS MISSING."));
		}
	}

	StartingProcedures();
}

void AC_MiniGamesPortfolioGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (BallGameHUD != nullptr)
	{
		BallGameHUD->RemoveFromParent();

		BallGameHUD = nullptr;
	}

	if (audioC_BGM != nullptr)
	{
		audioC_BGM->Stop();
	}
}

void AC_MiniGamesPortfolioGameModeBase::Tick(float DeltaTime)
{
	if (*GetTimerActiveValue() == true)
	{
		if (BallGameHUD != nullptr)
		{
			f_LevelTimer += DeltaTime;
			const FTimespan ft_timeSpan = UKismetMathLibrary::FromSeconds(f_LevelTimer);

			const int32 unsi_Miliseconds = ft_timeSpan.GetFractionMilli();
			const int32 unsi_Seconds = ft_timeSpan.GetSeconds();
			const int32 unsi_Minutes = ft_timeSpan.GetMinutes();

			const FString timerText = FString::Printf(TEXT("%02i:%02i:%03i"), unsi_Minutes, unsi_Seconds, unsi_Miliseconds);
			BallGameHUD->UpdateTextTimer(*timerText);
		}
	}
}

void AC_MiniGamesPortfolioGameModeBase::OpenGameMenu()
{
	if (audioC_BGM != nullptr && soundC_BGM_Level != nullptr)
	{
		audioC_BGM->SetSound(soundC_BGM_Level);

		audioC_BGM->SetAutoActivate(false);

		audioC_BGM->Play();
	}

	CloseLoadingDataCanvasAndOpenMainMenu();
}

void AC_MiniGamesPortfolioGameModeBase::CloseSaveLoadDataCanvas()
{
	if (BallGameHUD != nullptr)
	{
		BallGameHUD->SetSaveLoadDataCanvasVisibility(ESlateVisibility::Collapsed);

		if (gInstance != nullptr)
		{
			if (gInstance->IsGameInMainMenu() == true)
			{
				BallGameHUD->SetMainMenuVisibility(ESlateVisibility::Visible);
				BallGameHUD->SetEnumCurrentMenu(BallGameHUD->MAIN_MENU);
			}
		}
	}
}

void AC_MiniGamesPortfolioGameModeBase::CloseLoadingDataCanvasAndOpenMainMenu()
{
	if (BallGameHUD != nullptr)
	{
		BallGameHUD->SetSaveLoadDataCanvasVisibility(ESlateVisibility::Collapsed);

		BallGameHUD->SetMainMenuVisibility(ESlateVisibility::Visible);

		BallGameHUD->SetEnumCurrentMenu(BallGameHUD->MAIN_MENU);
	}
}

void AC_MiniGamesPortfolioGameModeBase::OnSaveBehavior(const FString& fs_SaveFileName, const int32 i32_SaveFileSlot, bool b_success)
{
	if (BallGameHUD != nullptr)
	{
		BallGameHUD->SetSaveLoadIconVisibility(ESlateVisibility::Collapsed);
	}

	if (b_success == true)
	{
		if (BallGameHUD != nullptr)
		{
			BallGameHUD->SetSaveLoadStatusText(BallGameHUD->cfs_SavingSuccess);

			FTimerHandle th_TimerHandle;
			GetWorldTimerManager().SetTimer(th_TimerHandle, this, &AC_MiniGamesPortfolioGameModeBase::CloseSaveLoadDataCanvas, 2.0f);
		}

		if (usg_saveData != nullptr)
		{
			if (gInstance != nullptr)
			{
				gInstance->SetNewRecordScore(usg_saveData->GetRecordScore());

				gInstance->SetNewRecordTime(usg_saveData->GetRecordTime());

				gInstance->SetBGMVolume(usg_saveData->GetBGMVolume());

				gInstance->SetSFXVolume(usg_saveData->GetSFXVolume());

				gInstance->SetCamSpeedValue(usg_saveData->GetCamSpeedValue());

				gInstance->GameWasLoaded();
			}
		}
	}
	else
	{
		if (gInstance != nullptr)
		{
			gInstance->GameWasLoaded();
		}

		if (BallGameHUD != nullptr)
		{
			BallGameHUD->SetSaveLoadStatusText(BallGameHUD->cfs_SavingFailed);

			FTimerHandle th_TimerHandle;
			GetWorldTimerManager().SetTimer(th_TimerHandle, this, &AC_MiniGamesPortfolioGameModeBase::CloseSaveLoadDataCanvas, 2.0f);
		}
	}
}

void AC_MiniGamesPortfolioGameModeBase::OnLoadBehavior(const FString& fs_SaveFileName, const int32 i32_SaveFileSlot, USaveGame* baseSaveGame)
{
	if (baseSaveGame != nullptr)
	{
		usg_saveData = Cast<UC_SaveData>(baseSaveGame);

		f_LevelTimerRecord = usg_saveData->GetRecordTime();

		unsi_ScoreRecord = usg_saveData->GetRecordScore();

		f_BGMVolume = usg_saveData->GetBGMVolume();

		f_SFXVolume = usg_saveData->GetSFXVolume();

		f_camSpeed = usg_saveData->GetCamSpeedValue();

		if (gInstance != nullptr)
		{
			gInstance->SetBGMVolume(f_BGMVolume);

			gInstance->SetSFXVolume(f_SFXVolume);

			gInstance->SetNewRecordScore(unsi_ScoreRecord);

			gInstance->SetNewRecordTime(f_LevelTimerRecord);

			gInstance->SetCamSpeedValue(f_camSpeed);

			gInstance->GameWasLoaded();
		}

		if (GetBGMPlayer() != nullptr)
		{
			GetBGMPlayer()->SetVolumeMultiplier(f_BGMVolume);
		}

		if (BallGameHUD != nullptr)
		{
			BallGameHUD->SetSliderLoadedValues(f_BGMVolume, f_SFXVolume, f_camSpeed);

			BallGameHUD->SetSaveLoadStatusText(BallGameHUD->cfs_LoadingSuccessText);

			FTimerHandle th_TimerHandle;
			GetWorldTimerManager().SetTimer(th_TimerHandle, this, &AC_MiniGamesPortfolioGameModeBase::OpenGameMenu, 2.0f);
		}
	}
	else
	{
		CreateSaveData();
	}
}

void AC_MiniGamesPortfolioGameModeBase::OnLevelWasLoaded()
{
	if (BallGameHUD != nullptr)
	{
		BallGameHUD->SetLoadingSceneCanvasVisibility(ESlateVisibility::Collapsed);
	}

	StartingProcedures();

	SetTimerActive(true);
}

void AC_MiniGamesPortfolioGameModeBase::OnLevelWasUnloaded()
{
	if (gInstance != nullptr)
	{
		gInstance->LevelWasReloaded();
	}

	LoadStreamedLevel();
}

void AC_MiniGamesPortfolioGameModeBase::StartingProcedures()
{
	if (gInstance != nullptr)
	{
		if (gInstance->WasGameLoaded() == false)
		{
			if (BallGameHUD != nullptr)
			{
				BallGameHUD->SetSaveLoadStatusText(BallGameHUD->cfs_SavingDataText);

				BallGameHUD->SetSaveLoadDataCanvasVisibility(ESlateVisibility::Visible);

				LoadAsyncGameData();
			}
		}
		else
		{
			unsi_ScoreRecord = gInstance->GetRecordScore();

			f_LevelTimerRecord = gInstance->GetRecordTime();

			SetUpMaxScoreAndRecords();

			if (gInstance->WasLevelReloaded())
			{
				ChangeBGM(EMusicToPlay::LEVEL_SONG, 0.2f);
			}
		}
	}
}

void AC_MiniGamesPortfolioGameModeBase::LoadStreamedLevel()
{
	if (gInstance != nullptr)
	{
		gInstance->ExitFromMainMenu();
	}

	if (BallGameHUD != nullptr)
	{
		BallGameHUD->SetLoadingSceneCanvasVisibility(ESlateVisibility::Visible);
	}

	levelStream = UGameplayStatics::GetStreamingLevel(GetWorld(), fName_LevelToStream);

	if (levelStream != nullptr)
	{
		if (levelStream->IsLevelLoaded() == false)
		{
			levelStream->OnLevelLoaded.Clear();
			levelStream->OnLevelLoaded.AddDynamic(this, &AC_MiniGamesPortfolioGameModeBase::OnLevelWasLoaded);

			FLatentActionInfo Latent;
			Latent.CallbackTarget = this;
			Latent.UUID = 1;
			UGameplayStatics::LoadStreamLevel(this, fName_LevelToStream, true, false, Latent);
		}
		else
		{
			levelStream->OnLevelUnloaded.Clear();
			levelStream->OnLevelUnloaded.AddDynamic(this, &AC_MiniGamesPortfolioGameModeBase::OnLevelWasUnloaded);

			FLatentActionInfo Latent;
			Latent.CallbackTarget = this;
			Latent.UUID = 1;
			UGameplayStatics::UnloadStreamLevel(this, fName_LevelToStream, Latent, false);
		}
	}
}

void AC_MiniGamesPortfolioGameModeBase::UpScore() 
{ 
	unsi_Score = UKismetMathLibrary::Clamp(unsi_Score + 1, 0, unsi_ScoreMax);

	unsi_ScoreRecord = UKismetMathLibrary::Clamp(unsi_Score, unsi_ScoreRecord, unsi_ScoreMax);

	UpdateScoreText();
}

void AC_MiniGamesPortfolioGameModeBase::UpdateScoreText()
{
	if (BallGameHUD != nullptr)
	{
		FString scoreText = FString::Printf(TEXT("%02i/%02i ( RECORD: %02i )"), unsi_Score, unsi_ScoreMax, unsi_ScoreRecord);

		BallGameHUD->UpdateTextScore(*scoreText);
	}
}

void AC_MiniGamesPortfolioGameModeBase::UpdateTimeRecordText()
{
	if (BallGameHUD != nullptr)
	{
		if (gInstance != nullptr)
		{
			const FTimespan ft_timeSpan = UKismetMathLibrary::FromSeconds(f_LevelTimerRecord);

			const int32 unsi_Miliseconds = ft_timeSpan.GetFractionMilli();
			const int32 unsi_Seconds = ft_timeSpan.GetSeconds();
			const int32 unsi_Minutes = ft_timeSpan.GetMinutes();

			const FString timerText = FString::Printf(TEXT("%02i:%02i:%03i"), unsi_Minutes, unsi_Seconds, unsi_Miliseconds);

			BallGameHUD->UpdateTextTimerRecord(*timerText);
		}
	}
}

void AC_MiniGamesPortfolioGameModeBase::SetUpMaxScoreAndRecords()
{
	TArray<AActor*> FoundCollectables;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AC_CollectableBase::StaticClass(), FoundCollectables);

	unsi_ScoreMax = FoundCollectables.Num();

	unsi_Score = 0;

	f_LevelTimer = 0.0f;

	UpdateScoreText();
	UpdateTimeRecordText();

	if (BallGameHUD != nullptr)
	{
		BallGameHUD->SetSatusCanvasVisibility(ESlateVisibility::Visible);

		BallGameHUD->StopAllAnimations();
	}
}

void AC_MiniGamesPortfolioGameModeBase::StopAndUpdateRecords()
{
	SetTimerActive(false);

	if (f_LevelTimer < f_LevelTimerRecord)
	{
		f_LevelTimerRecord = f_LevelTimer;

		UpdateTimeRecordText();

		if (BallGameHUD != nullptr)
		{
			BallGameHUD->FlashRecords();
		}
	}

	UpdateScoreText();
}

void AC_MiniGamesPortfolioGameModeBase::PlaySoundEffect(const ESoundEffectToPlay& EnumSoundEffect)
{
	switch (EnumSoundEffect)
	{
	case ESoundEffectToPlay::COIN_COLLECT:
		UGameplayStatics::PlaySound2D(GetWorld(), soundC_CoinCollect, f_SFXVolume);
		break;

	case ESoundEffectToPlay::COIN_BURST:
		UGameplayStatics::PlaySound2D(GetWorld(), soundC_CoinBurst, f_SFXVolume);
		break;

	case ESoundEffectToPlay::BALL_BOUNCE_GROUND:
		UGameplayStatics::PlaySound2D(GetWorld(), soundC_BallBounceOffGround, f_SFXVolume);
		break;

	case ESoundEffectToPlay::BALL_BOUNCE_WALL:
		UGameplayStatics::PlaySound2D(GetWorld(), soundC_BallBounceOffWall, f_SFXVolume);
		break;

	case ESoundEffectToPlay::BALL_FALLING:
		UGameplayStatics::PlaySound2D(GetWorld(), soundC_BallFalling, f_SFXVolume);
		break;

	case ESoundEffectToPlay::BALL_FLATTENING:
		UGameplayStatics::PlaySound2D(GetWorld(), soundC_BallFlatten, f_SFXVolume);
		break;

	case ESoundEffectToPlay::BALL_BURST:
		UGameplayStatics::PlaySound2D(GetWorld(), soundC_BallBurst, f_SFXVolume);
		break;

	case ESoundEffectToPlay::RISER_WIND:
		UGameplayStatics::PlaySound2D(GetWorld(), soundC_RiserWind, f_SFXVolume);
		break;

	case ESoundEffectToPlay::GOAL_REACH:
		UGameplayStatics::PlaySound2D(GetWorld(), soundC_GoalReach, f_SFXVolume);
		break;

	case ESoundEffectToPlay::CONFIRM_SOUND:
		UGameplayStatics::PlaySound2D(GetWorld(), soundC_CoinCollect, f_SFXVolume);
		break;

	case ESoundEffectToPlay::CANCEL_SOUND:
		UGameplayStatics::PlaySound2D(GetWorld(), soundC_CoinBurst, f_SFXVolume);
		break;
	}
}

void AC_MiniGamesPortfolioGameModeBase::PlaySoundEffectAtLocation(const ESoundEffectToPlay& EnumSoundEffect, const FVector& LocationToPlaySound)
{
	switch (EnumSoundEffect)
	{
	case ESoundEffectToPlay::COIN_COLLECT:
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), soundC_CoinCollect, LocationToPlaySound, f_SFXVolume);
		break;

	case ESoundEffectToPlay::COIN_BURST:
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), soundC_CoinBurst, LocationToPlaySound, f_SFXVolume);
		break;

	case ESoundEffectToPlay::BALL_BOUNCE_GROUND:
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), soundC_BallBounceOffGround, LocationToPlaySound, f_SFXVolume);
		break;

	case ESoundEffectToPlay::BALL_BOUNCE_WALL:
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), soundC_BallBounceOffWall, LocationToPlaySound, f_SFXVolume);
		break;

	case ESoundEffectToPlay::BALL_FALLING:
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), soundC_BallFalling, LocationToPlaySound, f_SFXVolume);
		break;

	case ESoundEffectToPlay::BALL_FLATTENING:
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), soundC_BallFlatten, LocationToPlaySound, f_SFXVolume);
		break;

	case ESoundEffectToPlay::BALL_BURST:
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), soundC_BallBurst, LocationToPlaySound, f_SFXVolume);
		break;

	case ESoundEffectToPlay::RISER_WIND:
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), soundC_RiserWind, LocationToPlaySound, f_SFXVolume);
		break;

	case ESoundEffectToPlay::GOAL_REACH:
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), soundC_GoalReach, LocationToPlaySound, f_SFXVolume);
		break;

	case ESoundEffectToPlay::CONFIRM_SOUND:
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), soundC_CoinCollect, LocationToPlaySound, f_SFXVolume);
		break;

	case ESoundEffectToPlay::CANCEL_SOUND:
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), soundC_CoinBurst, LocationToPlaySound, f_SFXVolume);
		break;
	}
}

void AC_MiniGamesPortfolioGameModeBase::ChangeBGM(const EMusicToPlay EnumMusic, const float& f_changeTimer)
{
	switch (EnumMusic)
	{
	case EMusicToPlay::LEVEL_SONG:
		soundC_BGM_Current = soundC_BGM_Level;
		break;

	case EMusicToPlay::VICTORY_SONG:
		soundC_BGM_Current = soundC_BGM_Victory;
		break;
	}

	FadeStopBGM(f_changeTimer);

	FTimerHandle th_TimerHandle;
	GetWorldTimerManager().SetTimer(th_TimerHandle, this, &AC_MiniGamesPortfolioGameModeBase::ChangeBGM, f_changeTimer + 0.2f);
}

void AC_MiniGamesPortfolioGameModeBase::PlayFromStartBGM()
{
	if (audioC_BGM != nullptr)
	{
		if (soundC_BGM_Current != nullptr)
		{
			soundC_BGM_Current->VolumeMultiplier = 1.0f;
		}

		audioC_BGM->Play();
	}
}

void AC_MiniGamesPortfolioGameModeBase::PauseBGM(const bool& b_Paused)
{
	if (audioC_BGM != nullptr)
	{
		audioC_BGM->SetPaused(b_Paused);
	}
}

void AC_MiniGamesPortfolioGameModeBase::StopBGM()
{
	if (audioC_BGM != nullptr)
	{
		audioC_BGM->Stop();
	}
}

void AC_MiniGamesPortfolioGameModeBase::FadeStopBGM(const float& f_TimeFade)
{
	if (audioC_BGM != nullptr)
	{
		audioC_BGM->FadeOut(f_TimeFade, 0.0f, EAudioFaderCurve::SCurve);
	}
}

void AC_MiniGamesPortfolioGameModeBase::UpdateBGMVolume(const float& f_newBGMVolume)
{
	if (GetBGMPlayer() != nullptr)
	{
		f_BGMVolume = f_newBGMVolume;

		GetBGMPlayer()->SetVolumeMultiplier(f_BGMVolume);
	}
}

void AC_MiniGamesPortfolioGameModeBase::UpdateSFXVolume(const float& f_newSFXVolume)
{
	f_SFXVolume = f_newSFXVolume;
}

void AC_MiniGamesPortfolioGameModeBase::PauseGame(const bool& b_Pause)
{
	if (BallGameHUD != nullptr)
	{
		b_GameIsPaused = b_Pause;

		if (b_Pause == true)
		{
			BallGameHUD->PauseGame();
		}
		else
		{
			BallGameHUD->UnpauseGame();
		}
	}
}

bool AC_MiniGamesPortfolioGameModeBase::GetGameIsPaused()
{
	if (AC_PlayerController* PC = Cast<AC_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		return PC->IsPaused();
	}

	return false;
}

void AC_MiniGamesPortfolioGameModeBase::SaveAsyncGameData()
{
	usg_saveData = Cast<UC_SaveData>(UGameplayStatics::CreateSaveGameObject(UC_SaveData::StaticClass()));

	if (BallGameHUD != nullptr)
	{
		BallGameHUD->SetSaveLoadStatusText(BallGameHUD->cfs_SavingDataText);

		BallGameHUD->SetSaveLoadDataCanvasVisibility(ESlateVisibility::Visible);
	}

	if (usg_saveData != nullptr)
	{
		usg_saveData->SetBGMVolume(f_BGMVolume);

		usg_saveData->SetSFXVolume(f_SFXVolume);

		usg_saveData->SetNewRecordScore(unsi_ScoreRecord);

		usg_saveData->SetNewRecordTime(f_LevelTimerRecord);

		usg_saveData->SetCamSpeedValue(f_camSpeed);

		UpdateScoreText();
		UpdateTimeRecordText();

		FAsyncSaveGameToSlotDelegate SavedDelegate;

		SavedDelegate.BindUObject(this, &AC_MiniGamesPortfolioGameModeBase::OnSaveBehavior);

		UGameplayStatics::AsyncSaveGameToSlot(usg_saveData, TEXT("BallGameRecordsAndSystems"), 0, SavedDelegate);
	}
	
}

void AC_MiniGamesPortfolioGameModeBase::LoadAsyncGameData()
{
	FAsyncLoadGameFromSlotDelegate LoadedDelegate;

	LoadedDelegate.BindUObject(this, &AC_MiniGamesPortfolioGameModeBase::OnLoadBehavior);

	UGameplayStatics::AsyncLoadGameFromSlot(TEXT("BallGameRecordsAndSystems"), 0, LoadedDelegate);
}

void AC_MiniGamesPortfolioGameModeBase::CreateSaveData()
{
	if (BallGameHUD != nullptr)
	{
		BallGameHUD->SetSaveLoadStatusText(BallGameHUD->cfs_LoadingFailedText);
	}

	FTimerHandle th_TimerHandle_Save;
	GetWorldTimerManager().SetTimer(th_TimerHandle_Save, this, &AC_MiniGamesPortfolioGameModeBase::SaveAsyncGameData, 1.5f);

	FTimerHandle th_TimerHandle_Menu;
	GetWorldTimerManager().SetTimer(th_TimerHandle_Menu, this, &AC_MiniGamesPortfolioGameModeBase::OpenGameMenu, 3.0f);
}

void AC_MiniGamesPortfolioGameModeBase::RestoreRecordsSaveData()
{
	if (BallGameHUD != nullptr)
	{
		BallGameHUD->SetSaveLoadStatusText(BallGameHUD->cfs_RestoreSaveText);

		BallGameHUD->SetSaveLoadDataCanvasVisibility(ESlateVisibility::Visible);
	}

	f_LevelTimerRecord = 599.999f;

	unsi_ScoreRecord = 0;

	FTimerHandle th_TimerHandle_Save;
	GetWorldTimerManager().SetTimer(th_TimerHandle_Save, this, &AC_MiniGamesPortfolioGameModeBase::SaveAsyncGameData, 1.5f);

	FTimerHandle th_TimerHandle_Menu;
	GetWorldTimerManager().SetTimer(th_TimerHandle_Menu, this, &AC_MiniGamesPortfolioGameModeBase::CloseLoadingDataCanvasAndOpenMainMenu, 4.0f);
}

void AC_MiniGamesPortfolioGameModeBase::GameHUDOpenResults()
{
	if (BallGameHUD != nullptr)
	{
		BallGameHUD->OpenGameOverVictory();
	}
}

void AC_MiniGamesPortfolioGameModeBase::ChangeBGM()
{
	if (audioC_BGM != nullptr)
	{
		audioC_BGM->SetSound(soundC_BGM_Current);

		PlayFromStartBGM();
	}
}
