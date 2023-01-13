// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "bullet.h"
#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//���̷�Ż �޽� ������ �ҷ�����
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
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

	
	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);
	tpsCamComp->bUsePawnControlRotation = false;
	bUseControllerRotationYaw = true;

	JumpMaxCount = 3;

	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	gunMeshComp->SetupAttachment(GetMesh());
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (TempGunMesh.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		gunMeshComp->SetRelativeLocation(FVector(-14, 52, 120));
	}
	sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGunComp"));
	sniperGunComp->SetupAttachment(GetMesh());
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperGunMesh(TEXT("StaticMesh'/Game/SniperGun/sniper1.sniper1'")); // staticMeshComponent ���� staticMesh�� ...
	if (TempSniperGunMesh.Succeeded())
	{
		sniperGunComp->SetStaticMesh(TempSniperGunMesh.Object);
		sniperGunComp->SetRelativeLocation(FVector(-22, 55, 120));
		sniperGunComp->SetRelativeScale3D(FVector(0.15f));
	}
	
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
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);

	ChangeToSniperGun();//default
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move();

}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATPSPlayer::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATPSPlayer::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &ATPSPlayer::InputHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &ATPSPlayer::InputVertical);
	PlayerInputComponent->BindAction(TEXT("Jump"),IE_Pressed ,this, &ATPSPlayer::InputJump);
	PlayerInputComponent->BindAction(TEXT("Fire"),IE_Pressed ,this, &ATPSPlayer::InputFire);
	PlayerInputComponent->BindAction(TEXT("GrenadeGun"), IE_Pressed, this, &ATPSPlayer::ChangeToGrenadeGun);
	PlayerInputComponent->BindAction(TEXT("SniperGun"), IE_Pressed, this, &ATPSPlayer::ChangeToSniperGun);
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Pressed, this, &ATPSPlayer::SniperAim);
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Released, this, &ATPSPlayer::SniperAim);
	

}

void ATPSPlayer::Turn(float value)
{
	AddControllerYawInput(value);
}

void ATPSPlayer::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void ATPSPlayer::InputHorizontal(float value)

{
	direction.Y = value;
}

void ATPSPlayer::InputVertical(float value)
{
	direction.X = value;
}

void ATPSPlayer::InputJump()//BindAction Deligate�� �Ű������� ���� �� 
{
	Jump();//ĳ���� Ŭ������ �̹� ������ ���
}

void ATPSPlayer::Move()
{
	
	
	direction = FTransform(GetActorRotation()).TransformVector(direction); // ��Ʈ�ѷ��� �ƴ� �� ���� �������� ���� ������, �ϴ� ���� �� �ȴ´ٴ°� ���� ��?

	//FVector P0 = GetActorLocation();
	//FVector Vt = direction * walkSpeed * DeltaTime; //direction * walkSpeed => ���� * ��Į�� 
	//FVector P = P0 + Vt;
	//SetActorLocation(P);
	
	//UE_LOG(LogTemp, Warning, TEXT("X : %f, Y : %f , Z : %f "), direction.X, direction.Y, direction.Z);
	AddMovementInput(direction);
	direction = FVector::ZeroVector;
}

void ATPSPlayer::InputFire()
{
	if (bUsingGrenadeGun)
	{
		FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("Fireposition"));
		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
	}
	else
	{
		FVector startPos = tpsCamComp->GetComponentLocation(); //WorldSpace Return
		FVector endPos = startPos + tpsCamComp->GetForwardVector() * 5000.0f;

		//�浹���� ����
		FHitResult hitInfo;
		//�浹�ɼ� ���� ���� 
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);

		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params); //ECC => Enum Collision Channel 
		
		if (bHit)
		{
			FTransform bulletTrans;
			bulletTrans.SetLocation(hitInfo.ImpactPoint);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans);

			UPrimitiveComponent* hitComp = hitInfo.GetComponent();
			if (hitComp != nullptr&& hitComp->IsSimulatingPhysics())
			{
					FVector force = hitInfo.ImpactNormal* -1 * hitComp->GetMass() * 500000.0f;
					hitComp->AddForce(force);
			}
		}
	}
	
}

void ATPSPlayer::ChangeToGrenadeGun()
{
	bUsingGrenadeGun = true;
	sniperGunComp->SetVisibility(false);
	gunMeshComp->SetVisibility(true);
}

void ATPSPlayer::ChangeToSniperGun()
{
	bUsingGrenadeGun = false;
	sniperGunComp->SetVisibility(true);
	gunMeshComp->SetVisibility(false);
}

void ATPSPlayer::SniperAim()
{
	if (bUsingGrenadeGun)
	{
		return;
	}
	if (bSniperAim == false)
	{
		bSniperAim = true;
		_sniperUI ->AddToViewport();
		tpsCamComp->SetFieldOfView(45.0f);
	}
	else
	{
		bSniperAim = false;
		_sniperUI->RemoveFromParent();
		tpsCamComp->SetFieldOfView(90.0f);
	}
}

