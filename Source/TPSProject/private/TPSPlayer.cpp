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

	//스켈레탈 메시 데이터 불러오기
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		//메시 컴포넌트 위치와 회전값 설정 
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
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperGunMesh(TEXT("StaticMesh'/Game/SniperGun/sniper1.sniper1'")); // staticMeshComponent 말고 staticMesh로 ...
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


	//초기 속도 is 걷기 
	//GetCharacterMovement()->MaxWalkSpeed = walkSpeed;


	//_sniperUI = CreateWidget(GetWorld(), sniperUIFactory); // 블루프린트의 sniperUIFactory에 등록된 BP 스나이퍼 UI를 만들어서 추가한다, 

	//위와 같다 
	//CrosshairUIWidget = CreateWidget(GetWorld(), crosshairUIFactory);
	//뷰포트에 추가한다
	//CrosshairUIWidget->AddToViewport();

	//기본으로 스나이퍼가 먼저사용하도록 설정됨
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

	//컴포넌트에서 입력 바인딩 처리하도록 호출 
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
//void ATPSPlayer::InputJump()//BindAction Deligate는 매개변수가 없는 듯 
//{
//	Jump();//캐릭터 클래스에 이미 구현된 기능
//}
//
//void ATPSPlayer::Move()
//{
//	
//	
//	direction = FTransform(GetActorRotation()).TransformVector(direction); // 콘트롤러가 아닌 이 액터 기준으로 임의 설정함, 하늘 보면 못 걷는다는게 말이 됨?
//
//	//FVector P0 = GetActorLocation();
//	//FVector Vt = direction * walkSpeed * DeltaTime; //direction * walkSpeed => 방향 * 스칼라 
//	//FVector P = P0 + Vt;
//	//SetActorLocation(P);
//	
//	//UE_LOG(LogTemp, Warning, TEXT("X : %f, Y : %f , Z : %f "), direction.X, direction.Y, direction.Z);
//	AddMovementInput(direction);
//	direction = FVector::ZeroVector;
//}

//void ATPSPlayer::InputFire()
//{
//	//사운드 재생ㅇ
//	UGameplayStatics::PlaySound2D(GetWorld(), bulletSound);
//	//카메라 shake 
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
//		//충돌정보 변수
//		FHitResult hitInfo;
//		//충돌옵션 설정 변수 
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
//			UObject* enemy = hitInfo.GetActor()->GetDefaultSubobjectByName("FSM");//CreateDefaultSubObject()에서 넣어준 이름 반환
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
//void ATPSPlayer::SniperAim() // 여기서 고장나는데 왜 고장나지 
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
//		//일반조준 제거 
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

