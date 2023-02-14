// Fill out your copyright notice in the Description page of Project Settings.



#include "PlayerBaseComponent.h"

// Sets default values for this component's properties

// Called when the game starts
void UPlayerBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UPlayerBaseComponent::InitializeComponent()
{

	//GetOwner�� �� ������Ʈ�� �����ϴ� ������ �ν��Ͻ��� ������ ATPSPlayer ������ ĳ���� �Ͽ� me ������ �Ҵ� 
	me = Cast<ATPSPlayer>(GetOwner());
	//me�κ��� GetCharacterMovement�� �̿��Ͽ� UCharacterMovementComponent* Ÿ���� moveComp�� �Ҵ� 
	moveComp = me->GetCharacterMovement();

	me->onInputBindingDelegate.AddUObject(this, &UPlayerBaseComponent::SetupInputBinding);

}

void UPlayerBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
