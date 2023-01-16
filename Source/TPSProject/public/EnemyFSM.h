// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

//����� Ÿ�� ����
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

	//���º���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	EEnemyState mState = EEnemyState::Idle;// ������ ���� default��, mState�� ���� ���¸� ������ ������ 

	void IdleState();
	void MoveState();
	void AttackState();
	void DamageState();
	void DieState();
	
	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float idleDelayTime = 2;

	float currentTIme = 0;
	
	//Ÿ�� = �÷��̾� 
	UPROPERTY(VisibleAnywhere, Category = FSM)
		class ATPSPlayer* target;

	//���� ����, �� ������Ʈ�� �����ϴ� ����
	UPROPERTY()
		class AEnemy* me;
	//���ݹ���
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackRange = 150.0f;
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackDelayTime = 2.0f;

	void OnDamageProcess();

	//Enemy�� ü��
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=FSM)
		int32 hp = 3;
	//�ǰݴ��ð�
	float damageDelayTime = 2.0;

	//�Ʒ��� ������� �ӵ�
	UPROPERTY(EditAnywhere, Category = FSM)
	float dieSpeed = 50.0f;

};
