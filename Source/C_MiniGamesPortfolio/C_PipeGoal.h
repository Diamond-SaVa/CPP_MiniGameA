// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PointLightComponent.h"
#include "C_PipeGoal.generated.h"

UCLASS()
class C_MINIGAMESPORTFOLIO_API AC_PipeGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_PipeGoal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal_Pipe_Components")
		UStaticMeshComponent* meshC_PipeGoal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal_Pipe_Components")
		USphereComponent* sphereC_PipeGoalTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal_Pipe_Components")
		UPointLightComponent* spotLight_LightFromPipe;


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
		void OnPlayerBallOverlapsGoalTrigger(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
