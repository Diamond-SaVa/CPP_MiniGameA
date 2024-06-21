// Code created by Ricardo Sánchez Villegas.

#include "C_PlayerController.h"
#include "InputCoreTypes.h"

AC_PlayerController::AC_PlayerController()
{
	bEnableClickEvents = true;

	ClickEventKeys.Add(FKey(FName(TEXT("Enter"))));

	ClickEventKeys.Add(FKey(FName(TEXT("SpaceBar"))));

	ClickEventKeys.Add(FKey(FName(TEXT("Gamepad_FaceButton_Bottom"))));
}

void AC_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (uWidget_Cursor != nullptr)
	{
		SetMouseCursorWidget(EMouseCursor::Default, uWidget_Cursor);
	}
}
