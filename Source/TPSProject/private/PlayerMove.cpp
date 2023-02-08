// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMove.h"

UPlayerMove::UPlayerMove() 
{
	//Tick 함수 호출되도록 
	PrimaryComponentTick.bCanEverTick = true;
}
void UPlayerMove::BeginPlay()
{
	Super::BeginPlay();
	moveComp->MaxWalkSpeed = walkSpeed;
}

void UPlayerMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Move();
}


void UPlayerMove::Turn(float value)
{
	me->AddControllerYawInput(value);
}

void UPlayerMove::LookUp(float value)
{
	me->AddControllerPitchInput(value);
}

void UPlayerMove::SetupInputBinding(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &UPlayerMove::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &UPlayerMove::LookUp);

	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &UPlayerMove::InputHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &UPlayerMove::InputVertical);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &UPlayerMove::InputJump);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &UPlayerMove::InputRun);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &UPlayerMove::InputRun);

}


void UPlayerMove::InputHorizontal(float value)

{
	direction.Y = value;
}

void UPlayerMove::InputVertical(float value)
{
	direction.X = value;
}

void UPlayerMove::InputJump()//BindAction Deligate는 매개변수가 없는 듯 
{
	me->Jump();//캐릭터 클래스에 이미 구현된 기능
}

void UPlayerMove::Move()
{


	direction = FTransform(me->GetActorRotation()).TransformVector(direction); // 콘트롤러가 아닌 이 액터 기준으로 임의 설정함, 하늘 보면 못 걷는다는게 말이 됨?

	//FVector P0 = GetActorLocation();
	//FVector Vt = direction * walkSpeed * DeltaTime; //direction * walkSpeed => 방향 * 스칼라 
	//FVector P = P0 + Vt;
	//SetActorLocation(P);

	//UE_LOG(LogTemp, Warning, TEXT("X : %f, Y : %f , Z : %f "), direction.X, direction.Y, direction.Z);
	me->AddMovementInput(direction);
	direction = FVector::ZeroVector;
}

void UPlayerMove::InputRun()
{
	UCharacterMovementComponent* movement = moveComp;//부모 클래스의 moveComp이다 

	if (movement->MaxWalkSpeed > walkSpeed)
	{
		movement->MaxWalkSpeed = walkSpeed;
	}
	else
	{
		movement->MaxWalkSpeed = runSpeed;
	}

}
