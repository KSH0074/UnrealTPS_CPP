// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
		//플레이어의 이동속도
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
		float speed = 0;
		//플레이어의 좌우 이동속도 
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
			float direction = 0;

		//프레임마다 갱신 
		virtual void NativeUpdateAnimation(float DeltaSeconds) override;
		
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
			bool isInAir;

		UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
			class UAnimMontage* attackAnimMontage;

		void PlayAttackAnim();
};
