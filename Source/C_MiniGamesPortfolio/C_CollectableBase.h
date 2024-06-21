// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Curves/CurveFloat.h"
#include "C_CollectableBase.generated.h"


UCLASS()
class C_MINIGAMESPORTFOLIO_API AC_CollectableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_CollectableBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base_Components")
		USphereComponent* collectableHitbox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base_Components")
		UStaticMeshComponent* collectableMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base_Components")
		URotatingMovementComponent* rotationManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base_Components")
		UNiagaraComponent* niagaraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base_Components")
		UNiagaraSystem* niagaraSparkleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base_Components")
		UNiagaraSystem* niagaraBurstSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base_Components")
		AActor* PawnOnScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base_Components")
		FVector startingLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base_Components")
		UCurveFloat* curve_CollectedBehaviorCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base_Components")
		bool b_Collected = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base_Components")
		float f_AlphaReach = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base_Components")
		float f_AlphaReachLength = 2.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base_Components")
		float f_AlphaReachTotal = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base_Components")
		bool b_DoOnceReach = false;

	/// <summary>
	/// Called when all Components have been initialized. Used to configure said components once they are ready.
	/// </summary>
	virtual void PostInitializeComponents() override;

	UFUNCTION()
		void OnBallPawnOverlapWithCollectable(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	void OnReachedPlayerOnce();

	void OrderDestroyActor();

	void RegisterOnGameMode();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
