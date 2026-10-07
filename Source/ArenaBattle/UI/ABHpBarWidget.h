// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABUserWidget.h"
#include "ABHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABHpBarWidget : public UABUserWidget
{
	GENERATED_BODY()

public:

    //UserWidget은 기본 생성자가 아니라 FObjectInitializer를 인자로 받는 생성자를 사용해야 함. UUserWidget에 기본 생성자 및 인자에 기본값 없음
    UABHpBarWidget(const FObjectInitializer& ObjectInitializer);

    //최대 체력 값 설정 함수
    FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }

    //HpBar에 게이지를 설정하기 위한 함수
    void UpdateHpBar(float NewCurrentHp);

protected:
    //umg가 초기화될 때 호출되는 함수
    virtual void NativeConstruct() override;



protected:
    //HP 게이지를 보여주기 위한 프로그레스 바 참조 변수
    UPROPERTY()
    TObjectPtr<class UProgressBar> HpProgressBar;
	
    //최대 체력 값
    UPROPERTY()
    float MaxHp;
        
};
