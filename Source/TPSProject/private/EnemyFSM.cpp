// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "Enemy.h"
#include "TPSPlayer.h"
#include <Kismet/GameplayStatics.h>
#include "TPSProject.h"
#include <Components/CapsuleComponent.h>
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
		mState = EEnemyState::Move;
		currentTIme = 0;
	}
}
void UEnemyFSM::MoveState() 
{
	FVector destination = target->GetActorLocation(); 
	FVector dir = destination -  me->GetActorLocation();
	me->AddMovementInput(dir.GetSafeNormal()*0.5);

	//목표와 거리가 가까워지면 공격 
	if (dir.Size() < attackRange)
	{
		mState = EEnemyState::Attack;
	}

};
void UEnemyFSM::AttackState() 
{
	currentTIme += GetWorld()->DeltaTimeSeconds;
	if (currentTIme > attackDelayTime)
	{
		PRINT_LOG(TEXT("Attack!!"));
		currentTIme = 0;
	}
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	if (distance > attackRange)
	{
		mState = EEnemyState::Move;
	}
};
void UEnemyFSM::DamageState() 
{
	currentTIme += GetWorld()->DeltaTimeSeconds;

	if (currentTIme > damageDelayTime)
	{
		mState = EEnemyState::Idle;
		currentTIme = 0;
	}
};
void UEnemyFSM::DieState() 
{
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
	}
	else
	{
		mState = EEnemyState::Die;
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
};
