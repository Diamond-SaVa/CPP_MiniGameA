// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BallRiser.h"

// Sets default values
AC_BallRiser::AC_BallRiser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	sMeshC_PlatformMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMeshComponent"));

	boxC_BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTriggerComponent"));

	niagaraC_RiserParticleSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraRiserParticleSystem"));

	if (sMeshC_PlatformMeshComponent != nullptr && boxC_BoxTrigger != nullptr && niagaraC_RiserParticleSystem != nullptr)
	{
		SetRootComponent(sMeshC_PlatformMeshComponent);

		boxC_BoxTrigger->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

		niagaraC_RiserParticleSystem->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}
}

// Called when the game starts or when spawned
void AC_BallRiser::BeginPlay()
{
	Super::BeginPlay();

	MiniGameMode = Cast<AC_MiniGamesPortfolioGameModeBase>(GetWorld()->GetAuthGameMode());
}

void AC_BallRiser::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (boxC_BoxTrigger != nullptr)
	{
		boxC_BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &AC_BallRiser::OnBallPawnOverlapWithRiserBegins);
	}
}

void AC_BallRiser::CalmParticleSystem()
{
	if (niagaraC_RiserParticleSystem != nullptr)
	{
		niagaraC_RiserParticleSystem->SetNiagaraVariableFloat(TEXT("vortexVelocity"), 0.0f);
		niagaraC_RiserParticleSystem->SetNiagaraVariableVec3(TEXT("startVelocity"), FVector::UpVector * f_ParticleSysVelocity);
	}
}

void AC_BallRiser::OnBallPawnOverlapWithRiserBegins(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	if (AC_BallCharacter* cast_BallSpawn = Cast<AC_BallCharacter>(OtherActor))
	{
		if (OtherComp->ComponentHasTag("POP"))
		{
			cast_BallSpawn->AutoImpulseUp(d_ImpulseSpeed, GetActorUpVector());

			if (MiniGameMode != nullptr)
			{
				MiniGameMode->PlaySoundEffectAtLocation(AC_MiniGamesPortfolioGameModeBase::RISER_WIND, GetActorLocation());
			}

			if (niagaraC_RiserParticleSystem != nullptr)
			{
				niagaraC_RiserParticleSystem->ActivateSystem();
				niagaraC_RiserParticleSystem->SetNiagaraVariableFloat(TEXT("vortexVelocity"), f_ParticleSysVortexVelocity);
				niagaraC_RiserParticleSystem->SetNiagaraVariableVec3(TEXT("startVelocity"), FVector::UpVector * (f_ParticleSysVelocity * 2.0f));

				FTimerHandle th_TimerHandle;
				GetWorldTimerManager().SetTimer(th_TimerHandle, this, &AC_BallRiser::CalmParticleSystem, 0.5f);
			}
		}
	}
}

// Called every frame
void AC_BallRiser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

