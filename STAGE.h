#pragma once
//�������� 1, 2���� �������� ���̴� ���� Ŭ����
class STAGE {
public:
	//����, �Ҹ��� ī����
	int GoodPeople;
	int BadPeople;
	//���� ���� ī����
	int GoodChose;

	//� �մ��� ����ؾ��ϴ��� ���� �޾� �Ǵ��ϴ� ��������
	int PrintPeoPle[5];

	//�������� 1�� ���̵�
	bool stage_1 = true;
	bool stage_2 = false;
	bool stage_3 = false;

	//������ ȭ���� ������, �� ������
	bool OutItem = false;

	//������ �մ��� ��� �Ǿ��°��� �Ǵ�
	bool LastGoodPeo = false;
	bool fullGoodPeo[3] = { false, false, false };

	//���� �͵� Ʋ�� �͵� ���� ȭ���� �������� BadPeople�� ������Ű�� �ϱ� ���� ��
	bool notGood = false;
	bool notBad = false;

	bool ExitClick = false;

	//���� ������ �����ߴ��� �ƴ����� �Ǵ�
	bool ItemClick[6] = { false, false, false, false, false, false };

	//���� �߾����� ���߾�����. �ߺ� ���� ��������
	bool checkChose[7] = { false, false, false, false, false, false, false };

	//���� ������ ���������� 1~5��° �մ� �׸���
	bool GoodPeo[5] = { false, false, false, false, false };

	//�������� 1, 2 ���� ����
	bool Clear_Stage = false;

	//�������� 1, 2
	bool p_Stage = false;
};