// Fill out your copyright notice in the Description page of Project Settings.


#include "C_HazardBase.h"

// Sets default values
AC_HazardBase::AC_HazardBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ac_ArrowPivot = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowPivot"));

	SetRootComponent(ac_ArrowPivot);

	smc_HazardMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hazard_StaticMeshComponent"));

	if (smc_HazardMeshComp != nullptr)
	{
		smc_HazardMeshComp->AttachToComponent(ac_ArrowPivot, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

// Called when the game starts or when spawned
void AC_HazardBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AC_HazardBase::CallBallToGameOver()
{
	if (ac_BallToGameOver != nullptr) {
		ac_BallToGameOver->OnGameOver(true);
	}
}

// Called every frame
void AC_HazardBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AC_HazardBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (smc_HazardMeshComp != nullptr)
	{
		smc_HazardMeshComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);

		smc_HazardMeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

		smc_HazardMeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

		smc_HazardMeshComp->SetGenerateOverlapEvents(true);

		smc_HazardMeshComp->OnComponentBeginOverlap.AddDynamic(this, &AC_HazardBase::OnHazardMeshOverlaps);
	}
}

void AC_HazardBase::OnHazardMeshOverlaps(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag("POP") == true)
	{
		if (AC_BallCharacter* ballPawn = Cast<AC_BallCharacter>(OtherActor))
		{
			if (*ballPawn->GetGameOverValue() != true)
			{
				ballPawn->SetGameOverToTrue();
				ballPawn->BallBurstFromHazard();
				ballPawn->PlayBurstSound();
				ac_BallToGameOver = ballPawn;
				FTimerHandle th_TimerHandle;
				GetWorldTimerManager().SetTimer(th_TimerHandle, this, &AC_HazardBase::CallBallToGameOver, 0.2f);
			}
		}
	}
}

