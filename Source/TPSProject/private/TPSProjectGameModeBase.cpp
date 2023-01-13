// Copyright Epic Games, Inc. All Rights Reserved.


#include "TPSProjectGameModeBase.h"
#include "TPSProject.h"

ATPSProjectGameModeBase::ATPSProjectGameModeBase() 
{
	//PRINT_LOG(TEXT("Log : %s"), TEXT("Fucking"));

	ConstructorHelpers::FClassFinder<APawn> BP_PAWN_C(TEXT("Blueprint'/Game/Blueprint/BP_TPSPlayer.BP_TPSPlayer_C'"));
	//Blueprint'/Game/Blueprint/BP_TPSPlayer.BP_TPSPlayer'
	//교훈, 블루프린트를 코드에서 가르킬 때 _C, 블루프린트 내부 클래스를 가리키는 듯하다 
	if (BP_PAWN_C.Succeeded())
	{
		PRINT_LOG(TEXT("Log : %s"), TEXT("Succeeded"));
		DefaultPawnClass = BP_PAWN_C.Class;
	}
	else
	{
		PRINT_LOG(TEXT("Log : %s"), TEXT("Fucked"));
	}
}