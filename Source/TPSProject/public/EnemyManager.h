// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

UCLASS()
class TPSPROJECT_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//랜덤시간 간격 최솟값 
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		float minTime = 1;
	//랜덤 시간 최댓값 
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		float maxTime = 5;
	//스폰위치 정보 배열 
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		TArray<class AActor*> spawnPoints;
	//AEnemy 타입의 블루프린트 할당받을 변수 
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		TSubclassOf<class AEnemy> enemyFactory;

	//스폰을 위한 알람 타이머 
	UPROPERTY()
	FTimerHandle spawnTimerHandle;

	//적생성합수 
	UFUNCTION()
	void CreateEnemy();

	//스폰위치 찾아서 동적할당 
	UFUNCTION()
	void FIndSpawnPoints();
	void FIndSpawnPoints2();
};
