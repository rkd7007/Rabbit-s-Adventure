#pragma once
//스테이지 1, 2에서 공통으로 쓰이는 변수 클래스
class STAGE {
public:
	//만족, 불만족 카운터
	int GoodPeople;
	int BadPeople;
	//옳은 선택 카운터
	int GoodChose;

	//어떤 손님을 출력해야하는지 값을 받아 판단하는 전역변수
	int PrintPeoPle[5];

	//스테이지 1의 난이도
	bool stage_1 = true;
	bool stage_2 = false;
	bool stage_3 = false;

	//물건이 화면을 나갔냐, 안 나갔냐
	bool OutItem = false;

	//마지막 손님이 출력 되었는가를 판단
	bool LastGoodPeo = false;
	bool fullGoodPeo[3] = { false, false, false };

	//맞춘 것도 틀린 것도 없이 화면을 나갔으면 BadPeople를 증가시키게 하기 위한 것
	bool notGood = false;
	bool notBad = false;

	bool ExitClick = false;

	//옳은 물건을 선택했는지 아닌지를 판단
	bool ItemClick[6] = { false, false, false, false, false, false };

	//선택 했었는지 안했었는지. 중복 선택 방지위함
	bool checkChose[7] = { false, false, false, false, false, false, false };

	//옳은 물건을 선택했으면 1~5번째 손님 그리기
	bool GoodPeo[5] = { false, false, false, false, false };

	//스테이지 1, 2 성공 여부
	bool Clear_Stage = false;

	//스테이지 1, 2
	bool p_Stage = false;
};