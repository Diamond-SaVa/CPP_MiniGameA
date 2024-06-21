// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "C_SaveData.h"
#include "GameFramework/SaveGame.h"
#include "C_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class C_MINIGAMESPORTFOLIO_API UC_GameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:

	/// <summary>
	/// Stored Record of the shortest amount of time a level was finished.
	/// </summary>
	UPROPERTY(VisibleAnywhere);
		float f_recordTime = 0.0f;

	/// <summary>
	/// Stored Record of most coinds collected in a level.
	/// </summary>
	UPROPERTY(VisibleAnywhere);
		uint32 unsi_scoreRecord = 0;

	/// <summary>
	/// Stored value of Background Music volume.
	/// </summary>
	UPROPERTY(VisibleAnywhere);
		float f_BGMVolume = 1.0f;

	/// <summary>
	/// Stored value of Sound Effects volume.
	/// </summary>
	UPROPERTY(VisibleAnywhere);
		float f_SFXVolume = 1.0f;

	/// <summary>
	/// Stored value of the speed of camera movement.
	/// </summary>
	UPROPERTY(VisibleAnywhere);
		float f_camSpeed = 1.0f;

	/// <summary>
	/// Stored value of inverted camera; will become negative if Inverted Camera is set.
	/// </summary>
		UPROPERTY(VisibleAnywhere);
		float f_camInvert = 1.0f;
		
	/// <summary>
	/// Boolean that checks if the game has been loaded, skipping re-loading the save data.
	/// </summary>
	UPROPERTY(VisibleAnywhere);
		bool b_GameLoaded = false;

	/// <summary>
	/// Boolean that checks if the level has been reloaded, for certain functions in the Game Mode.
	/// </summary>
	UPROPERTY(VisibleAnywhere);
		bool b_LevelReloaded = false;
		
	/// <summary>
	/// Boolean that checks if the game is currently inside the Main Menu.
	/// </summary>
	UPROPERTY(VisibleAnywhere);
		bool b_GameInMainMenu = true;

public:

	UC_GameInstance();

	/// <summary>
	/// Sets a new value for the variable f_recordTime.
	/// </summary>
	/// <param name="f_newRecord">New Value</param>
	void SetNewRecordTime(const float& f_newRecord) { f_recordTime = f_newRecord; }

	float GetRecordTime() { return f_recordTime; }

	/// <summary>
	/// Sets a new value for the variable unsi_scoreRecord.
	/// </summary>
	/// <param name="unsi_newRecord">New Value</param>
	void SetNewRecordScore(const uint32& unsi_newRecord) { unsi_scoreRecord = unsi_newRecord; }

	uint32 GetRecordScore() { return unsi_scoreRecord; }

	/// <summary>
	/// Sets a new value for the variable f_BGMVolume.
	/// </summary>
	/// <param name="f_newVolume">New Value</param>
	void SetBGMVolume(float f_newVolume) { f_BGMVolume = f_newVolume; }

	/// <summary>
	/// Gets the value of the Background Music's Volume.
	/// </summary>
	/// <returns></returns>
	float GetBGMVolume() { return f_BGMVolume; }

	/// <summary>
	/// Sets a new value for the variable f_SFXVolume.
	/// </summary>
	/// <param name="f_newVolume">New Value</param>
	void SetSFXVolume(float f_newVolume) { f_SFXVolume = f_newVolume; }

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	float GetCamSpeedValue() { return f_camSpeed; }
	
	void SetCamSpeedValue(const float& f_newSpeed);

	/// <summary>
	/// Gets the value of the Sound Effect's Volume.
	/// </summary>
	/// <returns></returns>
	float GetSFXVolume() { return f_SFXVolume; }

	/// <summary>
	/// Sets the game to already has been loaded.
	/// </summary>
	void GameWasLoaded() { b_GameLoaded = true; }

	/// <summary>
	/// Checks if the game has already been loaded at start up.
	/// </summary>
	/// <returns></returns>
	bool WasGameLoaded() { return b_GameLoaded; }

	/// <summary>
	/// Sets the level to already has been reloaded.
	/// </summary>
	void LevelWasReloaded() { b_LevelReloaded = true; }

	/// <summary>
	/// Checks if the level has been reloaded already.
	/// </summary>
	/// <returns></returns>
	bool WasLevelReloaded() { return b_LevelReloaded; }

	/// <summary>
	/// Sets the game to know its no longer in the Main Menu.
	/// </summary>
	void ExitFromMainMenu() { b_GameInMainMenu = false; }

	/// <summary>
	/// Checks if the game is currently in the Main Menu.
	/// </summary>
	/// <returns></returns>
	bool IsGameInMainMenu() { return b_GameInMainMenu; }

	/// <summary>
	/// Resets the boolean values of b_GameLoaded and b_FromReload
	/// </summary>
	void ResetGameStateToMainMenu() { b_GameLoaded = false; b_LevelReloaded = false; b_GameInMainMenu = true; }

};
