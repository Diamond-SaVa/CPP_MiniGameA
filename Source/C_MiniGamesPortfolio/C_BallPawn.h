// DEPRICATED CLASS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "InputActionValue.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "C_BallPawn.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class C_MINIGAMESPORTFOLIO_API AC_BallPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AC_BallPawn();

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
	/// Function automatically called when Instance of Object is destroyed.
	/// </summary>
	virtual void BeginDestroy() override;

protected:
	/// <summary>
	/// Called when the game starts or when spawned
	/// </summary>
	virtual void BeginPlay() override;

	/// <summary>
	/// Pointer to the Static Mesh where the sphere must go.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ball_Components")
		UStaticMeshComponent* smc_BallMeshComp;

	/// <summary>
	/// Pointer to the material to make a Dynamic Material Instance from.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ball_Components")
		UMaterialInterface* mint_BallMat;

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
	/// Sound to be played when being flattened by a hazard.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ball_Assets")
		USoundCue* soundB_Jump;

	/// <summary>
	/// Variable for the float value of Speed of the Sphere's Impulse.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ball_Properties")
		float f_speed = 20.0f;

	/// <summary>
	/// Variable for the float value of the Rotation Speed of the Camera's Spring Arm.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ball_Properties")
		float f_camSpeed = 3.0f;

	/// <summary>
	/// Variable for the float value of the Sphere's Jumping Strength.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ball_Properties")
		float f_jump = 35.0f;

	/// <summary>
	/// Variable for the float value of the Jump Button's time limit to be held.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ball_Properties")
		float f_timeJumpLimit = 0.25f;

	/// <summary>
	/// Variable for the float value of the timer of the duration of the Jump, which should increase by Delta Seconds while the Input Action is being triggered.
	/// </summary>
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Ball_Properties")
		float f_timeJump = 0.0;

	/// <summary>
	/// Boolean to replicate a Do Once blueprint function so that the Jump Audio Component is only played once during the Jump Action Triggering.
	/// </summary>
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Ball_Conditions")
		bool b_DoOnceJump = true;

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
	/// Pointer to the Input Mapping Context that allows the Controller to manage Enhanced Inputs.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advanced_Input")
		UInputMappingContext* imc_BallInputMapping;

public:

	void PlayFallingSound();

	void PlayFlatteningSound();

	void PlayBurstSound();

	void PlayBounceSound(const bool& b_Ground);

	/// <summary>
	/// Public function to make Game Over become true from external actors.
	/// </summary>
	void SetGameOverToTrue() { b_GameOver = true; }

	/// <summary>
	/// For referencial and delays, gets the value of b_GameOver from public access.
	/// </summary>
	bool* GetGameOverValue() { return &b_GameOver; };

	/// <summary>
	/// Function that controls Game Over behavior.
	/// </summary>
	/// <param name="b_WasHazard">If Pawn made contact with a Hazard, then set this as True. It's False by default.</param>
	void OnGameOver(const bool& b_WasHazard);

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
	/// Sets a new value for the Sphere's Speed Movement.
	/// </summary>
	/// <param name="f_newSpeed">The float value for f_speed.</param>
	void SetBallSpeedValue(float f_newSpeed) { f_speed = f_newSpeed; }

	/// <summary>
	/// Gets the value of the Rotation Speed of the Camera's Spring Arm.
	/// </summary>
	/// <returns>float f_camSpeed</returns>
	float* GetCamSpeedValue() { return &f_camSpeed; }

	/// <summary>
	/// Sets a new value for the Rotation Speed of the Camera's Spring Arm.
	/// </summary>
	/// <param name="f_newSpeed">The float value for f_camSpeed.</param>
	void SetCamSpeedValue(float f_newSpeed) { f_camSpeed = f_newSpeed; }

	/// <summary>
	/// Gets the value of the Jump Strength.
	/// </summary>
	/// <returns>float f_jump</returns>
	float* GetBallJumpValue() { return &f_jump; }

	/// <summary>
	/// Sets the new value of the Jump Strength.
	/// </summary>
	/// <param name="f_newJump">The float value for f_jump.</param>
	void SetBallJumpValue(float f_newJump) { f_jump = f_newJump; }

	/// <summary>
	/// Function called by other Actors when overlapping. This keeps on impulsing the Pawn on the air until it leaves the other Actor's trigger.
	/// </summary>
	void AutoImpulseUp(const float& f_AutoImpulseUp, const FVector& v_Direction);

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
