// Fill out your copyright notice in the Description page of Project Settings.


#include "C_PipeGoal.h"
#include "C_BallCharacter.h"
#include "C_MiniGamesPortfolioGameModeBase.h"

// Sets default values
AC_PipeGoal::AC_PipeGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	meshC_PipeGoal = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PipeGoalMeshComponent"));

	sphereC_PipeGoalTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("PipeGoalTrigger"));

	spotLight_LightFromPipe = CreateDefaultSubobject<UPointLightComponent>(TEXT("PipeGoalLight"));

	if (meshC_PipeGoal != nullptr)
	{
		SetRootComponent(meshC_PipeGoal);

		if (sphereC_PipeGoalTrigger != nullptr)
		{
			sphereC_PipeGoalTrigger->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		}

		if (spotLight_LightFromPipe != nullptr)
		{
			spotLight_LightFromPipe->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			spotLight_LightFromPipe->SetIntensity(1500.0f);
			spotLight_LightFromPipe->SetAttenuationRadius(175.0f);
			spotLight_LightFromPipe->SetMobility(EComponentMobility::Stationary);
			spotLight_LightFromPipe->SetRelativeLocation(FVector::UpVector * 65.0f);
		}
	}
}

// Called when the game starts or when spawned
void AC_PipeGoal::BeginPlay()
{
	Super::BeginPlay();
	
}

void AC_PipeGoal::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (sphereC_PipeGoalTrigger != nullptr)
	{
		sphereC_PipeGoalTrigger->OnComponentBeginOverlap.AddDynamic(this, &AC_PipeGoal::OnPlayerBallOverlapsGoalTrigger);
	}
}

void AC_PipeGoal::OnPlayerBallOverlapsGoalTrigger(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	if (AC_BallCharacter* BallCharacter = Cast<AC_BallCharacter>(OtherActor))
	{
		sphereC_PipeGoalTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);	

		if (AC_MiniGamesPortfolioGameModeBase* MiniGameMode = Cast<AC_MiniGamesPortfolioGameModeBase>(GetWorld()->GetAuthGameMode()))
		{
			MiniGameMode->StopAndUpdateRecords();
			
			MiniGameMode->PlaySoundEffect(MiniGameMode->GOAL_REACH);
			
			MiniGameMode->ChangeBGM(MiniGameMode->VICTORY_SONG);
		}

		BallCharacter->SetGoalReach();
	}
}

// Called every frame
void AC_PipeGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

