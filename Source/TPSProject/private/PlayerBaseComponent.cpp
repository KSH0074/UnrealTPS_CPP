// Fill out your copyright notice in the Description page of Project Settings.



#include "PlayerBaseComponent.h"

// Sets default values for this component's properties

// Called when the game starts
void UPlayerBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	//GetOwner로 이 컴포넌트를 소유하는 액터의 인스턴스를 가져와 ATPSPlayer 형으로 캐스팅 하여 me 변수에 할당 
	me = Cast<ATPSPlayer>(GetOwner());
	//me로부터 GetCharacterMovement를 이용하여 UCharacterMovementComponent* 타입을 moveComp에 할당 
	moveComp = me->GetCharacterMovement();
	
	
}


// Called every frame
void UPlayerBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

