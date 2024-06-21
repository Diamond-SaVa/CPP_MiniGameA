// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "C_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class C_MINIGAMESPORTFOLIO_API AC_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AC_PlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mouse_Interface)
	UUserWidget* uWidget_Cursor;

	virtual void BeginPlay() override;

	APlayerController* Hello;
};
