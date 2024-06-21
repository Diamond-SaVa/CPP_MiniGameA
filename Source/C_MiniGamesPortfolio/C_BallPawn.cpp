#include "C_BallPawn.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "C_FallCamera.h"
#include "C_MiniGamesPortfolioGameModeBase.h"


// Sets default values
AC_BallPawn::AC_BallPawn()
{
	// Set this pawn to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	smc_BallMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMeshComponent"));

	sac_CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	
	cc_CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	sc_SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

	audioC_BallAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("JumpAudioComponent"));

	if (audioC_BallAudio != nullptr)
	{
		audioC_BallAudio->SetAutoActivate(false);
	}

	if (smc_BallMeshComp != nullptr)
	{
		SetRootComponent(smc_BallMeshComp);
		
		smc_BallMeshComp->ComponentTags.Add("POP");

		smc_BallMeshComp->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);

		if (sc_SphereComp != nullptr)
		{
			sc_SphereComp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));

			sc_SphereComp->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
		}
	}
	
	if (sac_CameraArm != nullptr && cc_CameraComp != nullptr)
	{
		sac_CameraArm->AttachToComponent(GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));

		cc_CameraComp->AttachToComponent(sac_CameraArm, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));

		sac_CameraArm->bInheritPitch = false;
		sac_CameraArm->bInheritRoll = false;
		sac_CameraArm->bInheritYaw = false;
	}
	
	
}

void AC_BallPawn::BallMoveStart(const FInputActionValue& Value)
{
	smc_BallMeshComp->SetAngularDamping(2.5f);
}

void AC_BallPawn::BallMove(const FInputActionValue& Value)
{
	const FInputActionValue::Axis2D v2_Move = *GetBallSpeedValue() * Value.Get<FInputActionValue::Axis2D>();

	if (smc_BallMeshComp != nullptr && sac_CameraArm != nullptr)
	{
		if (smc_BallMeshComp->IsSimulatingPhysics())
		{
			FVector v_FwdVect = cc_CameraComp->GetForwardVector();
			v_FwdVect.Z = 0.0;
			v_FwdVect.Normalize();

			FVector v_MoveVect = (v2_Move.X * v_FwdVect) + (v2_Move.Y * cc_CameraComp->GetRightVector());

			smc_BallMeshComp->AddImpulse(v_MoveVect, NAME_None, true);
		}
	}
}

void AC_BallPawn::BallMoveCancel(const FInputActionValue& Value)
{
	smc_BallMeshComp->SetAngularDamping(5.0f);
}

void AC_BallPawn::BallJump(const FInputActionValue& Value)
{
	if (smc_BallMeshComp != nullptr)
	{
		if (f_timeJump < f_timeJumpLimit)
		{
			if (b_DoOnceJump == true)
			{
				if (audioC_BallAudio != nullptr)
				{
					SetBallSpeedValue(10.0f);
					audioC_BallAudio->Play();
					b_DoOnceJump = false;
				}
			}

			if (smc_BallMeshComp->IsSimulatingPhysics())
			{
				f_timeJump += UGameplayStatics::GetWorldDeltaSeconds(this);
				smc_BallMeshComp->AddImpulse(*GetBallJumpValue() * FVector::UpVector, NAME_None, true);
			}
		}
	}
}

void AC_BallPawn::BallJumpCancel(const FInputActionValue& Value)
{
	if (smc_BallMeshComp != nullptr)
	{
		if (smc_BallMeshComp->IsSimulatingPhysics())
		{
			f_timeJump = 1.0f;

			smc_BallMeshComp->AddImpulse(FVector::Zero(), NAME_None, true);

			if (audioC_BallAudio != nullptr)
			{
				audioC_BallAudio->Stop();
			}
		}
	}
}

void AC_BallPawn::CameraRotate(const FInputActionValue& Value)
{
	if (smc_BallMeshComp != nullptr)
	{
		if (smc_BallMeshComp->IsSimulatingPhysics() == true)
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
}

void AC_BallPawn::BeginDestroy()
{
	Super::BeginDestroy();
}

// Called when the game starts or when spawned
void AC_BallPawn::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* pc_Controller = Cast<APlayerController>(GetController()))
	{
		pc_Controller->ClientSetCameraFade_Implementation(true, FColor::Black, FVector2D(1.0, 0.0), 3.0f, false, true);
	}

	if (smc_BallMeshComp != nullptr && mid_BallMatDynamic != nullptr)
	{
		smc_BallMeshComp->SetMaterial(0, mid_BallMatDynamic);
	}
}

void AC_BallPawn::PlayFallingSound()
{
	// Play from GameMode
	if (AC_MiniGamesPortfolioGameModeBase* MiniGameMode = Cast<AC_MiniGamesPortfolioGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		MiniGameMode->PlaySoundEffect(AC_MiniGamesPortfolioGameModeBase::ESoundEffectToPlay::BALL_FALLING);
	}
}

void AC_BallPawn::PlayFlatteningSound()
{
	// Play from GameMode
	if (AC_MiniGamesPortfolioGameModeBase* MiniGameMode = Cast<AC_MiniGamesPortfolioGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		MiniGameMode->PlaySoundEffect(AC_MiniGamesPortfolioGameModeBase::ESoundEffectToPlay::BALL_FLATTENING);
	}
}

void AC_BallPawn::PlayBurstSound()
{
	// Play from GameMode
	if (AC_MiniGamesPortfolioGameModeBase* MiniGameMode = Cast<AC_MiniGamesPortfolioGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		MiniGameMode->PlaySoundEffect(AC_MiniGamesPortfolioGameModeBase::ESoundEffectToPlay::BALL_BURST);
	}
}

void AC_BallPawn::PlayBounceSound(const bool& b_Ground)
{
	if (b_Ground == true)
	{
		// Play from GameMode
		if (AC_MiniGamesPortfolioGameModeBase* MiniGameMode = Cast<AC_MiniGamesPortfolioGameModeBase>(GetWorld()->GetAuthGameMode()))
		{
			MiniGameMode->PlaySoundEffect(AC_MiniGamesPortfolioGameModeBase::ESoundEffectToPlay::BALL_BOUNCE_GROUND);
		}
	}
	else
	{
		// Play from GameMode
		if (AC_MiniGamesPortfolioGameModeBase* MiniGameMode = Cast<AC_MiniGamesPortfolioGameModeBase>(GetWorld()->GetAuthGameMode()))
		{
			MiniGameMode->PlaySoundEffect(AC_MiniGamesPortfolioGameModeBase::ESoundEffectToPlay::BALL_BOUNCE_WALL);
		}
	}
}

void AC_BallPawn::OnGameOver(const bool& b_WasHazard = false)
{
	if (smc_BallMeshComp)
	{
		smc_BallMeshComp->SetSimulatePhysics(false);
	}

	if (AActor* a_FallCamera = UGameplayStatics::GetActorOfClass(GetWorld(), AC_FallCamera::StaticClass()))
	{
		if (AC_FallCamera* ac_FallCamera = Cast<AC_FallCamera>(a_FallCamera))
		{
			ac_FallCamera->SetCameraLocation(GetActorLocation());

			if (APlayerController* pc_Controller = Cast<APlayerController>(GetController()))
			{
				if (b_WasHazard == false)
				{
					pc_Controller->SetViewTargetWithBlend(ac_FallCamera, 0.75f, EViewTargetBlendFunction::VTBlend_EaseIn, 2.0f, true);
					PlayFallingSound();
				}
				else
				{
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ns_SmokeSys, GetActorLocation(), FRotator(0.0));
					PlayFlatteningSound();
				}

				pc_Controller->ClientSetCameraFade_Implementation(true, FColor::Black, FVector2D(0.0, 1.0), 2.0f, true, true);
			}
		}
	}

	b_Shrink = true;
	b_DoOnceFall = false;
}



// Called every frame
void AC_BallPawn::Tick(float DeltaTime)
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
}

// Called to bind functionality to input
void AC_BallPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* sub = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			sub->ClearAllMappings();
			sub->AddMappingContext(imc_BallInputMapping, 0);
		}
	}

	if (UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		PEI->BindAction(ia_BallMove, ETriggerEvent::Started, this, &AC_BallPawn::BallMoveStart);
		PEI->BindAction(ia_BallMove, ETriggerEvent::Triggered, this, &AC_BallPawn::BallMove);
		PEI->BindAction(ia_BallMove, ETriggerEvent::Completed, this, &AC_BallPawn::BallMoveCancel);
		PEI->BindAction(ia_BallMove, ETriggerEvent::Canceled, this, &AC_BallPawn::BallMoveCancel);
		PEI->BindAction(ia_CameraRotate, ETriggerEvent::Triggered, this, &AC_BallPawn::CameraRotate);
		PEI->BindAction(ia_BallJump, ETriggerEvent::Triggered, this, &AC_BallPawn::BallJump);
		PEI->BindAction(ia_BallJump, ETriggerEvent::Completed, this, &AC_BallPawn::BallJumpCancel);
		PEI->BindAction(ia_BallJump, ETriggerEvent::Canceled, this, &AC_BallPawn::BallJumpCancel);
	}
}

void AC_BallPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (smc_BallMeshComp != nullptr)
	{
		mid_BallMatDynamic = UMaterialInstanceDynamic::Create(mint_BallMat, nullptr);

		if (mid_BallMatDynamic != nullptr)
		{
			smc_BallMeshComp->SetMaterial(0, mid_BallMatDynamic);
		}

		smc_BallMeshComp->SetSimulatePhysics(true);
	}

	if (sac_CameraArm != nullptr)
	{
		sac_CameraArm->TargetArmLength = 400.0f;
	}

	if (sc_SphereComp != nullptr)
	{
		sc_SphereComp->SetSphereRadius(53.0f, true);

		sc_SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AC_BallPawn::OnShpereComponentOverlapWithActors);

		sc_SphereComp->OnComponentEndOverlap.AddDynamic(this, &AC_BallPawn::OnSphereComponentOverlapEnds);
	}

	if (audioC_BallAudio != nullptr && soundB_Jump != nullptr)
	{
		audioC_BallAudio->SetSound(soundB_Jump);
		audioC_BallAudio->SetAutoActivate(false);
	}
}

void AC_BallPawn::AutoImpulseUp(const float &f_AutoImpulseUp, const FVector &v_Direction)
{
	if (smc_BallMeshComp != nullptr)
	{
		if (smc_BallMeshComp->IsSimulatingPhysics() == true)
		{
			smc_BallMeshComp->AddImpulse(v_Direction * f_AutoImpulseUp, NAME_None, true);
		}
	}
}

void AC_BallPawn::OnShpereComponentOverlapWithActors(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		if (OtherActor->ActorHasTag(FName(TEXT("Ground"))))
		{
			PlayBounceSound(true);

			SetBallSpeedValue(15.0f);

			if (f_timeJump != 0.0f)
			{
				f_timeJump = 0.0f;
				b_DoOnceJump = true;
			}
		}
		else if (OtherActor->ActorHasTag(FName(TEXT("Wall"))))
		{
			PlayBounceSound(false);
		}
		
		if (OtherActor->ActorHasTag(FName(TEXT("Boost"))))
		{
			SetBallSpeedValue(20.0f);
		}
	}
	
}

void AC_BallPawn::OnSphereComponentOverlapEnds(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != nullptr)
	{
		// SetBallSpeedValue(5.0f);
	}
}

