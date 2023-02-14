// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FInputBindingDelegate, class UInputComponent*);

UCLASS()
class TPSPROJECT_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	FInputBindingDelegate onInputBindingDelegate;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* springArmComp;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly,Category = Camera)
	class UCameraComponent* tpsCamComponent;

	
	//void Inputfire();

	
	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class USkeletalMeshComponent* gunComp;
	
	//UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
	//	TSubclassOf<class ABullet> bulletFactory; // Default ����� 
	
	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class UStaticMeshComponent* sniperComp;
	
	

public:
	//������Ʈ ���
	UPROPERTY(VisibleAnywhere, Category = Component)
	class UPlayerBaseComponent* playerMove;

	//������Ʈ ���
	UPROPERTY(VisibleAnywhere, Category = Component)
	class UPlayerBaseComponent* playerFire;

	//���� HP
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = health)
	int32 hp;
	//HP �ʱⰪ 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = health)
	int32 initialHP = 10;

	//�ǰ� ������ �� ó�� 
	UFUNCTION(BlueprintCallable, Category = helath)
	void OnHitEvent();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = health)
	void OnGameOver();
	//�̰� ����� ���尡 ��
	void OnGameOver_Implementation();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Health)
	void OnUsingGrenade(bool isGrenade);
	
};