// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BallGameHUD.h"
#include "C_PlayerController.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Widgets/Layout/Anchors.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"
#include "C_FallCamera.h"
#include "C_BallCharacter.h"
#include "C_GameInstance.h"
#include "C_MiniGamesPortfolioGameModeBase.h"

static AC_MiniGamesPortfolioGameModeBase* MiniGameModePtr;

void UC_BallGameHUD::OpenStreamLevel()
{
	if (AC_PlayerController* PC = Cast<AC_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		PC->SetInputMode(FInputModeGameOnly());

		PC->SetShowMouseCursor(false);
	}

	if (mPlayer_VideoPlayer != nullptr && mSource_VideoFile != nullptr)
	{
		mPlayer_VideoPlayer->Close();
	}

	if (MiniGameModePtr != nullptr)
	{
		SetMainMenuVisibility(ESlateVisibility::Collapsed);

		MiniGameModePtr->LoadStreamedLevel();
	}
}

void UC_BallGameHUD::PauseGame()
{
	if (AC_PlayerController* PC = Cast<AC_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		FInputModeGameAndUI InputModo;

		InputModo.SetHideCursorDuringCapture(false);

		InputModo.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);

		InputModo.SetWidgetToFocus(GetCachedWidget());

		PC->SetInputMode(InputModo);

		PC->SetShowMouseCursor(true);

		SetPauseCanvasVisibility(ESlateVisibility::Visible);

		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}

void UC_BallGameHUD::UnpauseGame()
{
	if (AC_PlayerController* PC = Cast<AC_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		PC->SetPause(false);

		PC->SetInputMode(FInputModeGameOnly());

		PC->SetShowMouseCursor(false);

		SetPauseCanvasVisibility(ESlateVisibility::Collapsed);

		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}
}

void UC_BallGameHUD::LeaveGame()
{
	if (AActor* FallCameraActor = UGameplayStatics::GetActorOfClass(GetWorld(), AC_FallCamera::StaticClass()))
	{
		if (AC_FallCamera* FallCamera = Cast<AC_FallCamera>(FallCameraActor))
		{
			FallCamera->StartFadeToQuit();
		}
	}

	FreezeAndStopPlayer();

	UnpauseGame();

	RemoveFromParent();
}

void UC_BallGameHUD::RestartLevel()
{
	if (AActor* FallCameraActor = UGameplayStatics::GetActorOfClass(GetWorld(), AC_FallCamera::StaticClass()))
	{
		if (AC_FallCamera* FallCamera = Cast<AC_FallCamera>(FallCameraActor))
		{
			FallCamera->StartFadeToReloadLevel();

			if (AC_PlayerController* PC = Cast<AC_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
			{

				PC->SetInputMode(FInputModeGameOnly());

				PC->SetShowMouseCursor(false);
			}
		}
	}

	SetGameOverCanvasVisibility(ESlateVisibility::Collapsed);

	FreezeAndStopPlayer();

	UnpauseGame();
}

void UC_BallGameHUD::FromGameToMainMenu()
{
	SetPauseCanvasVisibility(ESlateVisibility::Collapsed);
	SetGameOverCanvasVisibility(ESlateVisibility::Collapsed);
	SetSatusCanvasVisibility(ESlateVisibility::Collapsed);

	FreezeAndStopPlayer();
	UnpauseGame();

	if (AActor* FallCameraActor = UGameplayStatics::GetActorOfClass(GetWorld(), AC_FallCamera::StaticClass()))
	{
		if (AC_FallCamera* FallCamera = Cast<AC_FallCamera>(FallCameraActor))
		{
			FallCamera->StartFadeToRestartGame();
		}
	}
}

void UC_BallGameHUD::JustExitTheGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);
}

void UC_BallGameHUD::OpenSettings()
{
	if (mPlayer_VideoPlayer != nullptr && mSource_VideoFile != nullptr)
	{
		mPlayer_VideoPlayer->Play();
	}

	SetEnumCurrentMenu(SETTINGS);
	SetSettingsCanvasVisibility(ESlateVisibility::Visible);
	SetMainMenuVisibility(ESlateVisibility::Collapsed);

	if (MiniGameModePtr != nullptr)
	{
		if (MiniGameModePtr->GetGameInstance() != nullptr)
		{
			if (!MiniGameModePtr->GetGameInstance()->IsGameInMainMenu())
			{
				button_RestartRecords->SetVisibility(ESlateVisibility::Collapsed);

				canvas_PauseMenu->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				button_RestartRecords->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

void UC_BallGameHUD::CloseSettingsAndSave()
{
	if (mPlayer_VideoPlayer != nullptr && mSource_VideoFile != nullptr)
	{
		mPlayer_VideoPlayer->Pause();
	}

	if (MiniGameModePtr != nullptr)
	{
		if (MiniGameModePtr->GetGameInstance() != nullptr)
		{
			if (MiniGameModePtr->GetGameInstance()->IsGameInMainMenu() == false)
			{
				UnpauseGame();
			}
		}
	}

	if (MiniGameModePtr != nullptr)
	{
		SetSettingsCanvasVisibility(ESlateVisibility::Collapsed);

		SetSaveLoadDataCanvasVisibility(ESlateVisibility::Visible);

		SetSaveLoadStatusText(cfs_SavingDataText);

		MiniGameModePtr->SaveAsyncGameData();
	}
}

void UC_BallGameHUD::OpenConfirmResetScreen()
{
	SetConfirmResetCanvasVisibility(ESlateVisibility::Visible);
	SetEnumCurrentMenu(RESET_RECORDS);
}

void UC_BallGameHUD::CloseConfirmResetScreen()
{
	SetConfirmResetCanvasVisibility(ESlateVisibility::Collapsed);
	SetEnumCurrentMenu(SETTINGS);
}

void UC_BallGameHUD::RestoreAllRecordsThenSave()
{
	SetSettingsCanvasVisibility(ESlateVisibility::Collapsed);

	SetConfirmResetCanvasVisibility(ESlateVisibility::Collapsed);

	if (mPlayer_VideoPlayer != nullptr && mSource_VideoFile != nullptr)
	{
		mPlayer_VideoPlayer->Pause();
	}

	if (MiniGameModePtr != nullptr)
	{
		MiniGameModePtr->RestoreRecordsSaveData();
	}
}

void UC_BallGameHUD::MouseValueChange(float f_Value)
{
	if (MiniGameModePtr != nullptr)
	{
		MiniGameModePtr->SetCamSpeedValue(f_Value);
	}
}

void UC_BallGameHUD::BGMValueChange(float f_Value)
{
	if (MiniGameModePtr != nullptr)
	{
		MiniGameModePtr->UpdateBGMVolume(f_Value);
	}

	BGMChangeBorderIcon(f_Value);
}

void UC_BallGameHUD::SFXValueChange(float f_Value)
{
	if (MiniGameModePtr != nullptr)
	{
		MiniGameModePtr->UpdateSFXVolume(f_Value);
	}

	SFXChangeBorderIcon(f_Value);
}

void UC_BallGameHUD::SFXSliderLetGo()
{
	if (MiniGameModePtr != nullptr)
	{
		MiniGameModePtr->PlaySoundEffect(AC_MiniGamesPortfolioGameModeBase::COIN_COLLECT);
	}
}

void UC_BallGameHUD::BGMChangeBorderIcon(const float& f_Value)
{
	if (border_BGM != nullptr)
	{
		if (txtr_BGMVolumeIMGs.Num() != 0)
		{
			if (f_Value >= 1.0f)
			{
				if (txtr_BGMVolumeIMGs[0] != nullptr)
					border_BGM->SetBrushFromTexture(txtr_BGMVolumeIMGs[0]);
			}
			else if (f_Value < 1.0f && f_Value >= 0.5f)
			{
				if (txtr_BGMVolumeIMGs[1] != nullptr)
					border_BGM->SetBrushFromTexture(txtr_BGMVolumeIMGs[1]);
			}
			else if (f_Value < 0.5f && f_Value > 0.0f)
			{
				if (txtr_BGMVolumeIMGs[2] != nullptr)
					border_BGM->SetBrushFromTexture(txtr_BGMVolumeIMGs[2]);
			}
			else if (f_Value <= 0.0f)
			{
				if (txtr_BGMVolumeIMGs[3] != nullptr)
					border_BGM->SetBrushFromTexture(txtr_BGMVolumeIMGs[3]);
			}
		}
	}
}

void UC_BallGameHUD::SFXChangeBorderIcon(const float& f_Value)
{
	if (border_SFX != nullptr)
	{
		if (txtr_SFXVolumeIMGs.Num() != 0)
		{
			if (f_Value >= 1.0f)
			{
				if (txtr_SFXVolumeIMGs[0] != nullptr)
					border_SFX->SetBrushFromTexture(txtr_SFXVolumeIMGs[0]);
			}
			else if (f_Value < 1.0f && f_Value >= 0.5f)
			{
				if (txtr_SFXVolumeIMGs[1] != nullptr)
					border_SFX->SetBrushFromTexture(txtr_SFXVolumeIMGs[1]);
			}
			else if (f_Value < 0.5f && f_Value > 0.0f)
			{
				if (txtr_SFXVolumeIMGs[2] != nullptr)
					border_SFX->SetBrushFromTexture(txtr_SFXVolumeIMGs[2]);
			}
			else if (f_Value <= 0.0f)
			{
				if (txtr_SFXVolumeIMGs[3] != nullptr)
					border_SFX->SetBrushFromTexture(txtr_SFXVolumeIMGs[3]);
			}
		}
	}
}

void UC_BallGameHUD::SetSliderLoadedValues(const float& f_BGMVolume, const float& f_SFXVolume, const float& f_camSpeed)
{
	if (rSlider_Mouse != nullptr)
	{
		rSlider_Mouse->SetValue(f_camSpeed);
	}

	if (rSlider_BGM != nullptr)
	{
		rSlider_BGM->SetValue(f_BGMVolume);
	}

	BGMChangeBorderIcon(f_BGMVolume);

	if (rSlider_SFX != nullptr)
	{
		rSlider_SFX->SetValue(f_SFXVolume);
	}

	SFXChangeBorderIcon(f_SFXVolume);
}

void UC_BallGameHUD::OpenGameOverVictory()
{
	if (AC_PlayerController* PC = Cast<AC_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		SetGameOverCanvasVisibility(ESlateVisibility::Visible);

		FInputModeGameAndUI InputModo;

		InputModo.SetHideCursorDuringCapture(false);

		InputModo.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);

		InputModo.SetWidgetToFocus(GetCachedWidget());

		PC->SetInputMode(InputModo);

		PC->SetShowMouseCursor(true);
	}
}

void UC_BallGameHUD::SetSaveLoadIconVisibility(const ESlateVisibility eSlate_IsVisible)
{
	if (img_SaveLoadIcon != nullptr)
	{
		img_SaveLoadIcon->SetVisibility(eSlate_IsVisible);
	}
}

void UC_BallGameHUD::FlashRecords()
{
	if (wAnim_RecordTextFlash != nullptr)
	{
		PlayAnimation(wAnim_RecordTextFlash, 0.0f, 0);
	}
}

void UC_BallGameHUD::SetSaveLoadDataCanvasVisibility(const ESlateVisibility& enumVisibility)
{
	if (canvas_SaveLoadData != nullptr)
	{
		canvas_SaveLoadData->SetVisibility(enumVisibility);
	}
}

void UC_BallGameHUD::SetLoadingSceneCanvasVisibility(const ESlateVisibility& enumVisibility)
{
	if (canvas_LoadingScene != nullptr)
	{
		canvas_LoadingScene->SetVisibility(enumVisibility);
	}
}

void UC_BallGameHUD::SetSettingsCanvasVisibility(const ESlateVisibility& enumVisibility)
{
	if (canvas_Settings != nullptr)
	{
		canvas_Settings->SetVisibility(enumVisibility);
	}
}

void UC_BallGameHUD::SetConfirmResetCanvasVisibility(const ESlateVisibility& enumVisibility)
{
	if (canvas_ConfirmReset != nullptr)
	{
		canvas_ConfirmReset->SetVisibility(enumVisibility);
	}
}

void UC_BallGameHUD::SetMainMenuVisibility(const ESlateVisibility& enumVisibility)
{
	if (canvas_MainMenu != nullptr)
	{
		canvas_MainMenu->SetVisibility(enumVisibility);
	}
}

void UC_BallGameHUD::SetSatusCanvasVisibility(const ESlateVisibility& enumVisibility)
{
	if (canvas_Status != nullptr)
	{
		canvas_Status->SetVisibility(enumVisibility);
	}
}

void UC_BallGameHUD::SetGameOverCanvasVisibility(const ESlateVisibility& enumVisibility)
{
	if (canvas_GameOver != nullptr)
	{
		canvas_GameOver->SetVisibility(enumVisibility);
	}
}

void UC_BallGameHUD::SetPauseCanvasVisibility(const ESlateVisibility& enumVisibility)
{
	if (canvas_PauseMenu != nullptr)
	{
		canvas_PauseMenu->SetVisibility(enumVisibility);
	}
}

void UC_BallGameHUD::SetSaveLoadStatusText(const FString& fs_LoadingText)
{
	if (text_LoadingText != nullptr)
	{
		text_LoadingText->SetText(FText::FromString(fs_LoadingText));
	}
}

void UC_BallGameHUD::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (AC_PlayerController* PC = Cast<AC_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		SetIsFocusable(true);

		FInputModeGameAndUI InputModo;

		InputModo.SetHideCursorDuringCapture(false);

		InputModo.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);

		InputModo.SetWidgetToFocus(GetCachedWidget());

		PC->SetInputMode(InputModo);

		PC->SetShowMouseCursor(true);

		PC->SetMouseLocation(0, 0);

		SetFocus();
	}

	enum_CurrentMenu = EBallGameMenu::NONE;

	MiniGameModePtr = Cast<AC_MiniGamesPortfolioGameModeBase>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AC_MiniGamesPortfolioGameModeBase::StaticClass())
	);

	if (mPlayer_VideoPlayer != nullptr && mSource_VideoFile != nullptr)
	{
		mPlayer_VideoPlayer->OpenSource(mSource_VideoFile);
	}

	if (button_BeginGame != nullptr)
	{
		button_BeginGame->OnClicked.AddDynamic(this, &UC_BallGameHUD::OpenStreamLevel);
	}
	
	if (button_Unpause != nullptr)
	{
		button_Unpause->OnClicked.AddDynamic(this, &UC_BallGameHUD::UnpauseGame);
	}

	if (button_Unpause != nullptr)
	{
		button_Unpause->OnClicked.AddDynamic(this, &UC_BallGameHUD::UnpauseGame);
	}

	if (button_RetryGame != nullptr)
	{
		button_RetryGame->OnClicked.AddDynamic(this, &UC_BallGameHUD::RestartLevel);
	}
	
	if (button_GoToMainMenu != nullptr)
	{
		button_GoToMainMenu->OnClicked.AddDynamic(this, &UC_BallGameHUD::FromGameToMainMenu);
	}
	
	if (button_PauseToMenu != nullptr)
	{
		button_PauseToMenu->OnClicked.AddDynamic(this, &UC_BallGameHUD::FromGameToMainMenu);
	}

	if (button_Exit != nullptr)
	{
		button_Exit->OnClicked.AddDynamic(this, &UC_BallGameHUD::LeaveGame);
	}

	if (button_LeaveGame != nullptr)
	{
		button_LeaveGame->OnClicked.AddDynamic(this, &UC_BallGameHUD::LeaveGame);
	}
	
	if (button_ExitTheGame != nullptr)
	{
		button_ExitTheGame->OnClicked.AddDynamic(this, &UC_BallGameHUD::JustExitTheGame);
	}

	if (button_Settings != nullptr)
	{
		button_Settings->OnClicked.AddDynamic(this, &UC_BallGameHUD::OpenSettings);
	}
	
	if (button_Settings_Pause != nullptr)
	{
		button_Settings_Pause->OnClicked.AddDynamic(this, &UC_BallGameHUD::OpenSettings);
	}

	if (button_SaveSettings != nullptr)
	{
		button_SaveSettings->OnClicked.AddDynamic(this, &UC_BallGameHUD::CloseSettingsAndSave);
	}
	
	if (button_RestartRecords != nullptr)
	{
		button_RestartRecords->OnClicked.AddDynamic(this, &UC_BallGameHUD::OpenConfirmResetScreen);
	}

	if (button_ReturnToSettings != nullptr)
	{
		button_ReturnToSettings->OnClicked.AddDynamic(this, &UC_BallGameHUD::CloseConfirmResetScreen);
	}
	
	if (button_RestartRecordsConfirm != nullptr)
	{
		button_RestartRecordsConfirm->OnClicked.AddDynamic(this, &UC_BallGameHUD::RestoreAllRecordsThenSave);
	}

	if (rSlider_Mouse != nullptr)
	{
		rSlider_Mouse->OnValueChanged.AddDynamic(this, &UC_BallGameHUD::MouseValueChange);
	}
	
	if (rSlider_BGM != nullptr)
	{
		rSlider_BGM->OnValueChanged.AddDynamic(this, &UC_BallGameHUD::BGMValueChange);
	}

	if (rSlider_SFX != nullptr)
	{
		rSlider_SFX->OnValueChanged.AddDynamic(this, &UC_BallGameHUD::SFXValueChange);
		rSlider_SFX->OnMouseCaptureEnd.AddDynamic(this, &UC_BallGameHUD::SFXSliderLetGo);
	}
}

void UC_BallGameHUD::BeginDestroy()
{
	Super::BeginDestroy();

	MiniGameModePtr = nullptr;
}

void UC_BallGameHUD::FreezeAndStopPlayer()
{
	if (AActor* BallChar = UGameplayStatics::GetActorOfClass(GetWorld(), AC_BallCharacter::StaticClass()))
	{
		if (AC_BallCharacter* BallChara = Cast<AC_BallCharacter>(BallChar))
		{
			BallChara->SetGameOverToTrue();
		}
	}
}

void UC_BallGameHUD::MoveMouseLocationToWidget(UWidget* w_Widget)
{
	if (w_Widget != nullptr)
	{
		if (AC_PlayerController* PC = Cast<AC_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
		{
			UCanvasPanelSlot* slote = UWidgetLayoutLibrary::SlotAsCanvasSlot(w_Widget);

			if (slote != nullptr)
			{
				int32 VPSizeX;

				int32 VPSizeY;

				PC->GetViewportSize(VPSizeX, VPSizeY);

				double anchorX = slote->GetAnchors().Maximum.X;

				double anchorXCenter = anchorX * (double)VPSizeX;

				double relativeXPosition = (double)VPSizeX / 1920.0;

				double buttonRelativeX = anchorXCenter + (slote->GetPosition().X * relativeXPosition);

				double anchorY = slote->GetAnchors().Maximum.Y;

				double anchorYCenter = anchorY * VPSizeY;

				double relativeYPosition = (double)VPSizeY / 1080.0;

				double buttonRelativeY = anchorYCenter + (slote->GetPosition().Y * relativeYPosition);

				PC->SetMouseLocation((int)buttonRelativeX, (int)buttonRelativeY);
			}
		}
	}
}

