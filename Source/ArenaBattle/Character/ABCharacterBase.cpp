// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterBase.h"
#include "ABCharacterData.h"
#include "ABComboActionData.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/CapsuleComponent.h>
#include <Physics/ABCollision.h>
#include <Engine/DamageEvents.h>
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

    //콜리전 설정
    GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_ABCAPSULE);
    GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

    //몽타주 및 콤보 액션 데이터 에셋 지정
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Game/ArenaBattle/Animation/AM_ComboAttack.AM_ComboAttack"));

    if (ComboActionMontageRef.Succeeded())
    {
        ComboAttackMontage = ComboActionMontageRef.Object;
    }


    static ConstructorHelpers::FObjectFinder<UABComboActionData> ComboActionDataRef(TEXT("/Game/ArenaBattle/ComboData/ABA_ComboAction.ABA_ComboAction"));

    if (ComboActionDataRef.Succeeded())
    {
        ComboActionData = ComboActionDataRef.Object;
    }


    static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Game/ArenaBattle/Animation/AM_Dead.AM_Dead"));

    if (DeadMontageRef.Succeeded())
    {
        DeadMontage = DeadMontageRef.Object;
    }

}

float AABCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);



    //데미지를 받으면 죽음 처리 함수 호출
    SetDead();
    
    return DamageAmount;
}

void AABCharacterBase::SetDead()
{
    //죽으면 움직이지 않게 처리
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

    //죽는 모션 재생
    PlayDeadAnimation();

    //콜리전 끄기
    SetActorEnableCollision(false);


}

void AABCharacterBase::PlayDeadAnimation()
{
    //애님인스턴스를 통해 죽음 몽타주 재생
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance)
    {
        //현재 재생중인 모든 몽타주 중지
        AnimInstance->StopAllMontages(0.0f);

        //재생
        AnimInstance->Montage_Play(DeadMontage, 1.0f);
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

void AABCharacterBase::ProcessComboCommand()
{
    //처음 공격 시작할 때
    if (CurrentCombo == 0)
    {
        ComboActionBegin();
        return;
    }

    //처음 공격이 아닌 경우
    //타이머 핸들의 유효성 여부로 다음 공격으로 분기 결정
    if (ComboTimerHandle.IsValid())
    {
        //입력이 제대로 들어왔다고 판정
        bHasNextComboCommand = true;
    }
    else
    {
        bHasNextComboCommand = false;
    }
}

void AABCharacterBase::ComboActionBegin()
{
    //현재 콤보 단계 1로 설정
    CurrentCombo = 1;


    //몽타주 재생
    //몽타주 재생을 위해 애님 인스턴스 가져오기
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

    if (AnimInstance)
    {
        //몽타주 재생 속도
        const float AttackSpeedRate = 1.0f;
        //몽타주 재생
        AnimInstance->Montage_Play(ComboAttackMontage, AttackSpeedRate);

        //몽타주 종료 이벤트에 함수 등록
        FOnMontageEnded OnMontageEnded;
        OnMontageEnded.BindUObject(this, &AABCharacterBase::ComboActionEnded);
        //몽타주가 종료되면 ComboActionEnded가 호출되도록 등록
        AnimInstance->Montage_SetEndDelegate(OnMontageEnded, ComboAttackMontage);

        //공격 모션 중에는 이동 안하게 처리
        GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

        //타이머핸들 초기화
        ComboTimerHandle.Invalidate();
       
        //타이머 설정
        SetComboCheckTimer();
    }
}

void AABCharacterBase::ComboActionEnded(UAnimMontage* TargetMontage, bool bInterrupted)
{
    ensureAlways(CurrentCombo > 0);

    //콤보 단계 초기화
    CurrentCombo = 0;


    //몽타주 재생이 종료되면 캐릭터 이동 복구
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AABCharacterBase::SetComboCheckTimer()
{
    //현재 재생중인 콤보 단계의 인덱스 계산
    const int32 ComboIndex = CurrentCombo - 1;

    //인덱스 값 확인
    ensureAlways(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

    //애니메이션 재생속도
    //2배속 등 배속까지 계산
    const float AttackSpeedRate = 1.0f;

    //콤보 공격 입력 시간(초단위) 계산
    float ComboEffectTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate / AttackSpeedRate);

    //타이머 설정
    if (ComboEffectTime > 0)
    {
        GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AABCharacterBase::ComboCheck, ComboEffectTime, false);
    }


}

void AABCharacterBase::ComboCheck()
{
    //타이머 핸들 리셋
    ComboTimerHandle.Invalidate();

    //콤보 타이머 이전에 공격 입력이 제대로 들어왔는지 확인(분기 처리)
    if (bHasNextComboCommand)
    {
        //몽타주 점프 처리를 위해 애님 인스턴스 가져오기
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            //다음 단계 콤보 설정
            //CurrentCombo 카운트가 1에서 4사이로 가두는 클램프
            CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);

            //점프할 섹션 이름 구성
            //역참조 연산자 넣어서 TCHAR로 변환
            FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
            
            //몽타주 섹션 점프
            AnimInstance->Montage_JumpToSection(NextSection, ComboAttackMontage);

            //타이머 재설정
            SetComboCheckTimer();

            // 콤보 처리에 사용한 값 초기화
            bHasNextComboCommand = false;
            
        }
    }


}

void AABCharacterBase::AttackHitCheck()
{
    
    //콜리전 쿼리 파라미터
    FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

    //공격 범위
    const float AttackRange = 120.0f;

    //트레이스에 사용할 구체의 반지름
    const float AttackRadius = 20.0f;

    //트레이스 시작 위치
    //액터의 위치 + 캡슐 높이의 반지름 만큼 앞으로 떨어지는 위치
    FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();

    //트레이스 종료 위치
    //시작위치 + 공격 범위 만큼 앞으로 떨어진 위치
    FVector End = Start + GetActorForwardVector() * AttackRange;
 
    //트레이스를 활용한 충돌 확인
    FHitResult OutHitResult;
    bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_ABACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
    
    //충돌이 감지되면 데미지 전달
    if (HitDetected)
    {
        //전달할 데미지
        const float AttackDamage = 30.0f;

        //데미지 이벤트 변수
        FDamageEvent DamageEvent;

        //TakeDamage 함수 호출해서 데미지 전달
        OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
    }

    //시각적으로 충돌 여부를 확인할 수 있도록 디버깅 기능 활용
#if ENABLE_DRAW_DEBUG
    
    //캡슐의 중심 위치
    //(End - Start): Start 위치에서 End위치로 향하는 벡터
    FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
    
    //캡슐 높이의 절반
    const float CapsuleHalfHeight = AttackRange * 0.5f;

    //표시할 색상(맞았으면 빨간색, 안맞았으면 녹색)
    const FColor DrawColor = HitDetected ? FColor::Red : FColor::Green;
    
    //캡슐 그리기
    DrawDebugCapsule(
        GetWorld(), 
        CapsuleOrigin, 
        CapsuleHalfHeight, 
        AttackRadius, 
        FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), 
        DrawColor,
        false,
        5.0f
    );

#endif


}



