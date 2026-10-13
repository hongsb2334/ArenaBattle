#pragma once

#include <CoreMinimal.h>
#include <Engine/DataTable.h>
#include <ABCharacterStat.generated.h>

USTRUCT(BlueprintType)
struct FABCharacterStat : public FTableRowBase
{
    GENERATED_BODY()
public:
    FABCharacterStat() : MaxHp(0.0f), Attack(0.0f), AttackRange(0.0f), AttackSpeed(0.0f), MovementSpeed(0.0f)
    { }

    //덧셈 연산자 오버로딩
    //부모인 FTableRowBase에 가상함수가 있어서 구조체 맨 앞에 vtable 포인터가 들어감
    //this를 float 배열로 보고 sizeof 전체를 더하면 vtable 포인터까지 덮어쓰므로 속성별로 더함
    FABCharacterStat operator+(const FABCharacterStat& other) const
    {
        
        //포인터 사용
        //현재 데이터의 포인터, 위험함
        const float* const ThisPtr = reinterpret_cast<const float* const>(this);
        const float* const OtherPtr = reinterpret_cast<const float* const>(&other);

        //결과 저장할 구조체 변수
        FABCharacterStat Result;

        //결과 구조체 포인터
        float* ResultPtr = reinterpret_cast<float*>(&Result);

        //구조체에 포함된 float 변수의 개수
        int32 StatNum = sizeof(FABCharacterStat) / sizeof(float);

        //루프 순회를 통한 덧셈 처리
        for (int32 i = 0; i < StatNum; ++i)
        {
            ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
        }

        //결과 반환
        return Result;
        
        

        ////결과 저장할 구조체 변수
        //FABCharacterStat Result;

        //Result.MaxHp = MaxHp + Other.MaxHp;
        //Result.Attack = Attack + Other.Attack;
        //Result.AttackRange = AttackRange + Other.AttackRange;
        //Result.AttackSpeed = AttackSpeed + Other.AttackSpeed;
        //Result.MovementSpeed = MovementSpeed + Other.MovementSpeed;

        ////결과 반환
        //return Result;


    }

public:
    //스탯에서 사용할 속성
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
    float MaxHp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
    float Attack;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
    float AttackRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
    float AttackSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
    float MovementSpeed;

};