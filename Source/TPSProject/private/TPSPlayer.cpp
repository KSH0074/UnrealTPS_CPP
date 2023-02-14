// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "PlayerMove.h"
#include "PlayerFire.h"
#include "TPSProject.h"
#include <Kismet/GameplayStatics.h>
// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//���̷�Ż �޽� ������ �ҷ�����
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		//�޽� ������Ʈ ��ġ�� ȸ���� ���� 
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
	
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->TargetArmLength = 400;
	springArmComp->bUsePawnControlRotation = true;

	
	tpsCamComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComponent->SetupAttachment(springArmComp);
	tpsCamComponent->bUsePawnControlRotation = false;
	
	bUseControllerRotationYaw = true;

	JumpMaxCount = 3;

	gunComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComponent"));
	gunComp->SetupAttachment(GetMesh(),TEXT("GunSocket"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (TempGunMesh.Succeeded())
	{
		gunComp->SetSkeletalMesh(TempGunMesh.Object);
		gunComp->SetRelativeLocation(FVector(-17, 10, -3));
		gunComp->SetRelativeRotation(FRotator(0, 90, 0));
	}

	sniperComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGunComponent"));
	sniperComp->SetupAttachment(GetMesh(),TEXT("GunSocket"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperGunMesh(TEXT("StaticMesh'/Game/SniperGun/sniper1.sniper1'")); // staticMeshComponent ���� staticMesh�� ...
	if (TempSniperGunMesh.Succeeded())
	{
		sniperComp->SetStaticMesh(TempSniperGunMesh.Object);
		sniperComp->SetRelativeLocation(FVector(-42, 7, 1));
		sniperComp->SetRelativeRotation(FRotator(0, 90, 0));
		sniperComp->SetRelativeScale3D(FVector(0.15f));
	}

	
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	hp = initialHP;


}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Move();

}


void ATPSPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//������Ʈ���� �Է� ���ε� ó���ϵ��� ȣ�� 
	onInputBindingDelegate.Broadcast(PlayerInputComponent);
}
void ATPSPlayer::OnHitEvent()
{
	PRINT_LOG(TEXT("Damaged"));
	hp--;
	if (hp <= 0)
	{
		PRINT_LOG(TEXT("PLAYER IS DEAD"));
		OnGameOver();
	}
}
//BlueprintNativeEvnent => _Implementation �ٿ��� ���� �ؾ���
void ATPSPlayer::OnGameOver_Implementation()
{
	//���ӿ����� �Ͻ�����
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	PRINT_LOG(TEXT("the world"));
}

