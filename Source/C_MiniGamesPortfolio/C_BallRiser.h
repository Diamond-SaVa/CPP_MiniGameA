// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_MiniGamesPortfolioGameModeBase.h"
#include "C_BallCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "C_BallRiser.generated.h"

UCLASS()
class C_MINIGAMESPORTFOLIO_API AC_BallRiser : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_BallRiser();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// <summary>
	/// Reference to custom Game Mode
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Riser_Variables")
		AC_MiniGamesPortfolioGameModeBase* MiniGameMode;

	/// <summary>
	/// Float value of the launching speed.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Riser_Variables")
		double d_ImpulseSpeed = 100.0;

	/// <summary>
	/// Float value to set the Niagara System Parameter regarding Vortex Velocity.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Riser_Particle_Variables")
		float f_ParticleSysVortexVelocity = 100.0f;

	/// <summary>
	/// Float value to set the Niagara System Parameter regarding spawn Velocity of particles.
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Riser_Particle_Variables")
		float f_ParticleSysVelocity = 150.0f;

	/// <summary>
	/// Created component for the Riser's mesh.
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Riser_Components")
		UStaticMeshComponent* sMeshC_PlatformMeshComponent;

	/// <summary>
	/// Created component for the Riser's trigger that must overlap with character.
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Riser_Components")
		UBoxComponent* boxC_BoxTrigger;

	/// <summary>
	/// Niagara component for the particle system selected for this object.
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Riser_Components")
		UNiagaraComponent* niagaraC_RiserParticleSystem;

	/// <summary>
	/// Called when all Components have been initialized. Used to configure said components once they are ready.
	/// </summary>
	virtual void PostInitializeComponents() override;

	void CalmParticleSystem();

private:

	/// <summary>
	/// Event that is used to set behavior when Character's Component overlaps with this Riser.
	/// This allows it to launch the character, as well as manipulating Niagara Component's system behavior.
	/// </summary>
	/// <param name="OverlappedComp"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	/// <param name="FromSweep"></param>
	/// <param name="SweepResult"></param>
	UFUNCTION()
		void OnBallPawnOverlapWithRiserBegins(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
