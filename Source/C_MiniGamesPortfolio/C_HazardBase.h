// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "C_BallCharacter.h"
#include "C_HazardBase.generated.h"

UCLASS()
class C_MINIGAMESPORTFOLIO_API AC_HazardBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_HazardBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Hazard_Components")
		UArrowComponent* ac_ArrowPivot;

	UPROPERTY(EditAnywhere, Category = "Hazard_Components")
		UStaticMeshComponent* smc_HazardMeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Hazard_Assets")
		AC_BallCharacter* ac_BallToGameOver;

	void CallBallToGameOver();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// <summary>
	/// Called when all Components have been initialized. Used to configure said components once they are ready.
	/// </summary>
	virtual void PostInitializeComponents() override;

	/// <summary>
	/// Event that is used to set behavior when the Stacic Mesh Component makes physical contact with elments of the scene,
	/// such as hazards.
	/// </summary>
	/// <param name="OverlappedComp"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="NormalImpulse"></param>
	/// <param name="Hit"></param>
	UFUNCTION()
		void OnHazardMeshOverlaps(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

};
