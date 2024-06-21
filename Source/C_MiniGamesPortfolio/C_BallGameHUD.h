// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/RadialSlider.h"
#include "Components/CheckBox.h"
#include "Engine/Texture2D.h"
#include "Animation/WidgetAnimation.h"
#include "MediaPlayer.h"
#include "MediaSource.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "C_BallGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class C_MINIGAMESPORTFOLIO_API UC_BallGameHUD : public UUserWidget
{
	GENERATED_BODY()

public:

	enum EBallGameMenu : uint8
	{
		MAIN_MENU = 1,
		SETTINGS = 2,
		RESET_RECORDS = 3,
		PAUSE = 4,
		GAME_OVER = 5,
		NONE = 0
	};

	EBallGameMenu enum_CurrentMenu;

	/// <summary>
	/// Text to display when the game is loading Save Data
	/// </summary>
	const FString cfs_SavingDataText = "Loading save data...";

	/// <summary>
	/// Text to display when the game is creating new save data.
	/// </summary>
	const FString cfs_LoadingFailedText = "Save data not found. Creating new save data...";

	/// <summary>
	/// Text to display when the game is ready to restore records.
	/// </summary>
	const FString cfs_RestoreSaveText = "Bye bye, records...";

	/// <summary>
	/// Text to display when the game has created save data at start up.
	/// </summary>
	const FString cfs_CreatedSaveText = "Save data created.";

	/// <summary>
	/// Text to display when the game has saved successfully
	/// </summary>
	const FString cfs_SavingSuccess = "Data saved successfully.";

	/// <summary>
	/// Text to display when the game, somehow, couldn't save properly.
	/// </summary>
	const FString cfs_SavingFailed = "Data could not be saved.";

	/// <summary>
	/// Text to display when the game has loaded the save data successfully.
	/// </summary>
	const FString cfs_LoadingSuccessText = "Loading successful.";

	/// <summary>
	/// Quickly update the score text with the current score, max available score, and record score.
	/// </summary>
	/// <param name="fs_ScoreText">FString of the formatted record text = "{current} / {total} (RECORD : {record})</param>
	FORCEINLINE void UpdateTextScore(const FString& fs_ScoreText) { text_Score->SetText(FText::FromString("SCORE : " + fs_ScoreText)); }

	/// <summary>
	/// Quickly updates the timer text with the current, formated time.
	/// </summary>
	/// <param name="fs_TimerText"></param>
	FORCEINLINE void UpdateTextTimer(const FString& fs_TimerText) { text_Timer->SetText(FText::FromString("TIMER : " + fs_TimerText)); }

	/// <summary>
	/// Update the new record timer text at start up of the level or when a new record is being ser.
	/// </summary>
	/// <param name="fs_NewRecordText"></param>
	FORCEINLINE void UpdateTextTimerRecord(const FString& fs_NewRecordText) { text_TimerRecord->SetText(FText::FromString("RECORD : " + fs_NewRecordText)); }

	/// <summary>
	/// UFUNCTION that is called by the BeginGame button when On Clicked.
	/// Sets the game to change into GameOnly Mode, and calls the MiniGameMode to stream load the level.
	/// Also closes any motion running in the Main Menu, like the how to play vídeo.
	/// </summary>
	UFUNCTION()
	void OpenStreamLevel();

	/// <summary>
	/// Only called by the player's input from the Ball Character. Sets the game on pause while opening the Pause Menu.
	/// </summary>
	UFUNCTION()
	void PauseGame();
	
	/// <summary>
	/// Only called from the Pause Menu's button. Unpauses the game and closes the Pause Menuu.
	/// </summary>
	UFUNCTION()
	void UnpauseGame();

	/// <summary>
	/// Calls the camera manager to begin the procedure of exiting the game.
	/// </summary>
	UFUNCTION()
	void LeaveGame();

	/// <summary>
	/// Calls the camera manager to begin the procedure of restarting and reloading the level.
	/// </summary>
	UFUNCTION()
	void RestartLevel();
	
	/// <summary>
	/// Calls the camera manager to begin the procedure of unloading the level, reset C_GameInstance's variables and re-open the scene as if the game was restarted.
	/// </summary>
	UFUNCTION()
	void FromGameToMainMenu();

	/// <summary>
	/// Simply orders the game to be quitting according to platform's best practices. Currently only PC platform.
	/// </summary>
	UFUNCTION()
	void JustExitTheGame();

	/// <summary>
	/// Opens the Settings Canvas while closing the Main Menu Canvas.
	/// </summary>
	UFUNCTION()
	void OpenSettings();

	/// <summary>
	/// Closes the Settings Canvas, saves the game's modified data, and then opens the Main Menu Canvas.
	/// </summary>
	UFUNCTION()
	void CloseSettingsAndSave();
	
	/// <summary>
	/// Opens the Confirmation of Reset Records Canvas.
	/// </summary>
	UFUNCTION()
	void OpenConfirmResetScreen();

	/// <summary>
	/// Closes the Confirmation of Reset Records Canvas.
	/// </summary>
	UFUNCTION()
	void CloseConfirmResetScreen();

	/// <summary>
	/// Closes the Confirmation of Reset Records Canvas, as well as the Settings Canvas, and begins procedure to restore default variables for records.
	/// </summary>
	UFUNCTION()
	void RestoreAllRecordsThenSave();
	
	/// <summary>
	/// When the dial of the Mouse Slider changes its value, it updates its current value in the the Mini Game Mode, C_GameInstance and Character.
	/// </summary>
	/// <param name="f_Value"></param>
	UFUNCTION()
	void MouseValueChange(float f_Value);

	/// <summary>
	/// When the dial of the BGM Slider changes its value, it updates its current value in the Music Audio Component in the Mini Game Mode.
	/// </summary>
	/// <param name="f_Value"></param>
	UFUNCTION()
	void BGMValueChange(float f_Value);

	/// <summary>
	/// When the dial of the SFX Slider changes its value, it updates its current value in the Mini Game Mode, which is a manager for playing Sound Effects.
	/// </summary>
	/// <param name="f_Value"></param>
	UFUNCTION()
	void SFXValueChange(float f_Value);

	/// <summary>
	/// When the SFX Slider is let go, calls for the Mini Game Manager to make a test by playing a Sound Effect.
	/// </summary>
	UFUNCTION()
	void SFXSliderLetGo();

	/// <summary>
	/// Sets the instance of this class to know which menu is being controlled.
	/// </summary>
	/// <param name="enum_NewMenu">Enumerator that is now in control.</param>
	void SetEnumCurrentMenu(const EBallGameMenu& enum_NewMenu) { enum_CurrentMenu = enum_NewMenu; }

	/// <summary>
	/// Depending on the new value, the icon for the BGM Border component changes to reflect the level of audio.
	/// </summary>
	/// <param name="f_Value"></param>
	void BGMChangeBorderIcon(const float& f_Value);

	/// <summary>
	/// Depending on the new value, the icon for the SFX Border component changes to reflect the level of audio.
	/// </summary>
	/// <param name="f_Value"></param>
	void SFXChangeBorderIcon(const float& f_Value);

	/// <summary>
	/// Called on game being loaded for the first time, updates the values and textures for all Sliders.
	/// </summary>
	/// <param name="f_BGMVolume">BGM Value</param>
	/// <param name="f_SFXVolume">SFX Value</param>
	void SetSliderLoadedValues(const float& f_BGMVolume, const float& f_SFXVolume, const float& f_camSpeed = 1.0f);

	/// <summary>
	/// Sets the input to GameModeAndUI to enable mouse clicks input while opening the game over canvas.
	/// </summary>
	void OpenGameOverVictory();

	/// <summary>
	/// Makes the Save and Load icon visible or collapsed.
	/// </summary>
	/// <param name="eSlate_IsVisible"></param>
	void SetSaveLoadIconVisibility(const ESlateVisibility eSlate_IsVisible);

	/// <summary>
	/// Starts the animation to flash the record time text to indicate that a new record was reached.
	/// </summary>
	void FlashRecords();

	/// <summary>
	/// Makes the Save and Load Canvas with text visible or collapsed.
	/// </summary>
	/// <param name="enumVisibility"></param>
	void SetSaveLoadDataCanvasVisibility(const ESlateVisibility& enumVisibility);

	/// <summary>
	/// Makes the Loading Canvas screen as visible or collapsed while loading and unloading scenes.
	/// </summary>
	/// <param name="enumVisibility"></param>
	void SetLoadingSceneCanvasVisibility(const ESlateVisibility& enumVisibility);

	/// <summary>
	/// Makes the Settings Canvas visible or collapsed.
	/// </summary>
	/// <param name="enumVisibility"></param>
	void SetSettingsCanvasVisibility(const ESlateVisibility& enumVisibility);

	/// <summary>
	/// Makes the Reset Records Confirmation Canvas visible or collapsed.
	/// </summary>
	/// <param name="enumVisibility"></param>
	void SetConfirmResetCanvasVisibility(const ESlateVisibility& enumVisibility);

	/// <summary>
	/// Makes the Main Menu Canvas visible or collapsed.
	/// </summary>
	/// <param name="enumVisibility"></param>
	void SetMainMenuVisibility(const ESlateVisibility& enumVisibility);

	/// <summary>
	/// Makes the Status Canvas visible or collapsed.
	/// </summary>
	/// <param name="enumVisibility"></param>
	void SetSatusCanvasVisibility(const ESlateVisibility& enumVisibility);

	/// <summary>
	/// Makes the Game Over Canvas visible or collapsed.
	/// </summary>
	/// <param name="enumVisibility"></param>
	void SetGameOverCanvasVisibility(const ESlateVisibility& enumVisibility);

	/// <summary>
	/// Makes the Pause Canvas visible or collapsed.
	/// </summary>
	/// <param name="enumVisibility"></param>
	void SetPauseCanvasVisibility(const ESlateVisibility& enumVisibility);

	/// <summary>
	/// Changes the Text on the Saving Loading Data Canvas.
	/// </summary>
	/// <param name="fs_LoadingText"></param>
	void SetSaveLoadStatusText(const FString& fs_LoadingText);

protected:

	bool b_HasMenuControl = false;

	UPROPERTY()
		UButton* button_CurrentButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Textures")
		TArray<UTexture2D*> txtr_BGMVolumeIMGs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Textures")
		TArray<UTexture2D*> txtr_SFXVolumeIMGs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Videos")
		UMediaPlayer* mPlayer_VideoPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Videos")
		UMediaSource* mSource_VideoFile;

	UPROPERTY(meta = (BindWidget))
		UCanvasPanel* canvas_Status;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* text_Score;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* text_Timer;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* text_TimerRecord;

	UPROPERTY(meta = (BindWidget))
		UCanvasPanel* canvas_PauseMenu;

	UPROPERTY(meta = (BindWidget))
		UButton* button_Unpause;
	
	UPROPERTY(meta = (BindWidget))
		UButton* button_PauseToMenu;

	UPROPERTY(meta = (BindWidget))
		UButton* button_Exit;

	UPROPERTY(VisibleAnywhere)
		int8 i8_PauseMenuCursor = 0;
	
	UPROPERTY(meta = (BindWidget))
		UCanvasPanel* canvas_GameOver;
	
	UPROPERTY(meta = (BindWidget))
		UButton* button_RetryGame;
	
	UPROPERTY(meta = (BindWidget))
		UButton* button_GoToMainMenu;
	
	UPROPERTY(meta = (BindWidget))
		UButton* button_LeaveGame;

	UPROPERTY(VisibleAnywhere)
		int8 i8_GameOverMenuCursor = 0;

	UPROPERTY(meta = (BindWidget))
		UCanvasPanel* canvas_MainMenu;

	UPROPERTY(meta = (BindWidget))
		UButton* button_BeginGame;

	UPROPERTY(meta = (BindWidget))
		UButton* button_Settings;
		
	UPROPERTY(meta = (BindWidget))
		UButton* button_Settings_Pause;

	UPROPERTY(meta = (BindWidget))
		UButton* button_ExitTheGame;

	UPROPERTY(VisibleAnywhere)
		int8 i8_MainMenuCursor = 0;

	UPROPERTY(meta = (BindWidget))
		UCanvasPanel* canvas_Settings;

	UPROPERTY(meta = (BindWidget))
		UBorder* border_BGM;

	UPROPERTY(meta = (BindWidget))
		URadialSlider* rSlider_BGM;

	UPROPERTY(meta = (BindWidget))
		UBorder* border_SFX;

	UPROPERTY(meta = (BindWidget))
		URadialSlider* rSlider_SFX;

	UPROPERTY(meta = (BindWidget))
		URadialSlider* rSlider_Mouse;
		
	UPROPERTY(meta = (BindWidget))
		UCheckBox* cameraInvertX_Check;

	UPROPERTY(meta = (BindWidget))
		UButton* button_SaveSettings;

	UPROPERTY(meta = (BindWidget))
		UButton* button_RestartRecords;

	UPROPERTY(VisibleAnywhere)
		int8 i8_SettingsMenuCursor = 0;

	UPROPERTY(meta = (BindWidget))
		UCanvasPanel* canvas_ConfirmReset;

	UPROPERTY(meta = (BindWidget))
		UButton* button_RestartRecordsConfirm;

	UPROPERTY(meta = (BindWidget))
		UButton* button_ReturnToSettings;

	UPROPERTY(VisibleAnywhere)
		int8 i8_ConfirmResetMenuCursor = 0;

	UPROPERTY(meta = (BindWidget))
		UCanvasPanel* canvas_LoadingScene;

	UPROPERTY(meta = (BindWidget))
		UImage* img_SaveLoadIcon;
	
	UPROPERTY(meta = (BindWidget))
		UCanvasPanel* canvas_SaveLoadData;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* text_LoadingText;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* wAnim_RecordTextFlash;

	virtual void NativeConstruct() override;

	virtual void BeginDestroy() override;

	/// <summary>
	/// Freezes and Stops Player's Character to avoid issues while loading or unloading levels.
	/// </summary>
	void FreezeAndStopPlayer();

	void MoveMouseLocationToWidget(UWidget* w_Widget);

};
