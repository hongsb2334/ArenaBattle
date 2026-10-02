// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABCharacterData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABCharacterData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
    UABCharacterData();


    //속성
    
    //폰에 설정할 회전 속성
    UPROPERTY(EditAnywhere, Category = "Pawn")
    uint32 bUseControllerRotationYaw : 1;

    //캐릭터 무브먼트에 설정할 회전 관련 속성
    UPROPERTY(EditAnywhere, Category = "CharacterMovement")
    uint32 bUseControllerDesiredRotation : 1;

    UPROPERTY(EditAnywhere, Category = "CharacterMovement")
    uint32 bUseOrientToMovement: 1;

    UPROPERTY(EditAnywhere, Category = "CharacterMovement")
    FRotator RotationRate;

    //사용할 입력 매핑 컨텍스트 에셋
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    TObjectPtr<class UInputMappingContext> InputMappingContext;


    //스프링암 관련 속성
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    float TargetArmLength;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm")
    FRotator RelativeRotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm")
    uint32 bDoCollisionTest : 1;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm")
    uint32 bUsePawnControlRotation: 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm")
    uint32 bInheritPitch : 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm")
    uint32 bInheritYaw : 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm")
    uint32 bInheritRoll : 1;
};
