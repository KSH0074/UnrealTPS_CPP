// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerFire.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (PlayerComponent), meta = (BlueprintSpawnableComponent))
class TPSPROJECT_API UPlayerFire : public UPlayerBaseComponent
{
	GENERATED_BODY()
	
public:
	UPlayerFire();
	virtual void BeginPlay() override;
	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;
public:
	//�Ѿ˰���
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
	TSubclassOf<class ABullet> bulletFactory;

	//�Ѿ� �߻� ó���Լ�
	void InputFire();

	bool bUsingGrenadeGun = true;
	void ChangeToGrenadeGun();
	void ChangeToSniperGun();

	void SniperAim();
	bool bSniperAim = false;

	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	TSubclassOf<class UUserWidget> sniperUIFactory;// Default ����� 

	//�������� UI���� �ν��Ͻ�
	UPROPERTY() // �̰� ���ϸ� ������ ���� ó������ 
	class UUserWidget* _sniperUI;

	UPROPERTY(EditAnywhere, Category = BulletEffect)
	UParticleSystem* bulletEffectFactory;

	//�Ϲ�����
	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	TSubclassOf<class UUserWidget> crosshairUIFactory;
	//ũ�ν���� �ν��Ͻ�
	UPROPERTY()
	class UUserWidget* CrosshairUIWidget;



	//ī�޶� ����ũ BP ���� ���� 
	UPROPERTY(EditDefaultsOnly, Category = CameraMotion)
	TSubclassOf<class UCameraShakeBase> cameraShake;

	//Gun fire sound
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	class USoundBase* bulletSound;
	
	
	UPROPERTY()
	class UCameraComponent* tpsCamComponent;

	UPROPERTY()
	class USkeletalMeshComponent* gunComp;

	UPROPERTY()
	class UStaticMeshComponent* sniperComp;

};
