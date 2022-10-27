#pragma once
//이미지 RECT 구하는 클래스
class ANIINFO {
public:
	int x;
	int y;
	int width;
	int height;
	DWORD ani_time;
};
//물건 클래스
class ITEM {
public:
	//아이템이 창에 뜰 x, y좌표
	int x = 20;
	int y = 170;

	CMyImage Item;

	void LoadImage(char *filename)
	{
		Item.Load(filename);
	}
};
//손님 움직이는 클래스
class PEOPLE {
public:
	int p_x = 23;
	int p_y = 310;

	CMyImage people;

	void LoadImage(char *filename)
	{
		people.Load(filename);
	}
};
//캐릭터 움직이는 클래스
class CHARACTER {
public:
	//캐릭터 이동방향
	bool RIGHT = true; //시작시 오른쪽으로 뛰는 모션이 나오도록 true로 해놓음
	bool LEFT = false;

	int g_x = 20;
	int g_y = 430;

	CMyImage image;
	
	ANIINFO ani[50];//RECT의 갯수

	int index = 0;//애니메이션 프레임
	int nFrame;//프레임 갯수

	float speed;

	void LoadImage(char *filename)
	{
		image.Load(filename); //원래 : g_myimage.Load("./Data/Image/metalslug.png");
	}

	void LoadAniinfo(char *filename)
	{
		FILE *fp;

		fopen_s(&fp, filename, "rt"); //현재 폴더(vcxproj가 있는 폴더)에서 (")ani_data_01 이름의 txt(")​​ / 원래 : fopen_s(&fp, "ani_data_01.txt", "rt");
		fscanf_s(fp, "%d", &nFrame); //fp파일에서 정수 값을 받아와서 g_nFrame에 넣기​ / 원래 : g_nFrame
		
		for (int i = 0; i < nFrame; i++)
		{
			//메모장 정수들 사이에 공백이 있기 때문에 fscanf할 때 역시 공백 + %d하여 정상적으로 정수 받기
			fscanf_s(fp, "%d %d %d %d %d", &ani[i].x, &ani[i].y, &ani[i].width, &ani[i].height, &ani[i].ani_time);
			//ani[i(배열순서(:ㅡ프레임))].받아올 정보 이름을 적어 %d에 값을 참조(&)하여 정수 값 넣기
		}
		fclose(fp); //fp파일 닫기
	}

	void Move(int xDir, DWORD deltaTime)
	{
		g_x += (xDir * speed * deltaTime / 1000); //xDir(+냐 -냐)의 값에 따라 부호를 바꿔 방향을 바꾼다
	}
};