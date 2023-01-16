// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

//사용할 타입 정의
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle, 
	Move, 
	Attack, 
	Damage, 
	Die,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSPROJECT_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//상태변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	EEnemyState mState = EEnemyState::Idle;// 대기상태 지정 default임, mState는 현재 상태를 저장할 변수임 

	void IdleState();
	void MoveState();
	void AttackState();
	void DamageState();
	void DieState();
	
	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float idleDelayTime = 2;

	float currentTIme = 0;
	
	//타겟 = 플레이어 
	UPROPERTY(VisibleAnywhere, Category = FSM)
		class ATPSPlayer* target;

	//소유 액터, 이 컴포넌트를 소유하는 액터
	UPROPERTY()
		class AEnemy* me;
	//공격범위
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackRange = 150.0f;
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackDelayTime = 2.0f;

	void OnDamageProcess();

	//Enemy의 체력
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=FSM)
		int32 hp = 3;
	//피격대기시간
	float damageDelayTime = 2.0;

	//아래로 사라지는 속도
	UPROPERTY(EditAnywhere, Category = FSM)
	float dieSpeed = 50.0f;

};
