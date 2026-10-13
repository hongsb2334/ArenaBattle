// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"
#include <Interface/ABCharacterAIInterface.h>
#include <AIController.h>

UBTTask_Attack::UBTTask_Attack()
{
    //노드 이름 설정
    NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    //공격 수행 했다는 메시지를 npc에 전달 
    APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

    if (nullptr == ControllingPawn)
    {
        return EBTNodeResult::Failed;
    }


    //메시지 전달을 위해 인터페이스 형변환
    IABCharacterAIInterface* AIPawn = Cast<IABCharacterAIInterface>(ControllingPawn);
    if (nullptr == AIPawn )
    {
        return EBTNodeResult::Failed;
    }


    //공격이 종료될 때 실행될 로직을 델리게이트로 전달
    FAICharacterAttackFinished OnAttackFinished;

    //델리게이트에 로직 바인딩
    OnAttackFinished.BindLambda([this, &OwnerComp]()
        {
            //공격이 종료되면 최종 태스크 종료
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        }
    );
    
    //npc 캐릭터에 공격 종료시 실행할 로직 전달
    AIPawn->SetAIAttackDelegate(OnAttackFinished);

    //공격 메시지 전달
    AIPawn->AttackByAI();



    //공격 명령을 실행한 후에 곧바로 종료해도되는지 모름
    //따라서 InProgress(진행중) 반환

    return EBTNodeResult::InProgress;
}
