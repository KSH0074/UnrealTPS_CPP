// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyFSM.h" // getnerated 위에 넣어야함 항상 모든 헤더 
#include "EnemyAnim.generated.h"

/**
 *
 */
UCLASS()
class TPSPROJECT_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
		EEnemyState animState; 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
		bool bAttackPlay = false;
	UFUNCTION(BlueprintCallable, Category = FSMEvent)
		void OnEndAttackAnimation();
	//피격애니메이션 재생 함수 
	UFUNCTION(BlueprintImplementableEvent, Category = FSMEvent) // BlueprintImplementableEvent => 블루프린트에서 구현
		void PlayDamageAnim(FName sectionName);

};
