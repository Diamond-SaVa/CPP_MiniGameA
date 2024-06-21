// Fill out your copyright notice in the Description page of Project Settings.

#include "C_GameInstance.h"
#include "C_BallCharacter.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"

UC_GameInstance::UC_GameInstance()
{
	
}

void UC_GameInstance::SetCamSpeedValue(const float& f_newSpeed)
{
	f_camSpeed =  FMath::Clamp(f_newSpeed, 1.0f, 3.0f);

	if (AC_BallCharacter* ballCharacter = Cast<AC_BallCharacter>(
		UGameplayStatics::GetActorOfClass(
			GetWorld(), 
			AC_BallCharacter::StaticClass()
		)
	))
	{
		ballCharacter->SetCamSpeedValue(f_camSpeed);
	}
}

