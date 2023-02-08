// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPSPlayer.h"
#include <GameFrameWork/CharacterMovementComponent.h>
#include "PlayerBaseComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSPROJECT_API UPlayerBaseComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UPlayerBaseComponent()
	{
		//필요시에 자식클래스에서 수정, Tick함수가 호출되지 않도록 처리  
		PrimaryComponentTick.bCanEverTick = false;
	};

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	
public:	
	//사용자 입력 매핑 처리함수 
	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) {};

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY()
	 ATPSPlayer* me;

	UPROPERTY()
	 UCharacterMovementComponent* moveComp;	
};
