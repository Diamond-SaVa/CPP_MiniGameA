// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "C_FallCamera.generated.h"

/**
 * 
 */
UCLASS()
class C_MINIGAMESPORTFOLIO_API AC_FallCamera : public ACameraActor
{
	GENERATED_BODY()
	
public:
	/// <summary>
	/// Set Camera's Location atop the Position of the Player.
	/// </summary>
	/// <param name="v_PosRelativeTo">Position of the Player in the World.</param>
	void SetCameraLocation(FVector v_PosRelativeTo);
	
	/// <summary>
	/// Starts Fade In to a ReloadLevel function, calling the Game Mode as the Manager.
	/// </summary>
	void StartFadeToReloadLevel();

	/// <summary>
	/// Start Fade In to a RestartGame function, calling the Game Mode as the Manager.
	/// </summary>
	void StartFadeToRestartGame();

	/// <summary>
	/// Start Fade In to a OpenResultsScreen function, calling the Game Mode as the Manager.
	/// </summary>
	void StartFadeLevelVictory();

	/// <summary>
	/// Start Fade in to a QuitTheGame function, calling the Game Mode as the Manager.
	/// </summary>
	void StartFadeToQuit();

private:


protected:
	virtual void BeginPlay() override;

	/// <summary>
	/// Calls the Game Mode as the Manager to reload level.
	/// </summary>
	void ReloadLevel();

	/// <summary>
	/// Calls the Game Mode as the Manager to reopen the persistent scene while restarting C_GameInstance's important booleans.
	/// </summary>
	void RestartGame();

	/// <summary>
	/// Calls the Game Mode as the Manager to Open the Game Over screen.
	/// </summary>
	void OpenResultsScreen();

	/// <summary>
	/// Calls the Game Mode as the Manager to reload level.
	/// </summary>
	void QuitTheGame();
};