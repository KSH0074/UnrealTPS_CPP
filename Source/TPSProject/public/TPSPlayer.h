// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"


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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
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
	
	//bool bUsingGrenadeGun= true;
	//void ChangeToGrenadeGun();
	//void ChangeToSniperGun();

	//void SniperAim();
	//bool bSniperAim = false;
	//UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	//	TSubclassOf<class UUserWidget> sniperUIFactory;// Default ����� 

	////�������� UI���� �ν��Ͻ�
	//UPROPERTY(EditDefaultsOnly, Category = SniperUI) // �̰� ���ϸ� ������ ���� ó������ 
	//class UUserWidget* _sniperUI;

	//UPROPERTY(EditAnywhere,Category=BulletEffect)
	//UParticleSystem* bulletEffectFactory;

	////�Ϲ�����
	//UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	//TSubclassOf<class UUserWidget> crosshairUIFactory;
	////ũ�ν���� �ν��Ͻ�
	//class UUserWidget* CrosshairUIWidget;

	//

	////ī�޶� ����ũ BP ���� ���� 
	//UPROPERTY(EditDefaultsOnly, Category = CameraMotion)
	//	TSubclassOf<class UCameraShakeBase> cameraShake;

	////Gun fire sound
	//UPROPERTY(EditDefaultsOnly, Category = Sound)
	//	class USoundBase* bulletSound;

public:
	//������Ʈ ���
	UPROPERTY(VisibleAnywhere, Category = Component)
	class UPlayerBaseComponent* playerMove;

	//������Ʈ ���
	UPROPERTY(VisibleAnywhere, Category = Component)
	class UPlayerBaseComponent* playerFire;
};