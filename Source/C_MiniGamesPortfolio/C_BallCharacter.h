// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C_MiniGamesPortfolioGameModeBase.h"
#include "Components/StaticMeshComponent.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Curves/CurveFloat.h"
#include "C_BallCharacter.generated.h"

UCLASS()
class C_MINIGAMESPORTFOLIO_API AC_BallCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AC_BallCharacter();

protected:
	/// <summary>
	/// Called when the game starts or when spawned, loaded, etc.
	/// </summary>
	virtual void BeginPlay() override;

	/// <summary>
	/// Function automatically called when Instance of Object is destroyed or unloaded.
	/// </summary>
	virtual void BeginDestroy() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ball_Components")
		AC_MiniGamesPortfolioGameModeBase* MiniGameMode;

	/// <summary>
	/// Pointer to the Static Mesh where the sphere must go.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Ball_Components")
		UStaticMeshComponent* smc_BallMeshComp;

	/// <summary>
	/// Pointer to the Material that we will use to create a dynamic instance of it.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ball_Components")
		UMaterialInterface* mi_BallMaterial;

	/// <summary>
	/// Pointer to the Material Instance Dynamic to set onto smc_BallMeshSlot.
	/// It's made Dynamic in order to edit its parameters down the line.
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = "Ball_Components")
		UMaterialInstanceDynamic* mid_BallMatDynamic;

	/// <summary>
	/// Pointer to the Spring Arm Component that manages distance between cc_CameraComp and smc_BallMeshSlot.
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = "Ball_Components")
		USpringArmComponent* sac_CameraArm;

	/// <summary>
	/// Pointer to the Camera Component what renders the view for this Pawn.
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = "Ball_Components")
		UCameraComponent* cc_CameraComp;

	/// <summary>
	/// Pointer to the Sphere Component that manages collisions with other Actors and Components.
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = "Ball_Components")
		USphereComponent* sc_SphereComp;

	/// <summary>
	/// Pointer to the Audio Component that manages collisions with other Actors and Components.
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = "Ball_Components")
		UAudioComponent* audioC_BallAudio;

	/// <summary>
	/// Niagara system to attach to a new NiagaraSystem that spawns when making contact with hazards.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ball_Assets")
		UNiagaraSystem* ns_SmokeSys;

	/// <summary>
	/// Enumerator for different types of speeds according to terrain and tags.
	/// </summary>
	enum EMovementSpeed : uint8
	{
		NORMAL_SPEED = 0 UMETA(DispleyName = "NORMAL SPEED"),
		BOOST_SPEED = 1 UMETA(DispleyName = "BOOST SPEED"),
		MUD_SPEED = 2 UMETA(DispleyName = "MUD SPEED"),
		STOP_SPEED = 3 UMETA(DisplayName = "STOP BALL")
	};

	/// <summary>
	/// 
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ball_Properties")
		UCurveFloat* curveF_SmoothingCurve;

	/// <summary>
	/// 
	/// </summary>
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Ball_Properties")
		float f_InputTimer = 2.0f;

	/// <summary>
	/// Movement Vector taking into account the camera's rotation.
	/// </summary>
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Ball_Properties")
		FVector v_MoveVect = FVector::ZeroVector;

	/// <summary>
	/// Variable for the float value of Speed of the Sphere's Max Speed Walk.
	/// </summary>
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Ball_Properties")
		float f_speed = 0.0f;

	/// <summary>
	/// Variable for the float value of Speed of the Sphere's Movement on Normal surfaces.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ball_Properties")
		float f_normalSpeed = 650.0f;

	/// <summary>
	/// Variable for the float value of Speed of the Sphere's Movement on Boosted surfaces.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ball_Properties")
		float f_boostSpeed = 1200.0;

	/// <summary>
	/// Variable for the float value of Speed of the Sphere's Movement on Muddy Surfaces
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ball_Properties")
		float f_mudSpeed = 250.0f;

	/// <summary>
	/// Pointer to the currenly set max speed accoring to terrain and tags.
	/// </summary>
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Ball_Properties")
		float f_maxSpeed = 0.0f;

	/// <summary>
	/// Variable for the float value of the Rotation Speed of the Camera's Spring Arm.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ball_Properties")
		float f_camSpeed = 2.0f;

	/// <summary>
	/// Variable for the float value of the Jump Button's time limit to be held.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ball_Properties")
		float f_timeJumpLimit = 0.25f;

	/// <summary>
	/// Boolean to indicate whether the player is holding down the jump button or not; used to determine if ball should bounce or not (Bounce Canceling)
	/// </summary>
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Ball_Conditions")
		bool b_JumpIsHeld = false;

	/// <summary>
	/// Boolean to replicate a Do Once blueprint function so that an action inside Tick is donde once.
	/// </summary>
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Ball_Conditions")
		bool b_DoOnceFall = true;

	/// <summary>
	/// Boolean that tells the Sphere to shrink as long as the value is true.
	/// </summary>
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Ball_Conditions")
		bool b_Shrink = false;

	/// <summary>
	/// Boolean that tells the Sphere to shrink as long as the value is true.
	/// </summary>
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Ball_Conditions")
		bool b_GameOver = false;

	/// <summary>
	/// Variable for the floar value that determines the size of the Pawn in scale.
	/// </summary>
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Ball_Conditions")
		float f_ShrinkingScale = 1.0f;

	/// <summary>
	/// Pointer to the Input Action that controls Movement.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced_Input")
		UInputAction* ia_BallMove;

	/// <summary>
	/// Pointer to the Input Action that controls Jumping.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced_Input")
		UInputAction* ia_BallJump;

	/// <summary>
	/// Pointer to the Input Action that controls the Rotation of the Camera's Spring Arm.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced_Input")
		UInputAction* ia_CameraRotate;

	/// <summary>
	/// Pointer to the Input Action that controls the Pausing and Unpausing of the game.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced_Input")
		UInputAction* ia_Pause;

	/// <summary>
	/// Pointer to the Input Mapping Context that allows the Controller to manage Enhanced Inputs.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced_Input")
		UInputMappingContext* imc_BallInputMapping;

private:
	/// <summary>
	/// Function to bind to Input Action when Starting the Input.
	/// </summary>
	/// <param name="Value">Value from Input Action in Axis2D.</param>
	void BallMoveStart(const FInputActionValue& Value);

	/// <summary>
	/// Function to bind to Input Action for moving around.
	/// </summary>
	/// <param name="Value">Value from Input Action in Axis2D.</param>
	void BallMove(const FInputActionValue& Value);

	/// <summary>
	/// Function to bind to Input Action when Canceling or Completing the Input.
	/// </summary>
	/// <param name="Value">Value from Input Action in Axis2D.</param>
	void BallMoveCancel(const FInputActionValue& Value);

	/// <summary>
	/// Sets what happens when character begins its jump.
	/// </summary>
	virtual void OnJumped_Implementation() override;

	/// <summary>
	/// Defines how and when Character should bounce; if Jump Button is being held down, then it won't activate this bounce behavior (Bounce Canceling).
	/// </summary>
	void BallBounceOffGround();

	/// <summary>
	/// Action to bind to Input Action for Jumping.
	/// </summary>
	/// <param name="Value">Value from Input Action in bool signal.</param>
	void BallJumpStart(const FInputActionValue& Value);

	/// <summary>
	/// Action to bind to Input Action for Jumping.
	/// </summary>
	/// <param name="Value">Value from Input Action in bool signal.</param>
	void BallJump(const FInputActionValue& Value);

	/// <summary>
	/// Action to bind to Input Action for Canceling Jumping motion.
	/// </summary>
	/// <param name="Value">Value from Input Action in bool signal.</param>
	void BallJumpCancel(const FInputActionValue& Value);

	/// <summary>
	/// Action to bind to Input Action for rotating the camera.
	/// </summary>
	/// <param name="Value">Value from Input Action in Axis2D.</param>
	void CameraRotate(const FInputActionValue& Value);

	/// <summary>
	/// Action to bind to Input Action for pausing and unpausing game.
	/// </summary>
	/// <param name="Value"></param>
	void PauseBehavior(const FInputActionValue& Value);

public:
	/// <summary>
	/// Public function to make Game Over become true from external actors.
	/// </summary>
	void SetGameOverToTrue();

	/// <summary>
	/// For referencial and delays, gets the value of b_GameOver from public access.
	/// </summary>
	bool* GetGameOverValue() { return &b_GameOver; };

	/// <summary>
	/// Function that is called when a Game Over condition is triggered; handles all subsequent behaviors and code execution.
	/// </summary>
	/// <param name="b_WasHazard">If Pawn made contact with a Hazard, then set this as True. It's False by default.</param>
	void OnGameOver(const bool& b_WasHazard = false);

	/// <summary>
	/// Called every frame.
	/// </summary>
	/// <param name="DeltaTime">Delta Time in time dilation.</param>
	virtual void Tick(float DeltaTime) override;

	/// <summary>
	/// Called to bind functionality to input.
	/// </summary>
	/// <param name="PlayerInputComponent">Component used to bind Actions and Mapping Context to Controller.</param>
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/// <summary>
	/// Called when all Components have been initialized. Used to configure said components once they are ready.
	/// </summary>
	virtual void PostInitializeComponents() override;

	/// <summary>
	/// Gets the value of the Sphere's Speed Movement.
	/// </summary>
	/// <returns>float f_speed</returns>
	float* GetBallSpeedValue() { return &f_speed; }

	/// <summary>
	/// Sets the Character's Top Speed according to the state of where Character is landing.
	/// </summary>
	/// <param name="enum_NewSpeed">Enumerator to define top speed.</param>
	void SetBallSpeedValue(const EMovementSpeed& enum_NewSpeed);

	/// <summary>
	/// Gets the value of the Rotation Speed of the Camera's Spring Arm.
	/// </summary>
	/// <returns>float f_camSpeed</returns>
	float* GetCamSpeedValue() { return &f_camSpeed; }

	/// <summary>
	/// Sets a new value for the Rotation Speed of the Camera's Spring Arm.
	/// </summary>
	/// <param name="f_newSpeed">The float value for f_camSpeed.</param>
	void SetCamSpeedValue(float f_newSpeed) { f_camSpeed = 1.0f + f_newSpeed; }

	/// <summary>
	/// Function called by other Actors when overlapping. This keeps on impulsing the Pawn on the air until it leaves the other Actor's trigger.
	/// </summary>
	void AutoImpulseUp(const double& f_AutoImpulseUp, const FVector& v_Direction);

	/// <summary>
	/// Send signal to GameMode to play sound effect.
	/// </summary>
	void PlayFallingSound();

	/// <summary>
	/// Send signal to GameMode to play sound effect.
	/// </summary>
	void PlayFlatteningSound();

	/// <summary>
	/// Send signal to GameMode to play sound effect.
	/// </summary>
	void PlayBurstSound();

	/// <summary>
	/// Send signal to GameMode to play sound effect.
	/// </summary>
	/// <param name="b_Ground">Set true if colliding with the ground. Set false if colliding with a wall.</param>
	void PlayBounceSound(const bool& b_Ground);

	/// <summary>
	/// Function that is called when ball overlaps with the pipe goal's trigger.
	/// </summary>
	void SetGoalReach();

	/// <summary>
	/// Called by hazards to change its material's parameters.
	/// </summary>
	void BallBurstFromHazard();

	/// <summary>
	/// Event that is used to set behavior when Sphere Component overlaps with elments of the scene,
	/// such as Jumpable ground as well as Hazards.
	/// </summary>
	/// <param name="OverlappedComp"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	/// <param name="FromSweep"></param>
	/// <param name="SweepResult"></param>
	UFUNCTION()
		void OnShpereComponentOverlapWithActors(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	/// <summary>
	/// Event that is used to set behavior when Sphere Component exits an overlapping trigger with elments of the scene.
	/// </summary>
	/// <param name="OverlappedComp"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	UFUNCTION()
		void OnSphereComponentOverlapEnds(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
