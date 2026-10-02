// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AB_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UAB_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
    UAB_AnimInstance();

protected:
    //애니메이션 초기화할 때 호출되는 함수
    virtual void NativeInitializeAnimation() override;

    //애니메이션을 업데이트할때 프레임마다 호출되는 함수
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
protected:
    //오너를 저장해두고 재활용
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    TObjectPtr<class ACharacter> Owner;

    //캐릭터 무브먼트 컴포넌트도 재활용
    // -> Velocity 읽을 때 반복적으로 사용
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    TObjectPtr<class UCharacterMovementComponent> Movement;
    
    //Idle<->Move 전환을 위해 사용할 속도
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    FVector Velocity;

    //이동 빠르기(블렌드 스페이스에 적용)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    float GroundSpeed;

    //이동 중인지 멈췄는지 확인할 변수
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    uint32 bIsIdle : 1;

    //멈췄다가 이동할 지 판단할 때 사용할 문턱 값
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    float MovingThreshold;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    uint32 bIsFalling : 1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    uint32 bIsJumping : 1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    float JumpingThreshold;
;};
