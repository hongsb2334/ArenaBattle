// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterBase.h"
#include "ABCharacterData.h"
#include <GameFramework/CharacterMovementComponent.h>

// Sets default values
AABCharacterBase::AABCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    //맵(TMap) 설정
    static ConstructorHelpers::FObjectFinder<UABCharacterData> ShoulderDataRef(TEXT("/Game/ArenaBattle/CharacterControl/ABC_Shoulder.ABC_Shoulder"));

    if (ShoulderDataRef.Succeeded())
    {
        CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
    }

    static ConstructorHelpers::FObjectFinder<UABCharacterData> QuaterDataRef(
        TEXT("/Game/ArenaBattle/CharacterControl/ABC_Quater.ABC_Quater")
    );

    if (QuaterDataRef.Succeeded())
    {
        CharacterControlManager.Add(
            ECharacterControlType::Quater,
            QuaterDataRef.Object
        );
    }

}

void AABCharacterBase::SetCharacterControlData(const UABCharacterData* InCharacterData)
{
   
    //데이터에서 속성을 가져와서 필요한 곳에 설정

    //pawn 설정
    bUseControllerRotationYaw = InCharacterData->bUseControllerRotationYaw;

    //캐릭터 무브먼트 설정
    GetCharacterMovement()->bUseControllerDesiredRotation = InCharacterData->bUseControllerDesiredRotation;

    GetCharacterMovement()->bOrientRotationToMovement = InCharacterData->bUseOrientToMovement;

    GetCharacterMovement()->RotationRate = InCharacterData->RotationRate;

}



