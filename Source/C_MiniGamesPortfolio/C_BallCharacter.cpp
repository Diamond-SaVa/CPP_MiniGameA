// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BallCharacter.h"
#include "C_PlayerController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "C_FallCamera.h"
#include "C_GameInstance.h"
#include "Components/CapsuleComponent.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AC_BallCharacter::AC_BallCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*Create all necesary components*/
	smc_BallMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMeshComponent"));

	sac_CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));

	cc_CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	sc_SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

	audioC_BallAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("JumpAudioComponent"));

	/*Sets audio component to not activate on begin play.*/
	if (audioC_BallAudio != nullptr)
	{
		audioC_BallAudio->SetAutoActivate(false);
	}

	/*Attach created mesh component to root, while sphere component to mesh component.*/
	if (smc_BallMeshComp != nullptr)
	{
		smc_BallMeshComp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

		smc_BallMeshComp->ComponentTags.Add("POP");

		smc_BallMeshComp->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);

		if (sc_SphereComp != nullptr)
		{
			sc_SphereComp->AttachToComponent(smc_BallMeshComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));

			sc_SphereComp->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
		}
	}

	/*Set Camera and Arm attachments and parameters, such as the length between Camera and Character.*/
	if (sac_CameraArm != nullptr && cc_CameraComp != nullptr)
	{
		sac_CameraArm->AttachToComponent(smc_BallMeshComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));

		cc_CameraComp->AttachToComponent(sac_CameraArm, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));

		sac_CameraArm->bInheritPitch = false;
		sac_CameraArm->bInheritRoll = false;
		sac_CameraArm->bInheritYaw = false;

		sac_CameraArm->TargetArmLength = 400.0f;
	}

	JumpMaxHoldTime = 0.2f;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	GetCapsuleComponent()->SetCapsuleHalfHeight(40.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoReceiveInput = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AC_BallCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetBallSpeedValue(EMovementSpeed::NORMAL_SPEED);

	MiniGameMode = Cast<AC_MiniGamesPortfolioGameModeBase>(GetWorld()->GetAuthGameMode());

	if (UC_GameInstance* gInstance = GetWorld()->GetGameInstanceChecked<UC_GameInstance>())
	{
		if (AC_PlayerController* pc_Controller = Cast<AC_PlayerController>(GetController()))
		{
			pc_Controller->Possess(this);
			pc_Controller->ClientSetCameraFade_Implementation(true, FColor::Black, FVector2D(1.0, 0.0), 3.0f, false, true);
		}
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, TEXT("BEGIN PLAY CALLED"));
	}

	if (MiniGameMode != nullptr)
	{
		if (audioC_BallAudio != nullptr)
		{
			audioC_BallAudio->SetVolumeMultiplier(*MiniGameMode->GetSFXVolumeValue());
		}

		SetCamSpeedValue(MiniGameMode->GetCamSpeedValue());

		MiniGameMode->StartingProcedures();
	}
}

void AC_BallCharacter::BeginDestroy()
{
	Super::BeginDestroy();
}

void AC_BallCharacter::OnGameOver(const bool& b_WasHazard)
{
	SetGameOverToTrue();

	if (mid_BallMatDynamic != nullptr)
	{
		mid_BallMatDynamic->SetScalarParameterValue(FName(TEXT("Damaged")), 0.0f);
	}

	if (AActor* a_FallCamera = UGameplayStatics::GetActorOfClass(GetWorld(), AC_FallCamera::StaticClass()))
	{
		if (AC_FallCamera* ac_FallCamera = Cast<AC_FallCamera>(a_FallCamera))
		{
			ac_FallCamera->SetCameraLocation(GetActorLocation());

			ac_FallCamera->StartFadeToReloadLevel();

			if (AC_PlayerController* pc_Controller = Cast<AC_PlayerController>(GetController()))
			{
				if (b_WasHazard == false)
				{
					pc_Controller->SetViewTargetWithBlend(ac_FallCamera, 0.75f, EViewTargetBlendFunction::VTBlend_EaseIn, 2.0f, true);
					PlayFallingSound();
				}
				else
				{
					pc_Controller->SetViewTargetWithBlend(ac_FallCamera, 0.75f, EViewTargetBlendFunction::VTBlend_EaseIn, 2.0f, true);
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ns_SmokeSys, GetActorLocation(), FRotator(0.0));
					PlayFlatteningSound();
				}
			}
		}
	}

	b_Shrink = true;
	b_DoOnceFall = false;
}

// Called every frame
void AC_BallCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (b_DoOnceFall == true)
	{
		if (GetActorLocation().Z < -250.0f)
		{
			OnGameOver();
		}
	}

	if (b_Shrink == true)
	{
		if (f_ShrinkingScale > 0.1f)
		{
			f_ShrinkingScale -= DeltaTime;

			SetActorScale3D(FVector(f_ShrinkingScale));
		}
		else
		{
			SetActorHiddenInGame(true);
			b_Shrink = false;
		}
	}

	if (b_GameOver == false)
	{
		if (f_InputTimer < 2.0f)
		{
			f_InputTimer += DeltaTime;

			if (curveF_SmoothingCurve != nullptr)
			{
				float f_Curve = curveF_SmoothingCurve->GetFloatValue(f_InputTimer);

				f_speed = FMath::Lerp<float, float>(f_maxSpeed, 0.0f, f_Curve);

				GetCharacterMovement()->MaxWalkSpeed = f_speed;

				AddMovementInput(v_MoveVect, 1.0f, false);
			}
		}
	}
}

// Called to bind functionality to input
void AC_BallCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (AC_PlayerController* PC = Cast<AC_PlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* sub = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			sub->ClearAllMappings();
			sub->AddMappingContext(imc_BallInputMapping, 0);
		}
	}

	if (UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		PEI->BindAction(ia_BallMove, ETriggerEvent::Started, this, &AC_BallCharacter::BallMoveStart);
		PEI->BindAction(ia_BallMove, ETriggerEvent::Triggered, this, &AC_BallCharacter::BallMove);
		PEI->BindAction(ia_BallMove, ETriggerEvent::Completed, this, &AC_BallCharacter::BallMoveCancel);
		PEI->BindAction(ia_BallMove, ETriggerEvent::Canceled, this, &AC_BallCharacter::BallMoveCancel);
		PEI->BindAction(ia_CameraRotate, ETriggerEvent::Triggered, this, &AC_BallCharacter::CameraRotate);
		PEI->BindAction(ia_BallJump, ETriggerEvent::Started, this, &AC_BallCharacter::BallJumpStart);
		PEI->BindAction(ia_BallJump, ETriggerEvent::Triggered, this, &AC_BallCharacter::BallJump);
		PEI->BindAction(ia_BallJump, ETriggerEvent::Completed, this, &AC_BallCharacter::BallJumpCancel);
		PEI->BindAction(ia_BallJump, ETriggerEvent::Canceled, this, &AC_BallCharacter::BallJumpCancel);
		PEI->BindAction(ia_Pause, ETriggerEvent::Started, this, &AC_BallCharacter::PauseBehavior);
	}
}

void AC_BallCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (mi_BallMaterial != nullptr)
	{
		mid_BallMatDynamic = UMaterialInstanceDynamic::Create(mi_BallMaterial, nullptr, FName(TEXT("BALL_MATERIAL")));
	}

	if (smc_BallMeshComp != nullptr && mid_BallMatDynamic != nullptr)
	{
		smc_BallMeshComp->SetMaterial(0, mid_BallMatDynamic);
	}

	if (sc_SphereComp != nullptr)
	{
		sc_SphereComp->SetSphereRadius(55.0f, true);

		sc_SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AC_BallCharacter::OnShpereComponentOverlapWithActors);

		sc_SphereComp->OnComponentEndOverlap.AddDynamic(this, &AC_BallCharacter::OnSphereComponentOverlapEnds);
	}

	if (audioC_BallAudio != nullptr)
	{
		audioC_BallAudio->SetAutoActivate(false);
	}
}

void AC_BallCharacter::SetBallSpeedValue(const EMovementSpeed& enum_NewSpeed)
{
	switch (enum_NewSpeed)
	{
	case EMovementSpeed::NORMAL_SPEED:
		f_maxSpeed = f_normalSpeed;
		break;

	case EMovementSpeed::BOOST_SPEED:
		f_maxSpeed = f_boostSpeed;
		break;

	case EMovementSpeed::MUD_SPEED:
		f_maxSpeed = f_mudSpeed;
		break;

	case EMovementSpeed::STOP_SPEED:
		f_maxSpeed = 0.0f;
		break;
	}

	if (f_speed > f_maxSpeed)
	{
		f_speed = f_maxSpeed;

		GetCharacterMovement()->MaxWalkSpeed = f_speed;
	}
}

void AC_BallCharacter::AutoImpulseUp(const double& f_AutoImpulseUp, const FVector& v_Direction)
{
	FVector v_TotalForce = v_Direction * f_AutoImpulseUp;

	LaunchCharacter(v_TotalForce, true, true);
}

void AC_BallCharacter::PlayFallingSound()
{
	if (MiniGameMode != nullptr)
	{
		MiniGameMode->PlaySoundEffect(AC_MiniGamesPortfolioGameModeBase::BALL_FALLING);
	}
}

void AC_BallCharacter::PlayFlatteningSound()
{
	if (MiniGameMode != nullptr)
	{
		MiniGameMode->PlaySoundEffect(AC_MiniGamesPortfolioGameModeBase::ESoundEffectToPlay::BALL_FLATTENING);
	}
}

void AC_BallCharacter::PlayBurstSound()
{
	if (MiniGameMode != nullptr)
	{
		MiniGameMode->PlaySoundEffect(AC_MiniGamesPortfolioGameModeBase::ESoundEffectToPlay::BALL_BURST);
	}
}

void AC_BallCharacter::PlayBounceSound(const bool& b_Ground)
{
	if (b_Ground == true)
	{
		if (MiniGameMode != nullptr)
		{
			MiniGameMode->PlaySoundEffect(AC_MiniGamesPortfolioGameModeBase::ESoundEffectToPlay::BALL_BOUNCE_GROUND);
		}
	}
	else
	{
		if (MiniGameMode != nullptr)
		{
			MiniGameMode->PlaySoundEffect(AC_MiniGamesPortfolioGameModeBase::ESoundEffectToPlay::BALL_BOUNCE_WALL);
		}
	}
}

void AC_BallCharacter::OnShpereComponentOverlapWithActors(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		if (OtherActor->ActorHasTag(FName(TEXT("Ground"))))
		{
			PlayBounceSound(true);

			SetBallSpeedValue(EMovementSpeed::NORMAL_SPEED);

			BallBounceOffGround();
		}
		else if (OtherActor->ActorHasTag(FName(TEXT("Wall"))))
		{
			PlayBounceSound(false);
		}

		if (OtherActor->ActorHasTag(FName(TEXT("Boost"))))
		{
			SetBallSpeedValue(EMovementSpeed::BOOST_SPEED);
		}
	}
}

void AC_BallCharacter::OnSphereComponentOverlapEnds(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != nullptr)
	{
		
	}
}

void AC_BallCharacter::BallMoveStart(const FInputActionValue& Value)
{
	f_InputTimer = 2.0f;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Emerald, TEXT("MOVE"));
	}
}

void AC_BallCharacter::BallMove(const FInputActionValue& Value)
{
	if (b_GameOver == false)
	{
		const FInputActionValue::Axis2D v2_Move = *GetBallSpeedValue() * Value.Get<FInputActionValue::Axis2D>();

		if (sac_CameraArm != nullptr && cc_CameraComp != nullptr)
		{
			FVector v_FwdVect = cc_CameraComp->GetForwardVector();
			v_FwdVect.Z = 0.0;
			v_FwdVect.Normalize();

			v_MoveVect = (v2_Move.X * v_FwdVect) + (v2_Move.Y * cc_CameraComp->GetRightVector());

			if (f_speed < f_maxSpeed)
			{
				f_speed += f_maxSpeed * (UGameplayStatics::GetWorldDeltaSeconds(GetWorld()) * 0.75);

				GetCharacterMovement()->MaxWalkSpeed = f_speed;
			}

			AddMovementInput(v_MoveVect, 1.0f, false);
		}
	}
}

void AC_BallCharacter::BallMoveCancel(const FInputActionValue& Value)
{
	f_InputTimer = 0.0f;
}

void AC_BallCharacter::OnJumped_Implementation()
{
	Super::OnJumped_Implementation();

	if (audioC_BallAudio != nullptr)
	{
		audioC_BallAudio->Play();
	}
}

void AC_BallCharacter::BallBounceOffGround()
{
	if (b_JumpIsHeld == false)
	{
		double absVelZ = FMath::Abs<float>(GetVelocity().Z);
		double bounceForce = absVelZ / 2.0;

		if (bounceForce > 100.0)
		{
			LaunchCharacter(FVector::UpVector * bounceForce, false, true);

			FVector2D v2_velocityXY = FVector2D(GetVelocity().X, GetVelocity().Y);
		}
	}
}

void AC_BallCharacter::BallJumpStart(const FInputActionValue& Value)
{
	b_JumpIsHeld = true;
}

void AC_BallCharacter::BallJump(const FInputActionValue& Value)
{
	if (b_GameOver == false)
	{
		Jump();
	}
}

void AC_BallCharacter::BallJumpCancel(const FInputActionValue& Value)
{
	b_JumpIsHeld = false;

	if (b_GameOver == false)
	{
		StopJumping();
	}

	if (audioC_BallAudio != nullptr)
	{
		audioC_BallAudio->Stop();
	}
}

void AC_BallCharacter::CameraRotate(const FInputActionValue& Value)
{
	if (b_GameOver == false)
	{
		const FInputActionValue::Axis2D v2_Rotate = *GetCamSpeedValue() * Value.Get<FInputActionValue::Axis2D>();

		if (sac_CameraArm != nullptr)
		{
			FRotator rot_Value = FRotator(v2_Rotate.Y, v2_Rotate.X, 0.0);

			sac_CameraArm->AddLocalRotation(rot_Value);

			FRotator rot_Arm = sac_CameraArm->GetRelativeRotation();

			double pitch = rot_Arm.Pitch;

			if (pitch > 30.0)
			{
				pitch = 30.0;
			}

			if (pitch < -55.0)
			{
				pitch = -55.0;
			}

			sac_CameraArm->SetRelativeRotation(FRotator(pitch, rot_Arm.Yaw, 0.0));
		}
	}
}

void AC_BallCharacter::PauseBehavior(const FInputActionValue& Value)
{
	if (b_GameOver == false)
	{
		if (MiniGameMode != nullptr)
		{
			MiniGameMode->PauseGame(!MiniGameMode->GetGameIsPaused());
		}
	}
}

void AC_BallCharacter::SetGoalReach()
{
	SetGameOverToTrue();

	if (AActor* a_FallCamera = UGameplayStatics::GetActorOfClass(GetWorld(), AC_FallCamera::StaticClass()))
	{
		if (AC_FallCamera* ac_FallCamera = Cast<AC_FallCamera>(a_FallCamera))
		{
			ac_FallCamera->SetCameraLocation(GetActorLocation());

			ac_FallCamera->StartFadeLevelVictory();

			if (AC_PlayerController* pc_Controller = Cast<AC_PlayerController>(GetController()))
			{
				pc_Controller->SetViewTargetWithBlend(ac_FallCamera, 0.75f, EViewTargetBlendFunction::VTBlend_EaseIn, 2.0f, true);
				PlayFallingSound();
			}
		}
	}

	b_Shrink = true;
	b_DoOnceFall = false;
}

void AC_BallCharacter::BallBurstFromHazard()
{
	if (mid_BallMatDynamic != nullptr)
	{
		mid_BallMatDynamic->SetScalarParameterValue(FName(TEXT("IsBurst")), 1.0f);
		mid_BallMatDynamic->SetScalarParameterValue(FName(TEXT("Damaged")), 1.0f);
	}
}

void AC_BallCharacter::SetGameOverToTrue()
{
	b_GameOver = true;

	if (smc_BallMeshComp != nullptr && sc_SphereComp != nullptr)
	{
		smc_BallMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		sc_SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	SetBallSpeedValue(EMovementSpeed::STOP_SPEED);

	GetCharacterMovement()->GravityScale = 0.0;

	GetCharacterMovement()->Velocity = FVector::Zero();
}
