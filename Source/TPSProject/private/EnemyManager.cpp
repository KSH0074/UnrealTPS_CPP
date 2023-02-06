// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "Enemy.h"
#include <EngineUtils.h> //Iterator 사용하려면 필요
#include <Kismet/GameplayStatics.h>
// Sets default values
AEnemyManager::AEnemyManager()
{
 	// tick을 이용해서 시간체크 안함 
	PrimaryActorTick.bCanEverTick = false;
	
}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	float createTime = FMath::FRandRange(minTime, maxTime);
	GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyManager::CreateEnemy, createTime);
	FIndSpawnPoints2();
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyManager::CreateEnemy()
{
	int index = FMath::RandRange(0, spawnPoints.Num() - 1);//TArray 클래스 AActor* 제너릭
	//적 생성 및 배치
	GetWorld()->SpawnActor<AEnemy>(enemyFactory, spawnPoints[index]->GetActorLocation(), FRotator(0));
	//다시 랜덤 시간에 CreateEnemy 함수 호출되도록 설정 
	float createTime = FMath::FRandRange(minTime, maxTime);
	GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyManager::CreateEnemy, createTime);
}

void AEnemyManager::FIndSpawnPoints()
{
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* spawn = *It;
		if (spawn->GetName().Contains(TEXT("BP_EnemySPawnPoint")))
		{
			spawnPoints.Add(spawn);
		}
	}
}

void AEnemyManager::FIndSpawnPoints2()
{
	//검색으로 찾은 결과를 저장할 배열
	TArray<AActor*> allActors;
	//원하는 타입의 액터 모두 찾기
	// 매개변수 : "보통 UWorld객체","찾을 Actor클래스", "찾은 결과 액터들을 넘겨받을 배열" 
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allActors);
	for (auto spawn : allActors) //이 문법 생소하다 : 는 리스트 초기화 연산자라고 한다 
	{
		if (spawn->GetName().Contains(TEXT("BP_EnemySPawnPoint")))
		{
			spawnPoints.Add(spawn);
		}
	}

	
}
