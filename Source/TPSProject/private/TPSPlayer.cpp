// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "PlayerMove.h"
#include "PlayerFire.h"
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
	
	
	playerMove = CreateDefaultSubobject<UPlayerMove>(TEXT("PlayerMove"));
	playerFire = CreateDefaultSubobject<UPlayerFire>(TEXT("PlayerFire"));
	
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();


	//�ʱ� �ӵ� is �ȱ� 
	//GetCharacterMovement()->MaxWalkSpeed = walkSpeed;


	//_sniperUI = CreateWidget(GetWorld(), sniperUIFactory); // �������Ʈ�� sniperUIFactory�� ��ϵ� BP �������� UI�� ���� �߰��Ѵ�, 

	//���� ���� 
	//CrosshairUIWidget = CreateWidget(GetWorld(), crosshairUIFactory);
	//����Ʈ�� �߰��Ѵ�
	//CrosshairUIWidget->AddToViewport();

	//�⺻���� �������۰� ��������ϵ��� ������
	//ChangeToSniperGun();
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
	playerMove->SetupInputBinding(PlayerInputComponent);
	playerFire->SetupInputBinding(PlayerInputComponent);
	//PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATPSPlayer::Turn);
	//PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATPSPlayer::LookUp);
	//PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &ATPSPlayer::InputHorizontal);
	//PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &ATPSPlayer::InputVertical);
	//PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ATPSPlayer::InputJump);
	//PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &ATPSPlayer::InputRun);
	//PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &ATPSPlayer::InputRun);
	//PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATPSPlayer::InputFire);
	//PlayerInputComponent->BindAction(TEXT("GrenadeGun"), IE_Pressed, this, &ATPSPlayer::ChangeToGrenadeGun);
	//PlayerInputComponent->BindAction(TEXT("SniperGun"), IE_Pressed, this, &ATPSPlayer::ChangeToSniperGun);
	//PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Pressed, this, &ATPSPlayer::SniperAim);
	//PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Released, this, &ATPSPlayer::SniperAim);
	


}
//void ATPSPlayer::InputHorizontal(float value)
//
//{
//	direction.Y = value;
//}
//
//void ATPSPlayer::InputVertical(float value)
//{
//	direction.X = value;
//}
//
//void ATPSPlayer::InputJump()//BindAction Deligate�� �Ű������� ���� �� 
//{
//	Jump();//ĳ���� Ŭ������ �̹� ������ ���
//}
//
//void ATPSPlayer::Move()
//{
//	
//	
//	direction = FTransform(GetActorRotation()).TransformVector(direction); // ��Ʈ�ѷ��� �ƴ� �� ���� �������� ���� ������, �ϴ� ���� �� �ȴ´ٴ°� ���� ��?
//
//	//FVector P0 = GetActorLocation();
//	//FVector Vt = direction * walkSpeed * DeltaTime; //direction * walkSpeed => ���� * ��Į�� 
//	//FVector P = P0 + Vt;
//	//SetActorLocation(P);
//	
//	//UE_LOG(LogTemp, Warning, TEXT("X : %f, Y : %f , Z : %f "), direction.X, direction.Y, direction.Z);
//	AddMovementInput(direction);
//	direction = FVector::ZeroVector;
//}

//void ATPSPlayer::InputFire()
//{
//	//���� �����
//	UGameplayStatics::PlaySound2D(GetWorld(), bulletSound);
//	//ī�޶� shake 
//	APlayerController* controller = GetWorld()->GetFirstPlayerController();
//	controller->PlayerCameraManager->StartCameraShake(cameraShake);
//
//	if (bUsingGrenadeGun)
//	{
//		FTransform firePosition = gunComp -> GetSocketTransform(TEXT("Fireposition"));
//		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
//	}
//	else
//	{
//		UPlayerAnim* anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
//		anim->PlayAttackAnim();
//		FVector startPos = tpsCamComponent->GetComponentLocation(); //WorldSpace Return
//		FVector endPos = startPos + tpsCamComponent->GetForwardVector() * 5000000.0f;
//
//		//�浹���� ����
//		FHitResult hitInfo;
//		//�浹�ɼ� ���� ���� 
//		FCollisionQueryParams params;
//		params.AddIgnoredActor(this);
//
//		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params); //ECC => Enum Collision Channel 
//		
//		if (bHit)
//		{
//			FTransform bulletTrans;
//			bulletTrans.SetLocation(hitInfo.ImpactPoint);
//			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans);
//
//			UPrimitiveComponent* hitComp = hitInfo.GetComponent();
//			if (hitComp != nullptr&& hitComp->IsSimulatingPhysics())
//			{
//					FVector force = hitInfo.ImpactNormal* -1 * hitComp->GetMass() * 500000.0f;
//					hitComp->AddForce(force);
//					UE_LOG(TPS, Warning, TEXT("Hit_Box"));;
//			}
//			UObject* enemy = hitInfo.GetActor()->GetDefaultSubobjectByName("FSM");//CreateDefaultSubObject()���� �־��� �̸� ��ȯ
//			if (enemy != nullptr)
//			{
//				UEnemyFSM* enemyFSM = Cast<UEnemyFSM>(enemy);
//				enemyFSM->OnDamageProcess();
//				UE_LOG(TPS, Warning, TEXT("Hit_Enemy"));
//			}
//			
//			
//		//	UE_LOG(TPS, Warning, TEXT("%s"), *hitInfo.GetActor()->GetName());
//			
//		}
//	}
//	
//}

//void ATPSPlayer::ChangeToGrenadeGun()
//{
//	bUsingGrenadeGun = true;
//	sniperComp->SetVisibility(false);
//	gunComp->SetVisibility(true);
//}
//
//void ATPSPlayer::ChangeToSniperGun()
//{
//	bUsingGrenadeGun = false;
//	sniperComp->SetVisibility(true);
//	gunComp->SetVisibility(false);
//}
//
//void ATPSPlayer::SniperAim() // ���⼭ ���峪�µ� �� ���峪�� 
//{
//	if (bUsingGrenadeGun)
//	{
//		return;
//	}
//	if (bSniperAim == false)
//	{
//		bSniperAim =true;
//	
//		_sniperUI->AddToViewport();
//		tpsCamComponent->SetFieldOfView(45.0f);
//
//		//�Ϲ����� ���� 
//		CrosshairUIWidget->RemoveFromParent();
//	}
//	else
//	{
//		bSniperAim = false;
//		
//		_sniperUI->RemoveFromParent();
//		
//		tpsCamComponent->SetFieldOfView(90.0f);
//		
//		CrosshairUIWidget->AddToViewport();
//		
//	}
//}

//void ATPSPlayer::InputRun()
//{
//	UCharacterMovementComponent* movement = GetCharacterMovement();
//
//	if (movement->MaxWalkSpeed > walkSpeed)
//	{
//		movement->MaxWalkSpeed = walkSpeed;
//	}
//	else
//	{
//		movement->MaxWalkSpeed = runSpeed;
//	}
//	
//}

