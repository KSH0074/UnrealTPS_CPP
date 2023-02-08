// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMove.h"

UPlayerMove::UPlayerMove() 
{
	//Tick �Լ� ȣ��ǵ��� 
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

void UPlayerMove::InputJump()//BindAction Deligate�� �Ű������� ���� �� 
{
	me->Jump();//ĳ���� Ŭ������ �̹� ������ ���
}

void UPlayerMove::Move()
{


	direction = FTransform(me->GetActorRotation()).TransformVector(direction); // ��Ʈ�ѷ��� �ƴ� �� ���� �������� ���� ������, �ϴ� ���� �� �ȴ´ٴ°� ���� ��?

	//FVector P0 = GetActorLocation();
	//FVector Vt = direction * walkSpeed * DeltaTime; //direction * walkSpeed => ���� * ��Į�� 
	//FVector P = P0 + Vt;
	//SetActorLocation(P);

	//UE_LOG(LogTemp, Warning, TEXT("X : %f, Y : %f , Z : %f "), direction.X, direction.Y, direction.Z);
	me->AddMovementInput(direction);
	direction = FVector::ZeroVector;
}

void UPlayerMove::InputRun()
{
	UCharacterMovementComponent* movement = moveComp;//�θ� Ŭ������ moveComp�̴� 

	if (movement->MaxWalkSpeed > walkSpeed)
	{
		movement->MaxWalkSpeed = walkSpeed;
	}
	else
	{
		movement->MaxWalkSpeed = runSpeed;
	}

}
