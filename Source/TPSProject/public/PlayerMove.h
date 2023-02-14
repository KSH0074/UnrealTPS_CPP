// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerMove.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(PlayerComponent),meta =(BlueprintSpawnableComponent))
class TPSPROJECT_API UPlayerMove : public UPlayerBaseComponent
{
	GENERATED_BODY()
public:
	UPlayerMove();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	//좌우 회전입력처리
	void Turn(float value);
	//상하 회전입력처리
	void LookUp(float value);

public:

	UPROPERTY(EditAnyWhere, Category = PlayerSetting)
		float walkSpeed = 200;
	UPROPERTY(EditAnyWhere, Category = PlayerSetting)
		float runSpeed = 600;
	FVector direction;

	void InputHorizontal(float value);
	void InputVertical(float value);
	void InputJump();
	void Move();

	void InputRun();
	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent);

	
};
