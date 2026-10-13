// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/ABGameSingleton.h"

//로그 카테고리 정의
DEFINE_LOG_CATEGORY(LogABGameSingleton);

UABGameSingleton::UABGameSingleton()
{
    //스탯 데이터 테이블 에셋을 읽어서 데이터 배열로 구성

    //데이터 테이블 에셋 로드
    static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(
        TEXT("/Game/ArenaBattle/GameData/ABCharacterStatTable.ABCharacterStatTable")
    );

    //로드 성공하면 이어서 처리
    if (DataTableRef.Succeeded())
    {
        //UDataTable은 데이터를 내부에서 TMap<FName, uint8*>로 관리
        const UDataTable* DataTable = DataTableRef.Object;

        //로드한 데이터 확인
        ensureAlways(DataTable->GetRowMap().Num() > 0);

        TArray<uint8*> ValueArray;

        //전체 Row행 데이터 가져오기
        DataTable->GetRowMap().GenerateValueArray(ValueArray);

        //타입 형변환
        for (uint8* Value : ValueArray)
        {
            CharacterStatTable.Add(*reinterpret_cast<FABCharacterStat*>(Value));
        }
        
        //스탯 테이블의 원소(데이터) 개수를 최대 레벨로 설정
        CharacterMaxLevel = CharacterStatTable.Num();


        //값 확인
        ensureAlways(CharacterStatTable.Num() > 0);
    }
}

UABGameSingleton& UABGameSingleton::Get()
{
    //언리얼을 통해서 인스턴스(싱글톤 객체) 값 가져오기
    //좀더 강한 체크가 필요하면 CastChecked 사용
    UABGameSingleton* Instance = CastChecked<UABGameSingleton>(GEngine->GameSingleton);

    //if문 안에는 반드시 무언가를 리턴해야 함. 함수의 리턴값이 레퍼런스기 때문에 nullptr을 리턴할 수 없음
    //이후에 밑으로 내려가게 되면 Instance가 nullptr이므로 오류상황으로 판단하고 NewObject로 깡인스턴스 만들어서 반환하게 함.
    //CastChecked는 null이거나 타입이 다르면 그 자리에서 assert로 정지되서 Cast보다 강력함

    if (Instance)
    {
        return *Instance;
    }

    //여기에 오면 오류 상황
    UE_LOG(LogABGameSingleton, Error, TEXT("Invalid Game Singleton"));
    
    //반환이 필요하기 때문에 추가하는 코드, 여기 접근되면 안됨
    return *NewObject<UABGameSingleton>();
}
