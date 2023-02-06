// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "Enemy.h"
#include <EngineUtils.h> //Iterator ����Ϸ��� �ʿ�
#include <Kismet/GameplayStatics.h>
// Sets default values
AEnemyManager::AEnemyManager()
{
 	// tick�� �̿��ؼ� �ð�üũ ���� 
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
	int index = FMath::RandRange(0, spawnPoints.Num() - 1);//TArray Ŭ���� AActor* ���ʸ�
	//�� ���� �� ��ġ
	GetWorld()->SpawnActor<AEnemy>(enemyFactory, spawnPoints[index]->GetActorLocation(), FRotator(0));
	//�ٽ� ���� �ð��� CreateEnemy �Լ� ȣ��ǵ��� ���� 
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
	//�˻����� ã�� ����� ������ �迭
	TArray<AActor*> allActors;
	//���ϴ� Ÿ���� ���� ��� ã��
	// �Ű����� : "���� UWorld��ü","ã�� ActorŬ����", "ã�� ��� ���͵��� �Ѱܹ��� �迭" 
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allActors);
	for (auto spawn : allActors) //�� ���� �����ϴ� : �� ����Ʈ �ʱ�ȭ �����ڶ�� �Ѵ� 
	{
		if (spawn->GetName().Contains(TEXT("BP_EnemySPawnPoint")))
		{
			spawnPoints.Add(spawn);
		}
	}

	
}
