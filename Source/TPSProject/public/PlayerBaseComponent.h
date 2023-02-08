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
		//�ʿ�ÿ� �ڽ�Ŭ�������� ����, Tick�Լ��� ȣ����� �ʵ��� ó��  
		PrimaryComponentTick.bCanEverTick = false;
	};

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	
public:	
	//����� �Է� ���� ó���Լ� 
	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) {};

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY()
	 ATPSPlayer* me;

	UPROPERTY()
	 UCharacterMovementComponent* moveComp;	
};
