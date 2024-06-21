// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "C_SaveData.generated.h"

/**
 * 
 */
UCLASS()
class C_MINIGAMESPORTFOLIO_API UC_SaveData : public USaveGame
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, Category = Basic)
		float f_recordTime;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		uint32 unsi_scoreRecord;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		float f_BGMVolume;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		float f_SFXVolume;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		float f_camSpeed;

public:

	UC_SaveData();

	void SetNewRecordTime(const float& f_newRecord) { f_recordTime = f_newRecord; }

	float GetRecordTime() { return f_recordTime; }

	void SetNewRecordScore(const uint32& unsi_newRecord) { unsi_scoreRecord = unsi_newRecord; }

	uint32 GetRecordScore() { return unsi_scoreRecord; }

	void SetBGMVolume(float f_newVolume) { f_BGMVolume = f_newVolume; }

	float GetBGMVolume() { return f_BGMVolume; }

	void SetSFXVolume(float f_newVolume) { f_SFXVolume = f_newVolume; }

	float GetSFXVolume() { return f_SFXVolume; }

	void SetCamSpeedValue(float f_newValue) { f_camSpeed = f_newValue; }

	float GetCamSpeedValue() { return f_camSpeed; }
	
};
