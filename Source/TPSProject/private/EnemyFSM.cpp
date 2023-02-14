// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "Enemy.h"
#include "TPSPlayer.h"
#include <Kismet/GameplayStatics.h>
#include "TPSProject.h"
#include <Components/CapsuleComponent.h>
#include "EnemyAnim.h"
#include <AIController.h>
#include <NavigationSystem.h>
// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();
	 AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass()); // ATPSPlayerŸ�� ���� ã�� 
	 target = Cast<ATPSPlayer>(actor); // actor �� target���� ����ȯ GetActorOf~~�� AActor* Ÿ������ ��ȯ�Ѱ� 

	 me = Cast<AEnemy>(GetOwner());
	 
	 //anim �� ������� �ִϸ��̼� �������Ʈ  �Ҵ�,  
	 anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());
	 //AAIController�� �Ҵ�
	 ai = Cast<AAIController>(me->GetController());
	 
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	}

}

void UEnemyFSM::IdleState()
{
	currentTIme += GetWorld()->DeltaTimeSeconds;
	if (currentTIme > idleDelayTime)
	{
		//idle -> move �� ���� ��ȯ�� ������ġ �ʱ�ȭ 
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
		mState = EEnemyState::Move;
		currentTIme = 0;
		//�ִϸ��̼� ���� ����ȭ 
		anim->animState = mState;
	}
}
void UEnemyFSM::MoveState() 
{
	FVector destination = target->GetActorLocation(); 
	FVector dir = destination -  me->GetActorLocation();
	//me->AddMovementInput(dir.GetSafeNormal()*0.5);
	//ai->MoveToLocation(destination);
	
	//Navigation System ������ 
	UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	//������ �� ã�� ��� ������ �˻� 
	FPathFindingQuery query;
	FAIMoveRequest req;
	
	//���������� ������ �� �ִ� ���� 
	req.SetAcceptanceRadius(3);
	req.SetGoalLocation(destination);
	//�� ã�⸦ ���� ���� ���� 
	ai->BuildPathfindingQuery(req, query);
	//��ã�� ��� �������� 
	FPathFindingResult r = ns->FindPathSync(query);

	
	//���������� ��ã�� ���� ����Ȯ�� 
	if (r.Result == ENavigationQueryResult::Success)
	{
		//Ÿ������ �̵� 
		ai->MoveToLocation(destination);
		//UE_LOG(TPS, Warning, TEXT("move to taget"));
	}
	else
	{
		//������ġ�� �̵� 
		auto result = ai->MoveToLocation(randomPos);
		//UE_LOG(TPS, Warning, TEXT("RandomPos : %s"), *randomPos.ToString());
		//�������� �����ϸ� 
		if (result == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			//���ο� ������ġ �������� 
			GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
		//	UE_LOG(TPS, Warning, TEXT("destination set to random"));
		}
		//���ڱ� ���ڸ� ���� �Ҷ� ���� 
		else if (result == EPathFollowingRequestResult::Failed)
		{
			//UE_LOG(TPS, Warning, TEXT("Move Failed"));
			GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
		}
	
	}
	//��ǥ�� �Ÿ��� ��������� ���� 
	if (dir.Size() < attackRange)
	{
		//���ݽ� ��ã�� �������
		ai->StopMovement();

		mState = EEnemyState::Attack;
		//�ִϸ��̼� ���� ����ȭ 
		anim->animState = mState;
		//���� �ִϸ��̼� ��� Ȱ��ȭ
		anim->bAttackPlay = true;

		//���ݻ��� ��ȯ �� ��� �ð��� �ٷ� �������� ó��..? ���� �������ⰰ�� �ڵ�� 
		currentTIme = attackDelayTime;

		
	}

};
void UEnemyFSM::AttackState() 
{
	currentTIme += GetWorld()->DeltaTimeSeconds;
	if (currentTIme > attackDelayTime)
	{
		currentTIme = 0;
		anim->bAttackPlay = true;
	}
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	if (distance > attackRange)
	{
		//�����ϴ� Ÿ���� �̵��Ͽ� �־��� ���� ����� RandPos �ʱ�ȭ 
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500.0f, randomPos);
		mState = EEnemyState::Move;
		//�ִϸ��̼� ���� ����ȭ 
		anim->animState = mState;
	}
};
void UEnemyFSM::DamageState() 
{
	currentTIme += GetWorld()->DeltaTimeSeconds;

	if (currentTIme > damageDelayTime)
	{
		mState = EEnemyState::Idle;
		currentTIme = 0;
		//�ִϸ��̼� ���� ����ȭ 
		anim->animState = mState;
	}
};
void UEnemyFSM::DieState() 
{
	//die Anim not done => no Sinking
	if (anim->bDieDone == false)
	{
		return;
	}
	//�Ʒ��� ��� �������� 
	//P = P0 +VT
	
	FVector P0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector P = P0 + vt;
	me->SetActorLocation(P);
	if (P.Z < -200.0f)
	{
		me->Destroy();
	}
};


void UEnemyFSM::OnDamageProcess()
{
	hp--;
	if (hp > 0)
	{
		mState = EEnemyState::Damage;
		currentTIme = 0;

		int32 index = FMath::RandRange(0, 1);
		FString sectionName = FString::Printf(TEXT("Damage%d"), index);
		anim->PlayDamageAnim(FName(*sectionName));
	}
	else
	{
		mState = EEnemyState::Die;
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		anim->PlayDamageAnim(TEXT("Die"));

	}
	//�ִϸ��̼� ���� ����ȭ 
	anim->animState = mState;
	ai->StopMovement();
}
bool UEnemyFSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
	dest = loc.Location;
	
	return result;
}
;
