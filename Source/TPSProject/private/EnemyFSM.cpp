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
	 AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass()); // ATPSPlayer타입 액터 찾기 
	 target = Cast<ATPSPlayer>(actor); // actor 를 target으로 형변환 GetActorOf~~가 AActor* 타입으로 반환한걸 

	 me = Cast<AEnemy>(GetOwner());
	 
	 //anim 에 사용중인 애니메이션 블루프린트  할당,  
	 anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());
	 //AAIController에 할당
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
		//idle -> move 로 상태 변환시 랜덤위치 초기화 
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
		mState = EEnemyState::Move;
		currentTIme = 0;
		//애니메이션 상태 동기화 
		anim->animState = mState;
	}
}
void UEnemyFSM::MoveState() 
{
	FVector destination = target->GetActorLocation(); 
	FVector dir = destination -  me->GetActorLocation();
	//me->AddMovementInput(dir.GetSafeNormal()*0.5);
	//ai->MoveToLocation(destination);
	
	//Navigation System 얻어오기 
	UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	//목적지 길 찾기 경로 데이터 검색 
	FPathFindingQuery query;
	FAIMoveRequest req;
	
	//목적지에서 인지할 수 있는 범위 
	req.SetAcceptanceRadius(3);
	req.SetGoalLocation(destination);
	//길 찾기를 위한 쿼리 생성 
	ai->BuildPathfindingQuery(req, query);
	//길찾기 결과 가져오기 
	FPathFindingResult r = ns->FindPathSync(query);

	
	//목적지까지 길찾기 성공 여부확인 
	if (r.Result == ENavigationQueryResult::Success)
	{
		//타깃으로 이동 
		ai->MoveToLocation(destination);
		//UE_LOG(TPS, Warning, TEXT("move to taget"));
	}
	else
	{
		//랜덤위치로 이동 
		auto result = ai->MoveToLocation(randomPos);
		//UE_LOG(TPS, Warning, TEXT("RandomPos : %s"), *randomPos.ToString());
		//목적지에 도착하면 
		if (result == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			//새로운 랜덤위치 가져오기 
			GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
		//	UE_LOG(TPS, Warning, TEXT("destination set to random"));
		}
		//갑자기 제자리 걸음 할때 방지 
		else if (result == EPathFollowingRequestResult::Failed)
		{
			//UE_LOG(TPS, Warning, TEXT("Move Failed"));
			GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
		}
	
	}
	//목표와 거리가 가까워지면 공격 
	if (dir.Size() < attackRange)
	{
		//공격시 길찾기 기능정지
		ai->StopMovement();

		mState = EEnemyState::Attack;
		//애니메이션 상태 동기화 
		anim->animState = mState;
		//공격 애니메이션 재생 활성화
		anim->bAttackPlay = true;

		//공격상태 전환 시 대기 시간이 바로 끝나도록 처리..? 뭔가 군더더기같은 코드다 
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
		//공격하다 타깃이 이도하여 멀어져 상태 변경시 RandPos 초기화 
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500.0f, randomPos);
		mState = EEnemyState::Move;
		//애니메이션 상태 동기화 
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
		//애니메이션 상태 동기화 
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
	//아래로 계속 내려간다 
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
	//애니메이션 상태 동기화 
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
