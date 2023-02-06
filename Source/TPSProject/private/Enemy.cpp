// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h"
#include "TPSProject.h"
// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Enemy/Model/vampire_a_lusth.vampire_a_lusth'")); //오브젝트를 찾는다
	//성공할 경우
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-88),FRotator(0,-90,0));
		UE_LOG(TPS, Warning, TEXT("Set Mesh default Setting "));
		GetMesh()->SetRelativeScale3D(FVector(0.84f));
	}
	//컴포넌트 추가 
	
	fsmComp = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM")); //FSM => Finit State Machine
	
	//애니메이션 블루프린트 할당 
	ConstructorHelpers::FClassFinder<UAnimInstance> tempClass(TEXT("AnimBlueprint'/Game/Blueprint/ABP_Enemy.ABP_Enemy_C'")); // _C 가 있어야 블루프린트 클래스로 인식
	if (tempClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempClass.Class);
	}
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

