// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Blueprint/UserWidget.h"
#include "C_BallGameHUD.h"
#include "C_GameInstance.h"
#include "GameFramework/SaveGame.h"
#include "C_SaveData.h"
#include "Engine/Level.h"
#include "Engine/LevelStreaming.h"
#include "C_MiniGamesPortfolioGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class C_MINIGAMESPORTFOLIO_API AC_MiniGamesPortfolioGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AC_MiniGamesPortfolioGameModeBase();

	enum ESoundEffectToPlay : uint8
	{
		COIN_COLLECT = 0 UMETA(DisplayName = "COIN COLLECT"),
		COIN_BURST = 1 UMETA(DisplayName = "COIN BURST"),
		BALL_BOUNCE_GROUND = 2 UMETA(DisplayName = "BOUNCE FROM GROUND"),
		BALL_BOUNCE_WALL = 3 UMETA(DisplayName = "BOUNCE FROM WALL"),
		BALL_FALLING = 4 UMETA(DisplayName = "BALL FALLING FROM LEVEL"),
		BALL_FLATTENING = 5 UMETA(DisplayName = "BALL FLATTENING ITSELF"),
		BALL_BURST = 6 UMETA(DisplayName = "BALL BURSTING ITSELF"),
		RISER_WIND = 7 UMETA(DisplayName = "RISER WIND"),
		GOAL_REACH = 8 UMETA(DisplayName = "REACHED THE GOAL"),
		CONFIRM_SOUND = 9 UMETA(DisplayName = "CONFIRM SOUND"),
		CANCEL_SOUND = 10 UMETA(DisplayName = "CANCEL SOUND")
	};

	enum EMusicToPlay : uint8
	{
		LEVEL_SONG = 0 UMETA(DisplayName = "LEVEL SONG"),
		VICTORY_SONG = 1 UMETA(DisplayName = "VICTORY SONG")
	};

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "A_Persistent_Components")
		UAudioComponent* audioC_BGM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Persistent_Variables")
		USoundCue* soundC_BGM_Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Persistent_Variables")
		USoundCue* soundC_BGM_Victory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "A_Persistent_Variables")
		USoundCue* soundC_BGM_Current;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Persistent_Variables")
		USoundCue* soundC_CoinCollect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Persistent_Variables")
		USoundCue* soundC_CoinBurst;

	/// <summary>
	/// Sound to be played when falling into a Game Over.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "A_Persistent_Variables")
		USoundCue* soundC_BallFalling;

	/// <summary>
	/// Sound to be played when being flattened by a hazard.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "A_Persistent_Variables")
		USoundCue* soundC_BallFlatten;

	/// <summary>
	/// Sound to be played when making contact once with a hazard.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "A_Persistent_Variables")
		USoundCue* soundC_BallBurst;

	/// <summary>
	/// Sound to be played when ball makes contact with a riser.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "A_Persistent_Variables")
		USoundCue* soundC_RiserWind;

	/// <summary>
	/// Sound to be played when being bouncing and landing on a surface tagged as Ground.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "A_Persistent_Variables")
		USoundCue* soundC_BallBounceOffGround;

	/// <summary>
	/// Sound to be played when ball reaches the goal.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "A_Persistent_Variables")
		USoundCue* soundC_GoalReach;

	/// <summary>
	/// Sound to be played when being bouncing and landing on a surface tagged as Wall.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "A_Persistent_Variables")
		USoundCue* soundC_BallBounceOffWall;

	UPROPERTY(VisibleAnywhere, Category = "A_Persistent_Variables")
		uint32 unsi_Score = 0.0;

	UPROPERTY(VisibleAnywhere, Category = "A_Persistent_Variables")
		uint32 unsi_ScoreMax = 0.0;

	UPROPERTY(VisibleAnywhere, Category = "A_Persistent_Variables")
		uint32 unsi_ScoreRecord = 0.0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "A_Persistent_Variables")
		float f_LevelTimer = 0.0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "A_Persistent_Variables")
		float f_LevelTimerRecord = 599.999;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "A_Persistent_Variables")
		float f_BGMVolume = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "A_Persistent_Variables")
		float f_SFXVolume = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "A_Persistent_Variables")
		float f_camSpeed = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "A_Persistent_Variables")
		bool b_TimerActive = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "A_Persistent_Variables")
		bool b_GameIsPaused = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "A_Persistent_Variables")
		FName fName_LevelToStream = FName(TEXT("TestLevel"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Other")
		TSubclassOf<UUserWidget> uWidget_BallHUDClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "A_Other")
		UC_BallGameHUD* BallGameHUD;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "A_Other")
		UC_GameInstance* gInstance;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "A_Other")
		UC_SaveData* usg_saveData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "A_Other")
		ULevelStreaming* levelStream;

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaTime) override;

	void OpenGameMenu();

	void CloseSaveLoadDataCanvas();

	void CloseLoadingDataCanvasAndOpenMainMenu();

	void OnSaveBehavior(const FString& fs_SaveFileName, const int32 i32_SaveFileSlot, bool b_success);

	void OnLoadBehavior(const FString& fs_SaveFileName, const int32 i32_SaveFileSlot, USaveGame* baseSaveGame);

	UFUNCTION(BlueprintCallable, Category = Game)
		void OnLevelWasLoaded();

	UFUNCTION(BlueprintCallable, Category = Game)
		void OnLevelWasUnloaded();

public:
	UC_GameInstance* GetGameInstance()
	{
		return gInstance;
	}

	void StartingProcedures();

	void LoadStreamedLevel();

	void SetCamSpeedValue(const float& f_newValue) { f_camSpeed = f_newValue; }

	float GetCamSpeedValue() { return f_camSpeed; }

	UAudioComponent* GetBGMPlayer() { return audioC_BGM; }

	float* GetBGMVolumeValue() { return &f_BGMVolume; }

	float* GetSFXVolumeValue() { return &f_SFXVolume; }

	float* GetTimerValue() { return &f_LevelTimer; }

	void RestartTimer() { f_LevelTimer = 0.0f; }

	bool* GetTimerActiveValue() { return &b_TimerActive; }

	uint32* GetScoreMax() { return &unsi_Score; }

	void SetScoreMax(const uint32& unsi_NewMax) { unsi_Score = unsi_NewMax; }

	uint32* GetScoreRecord() { return &unsi_Score; }

	void SetTimerActive(const bool& b_NewTimerActive) { b_TimerActive = b_NewTimerActive; }

	uint32* GetScore() { return &unsi_Score; }

	void UpScore();

	void UpdateScoreText();

	void UpdateTimeRecordText();

	void SetUpMaxScoreAndRecords();

	void StopAndUpdateRecords();

	void PlaySoundEffect(const ESoundEffectToPlay& EnumSoundEffect);

	void PlaySoundEffectAtLocation(const ESoundEffectToPlay& EnumSoundEffect, const FVector& LocationToPlaySound);

	void ChangeBGM(const EMusicToPlay EnumMusic, const float& f_changeTimer = 2.5f);

	void PlayFromStartBGM();

	void PauseBGM(const bool& b_Paused);

	void StopBGM();

	void FadeStopBGM(const float& f_TimeFade = 2.0f);

	void UpdateBGMVolume(const float& f_newBGMVolume);

	void UpdateSFXVolume(const float& f_newSFXVolume);

	void PauseGame(const bool& b_Pause);

	bool GetGameIsPaused();

	void SaveAsyncGameData();

	void LoadAsyncGameData();

	void CreateSaveData();

	void RestoreRecordsSaveData();

	void GameHUDOpenResults();
	
private:
	EMusicToPlay enum_CurrentMusic = EMusicToPlay::LEVEL_SONG;

	void ChangeBGM();

};
