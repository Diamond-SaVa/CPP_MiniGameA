// Fill out your copyright notice in the Description page of Project Settings.


#include "C_CollectableBase.h"
#include "C_BallCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "NiagaraFunctionLibrary.h"
#include "C_MiniGamesPortfolioGameModeBase.h"

// Sets default values
AC_CollectableBase::AC_CollectableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collectableHitbox = CreateDefaultSubobject<USphereComponent>(TEXT("CollectableHitbox"));

	collectableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollectableStaticMesh"));

	rotationManager = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotationManager"));

	niagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));

	SetRootComponent(collectableHitbox);

	collectableMesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	niagaraComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AC_CollectableBase::BeginPlay()
{
	Super::BeginPlay();

	PawnOnScene = UGameplayStatics::GetActorOfClass(GetWorld(), AC_BallCharacter::StaticClass());

	startingLocation = GetActorLocation();

	SetActorTickEnabled(false);
}

void AC_CollectableBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (collectableHitbox != nullptr)
	{
		collectableHitbox->OnComponentBeginOverlap.AddDynamic(this, &AC_CollectableBase::OnBallPawnOverlapWithCollectable);
	}

	if (collectableMesh != nullptr)
	{
		collectableMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}
}

void AC_CollectableBase::OnBallPawnOverlapWithCollectable(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	if (b_Collected == false)
	{
		if (AC_BallCharacter* BallPawn = Cast<AC_BallCharacter>(OtherActor))
		{
			if (rotationManager != nullptr)
			{
				rotationManager->RotationRate = FRotator(0.0, 540.0, 0.0);
				rotationManager->PivotTranslation.X = 250.0f;
			}

			collectableHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			if (niagaraComponent != nullptr)
			{
				niagaraComponent->SetNiagaraVariableFloat(TEXT("sphereRadious"), 16.0f);
				niagaraComponent->SetNiagaraVariableFloat(TEXT("spawnRate"), 25.0f);
			}

			RegisterOnGameMode();

			b_Collected = true;

			SetActorTickEnabled(true);
		}
	}
}

void AC_CollectableBase::OnReachedPlayerOnce()
{
	if (niagaraComponent != nullptr)
	{
		niagaraComponent->SetActive(false);
	}
	collectableMesh->SetVisibility(false);

	if (AC_MiniGamesPortfolioGameModeBase* MiniGameMode = Cast<AC_MiniGamesPortfolioGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		MiniGameMode->PlaySoundEffect(AC_MiniGamesPortfolioGameModeBase::ESoundEffectToPlay::COIN_BURST);
	}

	FTimerHandle timerHandle;
	GetWorldTimerManager().SetTimer(timerHandle, this, &AC_CollectableBase::OrderDestroyActor, 1.2f);
}

void AC_CollectableBase::OrderDestroyActor()
{
	Destroy();
}

void AC_CollectableBase::RegisterOnGameMode()
{
	if (AC_MiniGamesPortfolioGameModeBase* MiniGameMode = Cast<AC_MiniGamesPortfolioGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		MiniGameMode->UpScore();
		MiniGameMode->PlaySoundEffect(AC_MiniGamesPortfolioGameModeBase::ESoundEffectToPlay::COIN_COLLECT);
	}
}

// Called every frame
void AC_CollectableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (b_Collected == true)
	{
		if (PawnOnScene)
		{
			f_AlphaReachTotal = f_AlphaReach / f_AlphaReachLength;

			FVector NewLocation = FMath::Lerp<FVector, float>(startingLocation, PawnOnScene->GetActorLocation(), f_AlphaReachTotal);

			SetActorLocation(NewLocation);
			
			if (curve_CollectedBehaviorCurve != nullptr)
			{
				float curveFloat = curve_CollectedBehaviorCurve->GetFloatValue(f_AlphaReachTotal);

				FVector NewRelativeLocation = (FVector::RightVector + FVector::UpVector) * (curveFloat * 150.0f);

				collectableMesh->SetRelativeLocation(NewRelativeLocation);

				collectableMesh->SetRelativeScale3D(FVector::One() * (1.1f - f_AlphaReachTotal));
			}

			f_AlphaReach += DeltaTime;

			if (b_DoOnceReach == false)
			{
				if (f_AlphaReach > f_AlphaReachLength)
				{
					b_DoOnceReach = true;

					if (niagaraBurstSystem != nullptr)
						UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), niagaraBurstSystem, GetActorLocation())->SetAutoDestroy(true);

					OnReachedPlayerOnce();
				}
			}
		}
	}
}

