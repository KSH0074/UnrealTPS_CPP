// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerAnim.h"
#include "EnemyFSM.h"
#include <Camera/CameraComponent.h>
#include "bullet.h"
#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include "PlayerFire.h"
/*
*
* 
* 
*/

UPlayerFire::UPlayerFire()
{
	ConstructorHelpers::FClassFinder<ABullet> TempBullet(TEXT("Blueprint'/Game/Blueprint/BP_Bullet.BP_Bullet_C'"));
	if (TempBullet.Succeeded())
	{
		bulletFactory = TempBullet.Class;
	}
	ConstructorHelpers::FClassFinder<UUserWidget> TempSniperUI(TEXT("WidgetBlueprint'/Game/Blueprint/WBP_SniperUI.WBP_SniperUI_C'"));
	if (TempSniperUI.Succeeded())
	{
		sniperUIFactory = TempSniperUI.Class;
	}

	//get Sound 
	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("SoundWave'/Game/SniperGun/Rifle.Rifle'"));
	if (tempSound.Succeeded())
	{
		bulletSound = tempSound.Object;
	}
}
void UPlayerFire::BeginPlay()
{
	Super::BeginPlay();
	
	tpsCamComponent = me->tpsCamComponent;
	gunComp = me->gunComp;
	sniperComp = me->sniperComp;

	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory); // �������Ʈ�� sniperUIFactory�� ��ϵ� BP �������� UI�� ���� �߰��Ѵ�, 

	//���� ���� 
	CrosshairUIWidget = CreateWidget(GetWorld(), crosshairUIFactory);
	//����Ʈ�� �߰��Ѵ�
	CrosshairUIWidget->AddToViewport();

	//�⺻���� �������۰� ��������ϵ��� ������
	ChangeToSniperGun();


}

void UPlayerFire::SetupInputBinding(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &UPlayerFire::InputFire);
	PlayerInputComponent->BindAction(TEXT("GrenadeGun"), IE_Pressed, this, &UPlayerFire::ChangeToGrenadeGun);
	PlayerInputComponent->BindAction(TEXT("SniperGun"), IE_Pressed, this, &UPlayerFire::ChangeToSniperGun);
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Pressed, this, &UPlayerFire::SniperAim);
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Released, this, &UPlayerFire::SniperAim);

}

void UPlayerFire::InputFire()
{
	//���� �����
	UGameplayStatics::PlaySound2D(GetWorld(), bulletSound);
	//ī�޶� shake 
	APlayerController* controller = GetWorld()->GetFirstPlayerController();
	controller->PlayerCameraManager->StartCameraShake(cameraShake);

	if (bUsingGrenadeGun)
	{
		FTransform firePosition = gunComp->GetSocketTransform(TEXT("Fireposition"));
		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
	}
	else 
	{
		UPlayerAnim* anim = Cast<UPlayerAnim>(me->GetMesh()->GetAnimInstance());
		anim->PlayAttackAnim();
		FVector startPos = tpsCamComponent->GetComponentLocation(); //WorldSpace Return
		FVector endPos = startPos + tpsCamComponent->GetForwardVector() * 5000000.0f;

		//�浹���� ����
		FHitResult hitInfo;
		//�浹�ɼ� ���� ���� 
		FCollisionQueryParams params;
		params.AddIgnoredActor(me);

		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params); //ECC => Enum Collision Channel 

		if (bHit)
		{
			FTransform bulletTrans;
			bulletTrans.SetLocation(hitInfo.ImpactPoint);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans);

			UPrimitiveComponent* hitComp = hitInfo.GetComponent();
			if (hitComp != nullptr && hitComp->IsSimulatingPhysics())
			{
				FVector force = hitInfo.ImpactNormal * -1 * hitComp->GetMass() * 500000.0f;
				hitComp->AddForce(force);
				//UE_LOG(TPS, Warning, TEXT("Hit_Box"));;
			}
			UObject* enemy = hitInfo.GetActor()->GetDefaultSubobjectByName("FSM");//CreateDefaultSubObject()���� �־��� �̸� ��ȯ
			if (enemy != nullptr)
			{
				UEnemyFSM* enemyFSM = Cast<UEnemyFSM>(enemy);
				enemyFSM->OnDamageProcess();
				//UE_LOG(TPS, Warning, TEXT("Hit_Enemy"));
			}


			//	UE_LOG(TPS, Warning, TEXT("%s"), *hitInfo.GetActor()->GetName());

		}
	}

}

void UPlayerFire::ChangeToGrenadeGun()
{
	bUsingGrenadeGun = true;
	sniperComp->SetVisibility(false);
	gunComp->SetVisibility(true);
	//TPSPlayer�� OnUsingGrenade �Լ��� ���޵� 
	me->OnUsingGrenade(bUsingGrenadeGun);
}

void UPlayerFire::ChangeToSniperGun()
{
	bUsingGrenadeGun = false;
	sniperComp->SetVisibility(true);
	gunComp->SetVisibility(false);
	//TPSPlayer�� OnUsingGrenade �Լ��� ���޵� 
	me->OnUsingGrenade(bUsingGrenadeGun);
}

void UPlayerFire::SniperAim() // ���⼭ ���峪�µ� �� ���峪�� 
{
	if (bUsingGrenadeGun)
	{
		return;
	}
	if (bSniperAim == false)
	{
		bSniperAim = true;

		_sniperUI->AddToViewport();

		tpsCamComponent->SetFieldOfView(45.0f);

		//�Ϲ����� ���� 
		CrosshairUIWidget->RemoveFromParent();
	}
	else
	{
		bSniperAim = false;

		_sniperUI->RemoveFromParent();

		tpsCamComponent->SetFieldOfView(90.0f);

		CrosshairUIWidget->AddToViewport();

	}
}

