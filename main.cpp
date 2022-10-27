#include "MyImage.h"
#include "DSpriteManager.h"
#include "MyCharacter.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <Windows.h>
#pragma comment(lib, "winmm.lib")
#include <MMSystem.h>
#include "mmsystem.h"
#include "Digitalv.h"
#include "STAGE.h"

//사운드
MCI_OPEN_PARMS m_mciOpenParms;
MCI_PLAY_PARMS m_mciPlayParms;
DWORD m_dwDeviceID;
MCI_OPEN_PARMS mciOpen;
MCI_PLAY_PARMS mciPlay;

int dwID;

Graphics* g_BackBuffer;
Graphics* g_MainBuffer;
Bitmap* g_Bitmap;

CMyImage PlayBar;//시작 UI 이미지
CMyImage ExitBar;//나가기 UI 이미지
CMyImage g_BackImage;//메뉴 이미지
CMyImage MainImage;//스테이지 배경 이미지
CMyImage CounterImage;//스테이지 1 이미지
CMyImage CounterText;//스테이지 1 게임방법
CMyImage Cafe;//스테이지 2 이미지
CMyImage CafeText;//스테이지 2 게임방법

RECT PLAY = { 430, 240, 680, 300 }; //시작 버튼
RECT EXIT = { 430, 500, 680, 560 }; //나가기 버튼
RECT STAGE_1 = { 75, 570, 140, 680 }; //스테이지 1 버튼
RECT STAGE_2 = { 910, 570, 980, 680 }; //스테이지 2 버튼
RECT EXIT_1 = { 925, 100, 975, 150 };//게임방법 1 EXIT 버튼
RECT EXIT_2 = { 420, 135, 470, 185 };//게임방법 2 EXIT 버튼

PEOPLE g_People;//손님 이미지
//작은 손님 이미지
PEOPLE Hiper;
PEOPLE Flower;
PEOPLE Squid;
PEOPLE Woman;
PEOPLE Turtle;
PEOPLE Fish;

ITEM g_item[10];//스테이지 1과 2의 아이템들(물건)

//스테이지 1, 2
STAGE s_Stage1;
STAGE s_Stage2;

//PLAY 눌렀는지 안 눌렀는지
bool InGame = false;

//스테이지 1 자리인지 2 자리인지
bool Stagelocation_1 = true;
bool Stagelocation_2 = false;

//물건 RECT l t r b
//스테이지 1
RECT Item1 = { 22, 26, 150, 210 };
RECT Item2 = { 20, 235, 150, 405 };
RECT Item3 = { 17, 438, 153, 555 };
RECT Item4 = { 910, 46, 1065, 165 };
RECT Item5 = { 910, 217, 1065, 345 };
RECT Item6 = { 910, 391, 1065, 515 };
//스테이지 2
RECT Item7 = { 23, 10, 200, 155 }; //과자
RECT Item8 = { 45, 190, 165, 325 }; //얼음
RECT Item9 = { 50, 365, 160, 555 }; //가루
RECT Item10 = { 47, 600, 180, 740 }; //커피콩
RECT Item11 = { 960, 15, 1030, 235 }; //음료수
RECT Item12 = { 945, 265, 1045, 485 }; //우유
RECT Item13 = { 960, 515, 1035, 750 }; //물

//캐릭터 이미지
CHARACTER g_mychar;

DWORD g_duration = 0; //현재까지 흐른 시간을 보여주는 전역변수
DWORD g_Interval = 0; //시간 값을 저장할 전역변수
DWORD g_NextStage = 0; //다음 스테이지로 넘어가는 시간 조절하는 전역변수
DWORD g_DirSound = 0; //뛰는 효과음 나오는 시간 조절하는 전역변수
DWORD g_checkplay = 0; //게임시작으로 넘어갈때 시간을 조절하는 전역변수

int num;//랜덤으로 받을 값을 저장하는 변수
int i_x, i_y, i_w, i_h;//아이템의 x, y, w, h 크기
int X, Y, W, H;//손님의 x, y, w, h 크기

void OnUpdate(HWND hWnd, DWORD tick);
void CreateBuffer(HWND hWnd, HDC hDC);
void ReleaseBuffer(HWND hWnd, HDC hDC);

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS   wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = NULL;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = "API";

	if (RegisterClass(&wndclass) == 0)
	{
		return 0;
	}
	//ClassName, GmaeName, 일반적인 윈도우 형태로 만들어라, 창 생성시 x위치, y위치, 창 x크기, y크기, 메뉴유무, 툴바유무 등)
	HWND hwnd = CreateWindow("API", "카운터 알바 게임", WS_OVERLAPPEDWINDOW, 400, 100, 1100, 800, NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		return 0;
	}

	HDC hDC = GetDC(hwnd);

	CreateBuffer(hwnd, hDC);

	PlayBar.Load("./Data/Image/play.png");//시작 바 이미지를 가져온다
	ExitBar.Load("./Data/Image/exit.png");//나가기 바 이미지를 가져온다

	g_BackImage.Load("./Data/Image/PlayMenu.png");//메뉴 이미지를 가져온다

	MainImage.Load("./Data/Image/MainImage.png");//스테이지 배경 이미지를 가져온다

	CounterImage.Load("./Data/Image/Convenience store.png");//스테이지 1 이미지를 가져온다
	CounterText.Load("./Data/Image/text1.png");//스테이지 1 게임방법 이미지
	Cafe.Load("./Data/Image/Cafe.png");//스테이지 2 이미지를 가져온다
	CafeText.Load("./Data/Image/text2.png");//스테이지 2 게임방법 이미지

	g_People.LoadImage("./Data/Image/people.png");//손님 이미지를 가져온다

	//여섯개의 물건 이미지를 가져온다
	//스테이지 1
	g_item[0].LoadImage("./Data/Image/bread.png"); //빵	
	g_item[1].LoadImage("./Data/Image/Burger.png"); //햄버거	
	g_item[2].LoadImage("./Data/Image/juice.png"); //주스	
	g_item[3].LoadImage("./Data/Image/meat.png"); //고기
	g_item[4].LoadImage("./Data/Image/milk.png"); //우유
	g_item[5].LoadImage("./Data/Image/snack.png"); //과자
	//스테이지 2
	g_item[6].LoadImage("./Data/Image/coffee.png"); //커피
	g_item[7].LoadImage("./Data/Image/aide.png"); //에이드
	g_item[8].LoadImage("./Data/Image/Java chip.png"); //자바칩
	g_item[9].LoadImage("./Data/Image/Cafe latte.png"); //카페라떼

	//작은 손님 이미지를 가져온다
	Hiper.LoadImage("./Data/Image/people_1.png"); //히퍼
	Flower.LoadImage("./Data/Image/people_2.png"); //꽃
	Squid.LoadImage("./Data/Image/people_3.png"); //오징어
	Woman.LoadImage("./Data/Image/people_4.png"); //빨간 여자
	Turtle.LoadImage("./Data/Image/people_5.png"); //구름탄 거북이
	Fish.LoadImage("./Data/Image/people_6.png"); //물고기

	//주인공 오른쪽 이미지를 가져온다
	g_mychar.LoadImage("./Data/Image/Char3.png"); //원래 : g_myimage.Load("./Data/Image/Char.png"​);

	ShowWindow(hwnd, nCmdShow);

	UpdateWindow(hwnd);

	//주인공 애니메이션
	g_mychar.LoadAniinfo("ani_data_01.txt");

	//사운드
	mciOpen.lpstrElementName = "main.mp3"; //메인
	mciOpen.lpstrDeviceType = "mpegvideo";
	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)& mciOpen);
	dwID = mciOpen.wDeviceID;
	mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)& m_mciPlayParms);

	MSG msg;

	DWORD tick = GetTickCount();
	g_mychar.speed = 200; //주인공 스피드 : 130

	while (1)
	{
		//윈도우 메세지가 있을경우 메세지를 처리한다
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else //메세지가 없을 경우 게임 루프를 실행한다. -> 게임부분
		{
			char buff[128];

			DWORD curTick = GetTickCount();
			DWORD deltatime = curTick - tick; //curTick > tick

			g_checkplay += tick;

			OnUpdate(hwnd, deltatime);
			tick = curTick;

			g_duration += deltatime;

			g_MainBuffer->DrawImage(g_Bitmap, 0, 0);
		}
	}
	ReleaseBuffer(hwnd, hDC);

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (msg)
	{
	case WM_KEYDOWN:
	{
		g_duration = 0; //현재까지 누적된 시간을 보여주는 것
	}
	break;
	case WM_CREATE:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN: //왼쪽 마우스 클릭
		int xPos = LOWORD(lParam); //마우스를 누른 x좌표 값 저장
		int yPos = HIWORD(lParam); //마우스를 누른 y좌표 값 저장
		//메뉴화면이면
		if (InGame == false)
		{
			//시작 UI를 눌렀다면
			if ((PLAY.left <= xPos) && (PLAY.right >= xPos) && (PLAY.top <= yPos) && (PLAY.bottom >= yPos))
			{
				MessageBox(hWnd, "강토끼씨는 등록금을 마련하기 위해 오늘도 알바를 하러 갑니다\n※ 좌, 우로 움직여 원하는 스테이지 위에서 스테이지를 클릭하세요 ※", "카운터 알바 게임", MB_ICONASTERISK | MB_OK);
				InGame = true;
			}
			//나가기 UI를 눌렀다면
			if ((EXIT.left <= xPos) && (EXIT.right >= xPos) && (EXIT.top <= yPos) && (EXIT.bottom >= yPos))
			{
				MessageBox(hWnd, "강토끼씨는 등록금을 내지못해 자퇴하게 되었습니다", "카운터 알바 게임", MB_ICONERROR | MB_OK);
				exit(0);
			}
		}
		//스테이지 화면이고 스테이지 1과 2가 아니면
		if (InGame == true && s_Stage1.p_Stage == false && s_Stage2.p_Stage == false)
		{
			//스테이지 1을 클리어 하지 않고 스테이지 1을 눌렀으면
			if (s_Stage1.Clear_Stage == false && Stagelocation_1 == true && (STAGE_1.left <= xPos) && (STAGE_1.right >= xPos) && (STAGE_1.top <= yPos) && (STAGE_1.bottom >= yPos))
			{
				//ok를 눌렀다면
				if (MessageBox(hWnd, "편의점으로 들어가시겠습니까?", "카운터 알바 게임", MB_ICONQUESTION | MB_YESNO) == IDYES)
				{
					s_Stage1.ExitClick = true;
				}
				//no를 눌렀다면
				else
				{
					g_mychar.g_x = 20;
				}
			}
			//스테이지 1을 클리어 하고 스테이지 1을 눌렀으면
			else if (s_Stage1.Clear_Stage == true && Stagelocation_1 == true && (STAGE_1.left <= xPos) && (STAGE_1.right >= xPos) && (STAGE_1.top <= yPos) && (STAGE_1.bottom >= yPos))
			{
				MessageBox(hWnd, "▶ 입장 불가 ◀\n※ 강토끼씨는 이미 편의점 알바를 끝냈습니다 ※", "카운터 알바 게임", MB_ICONWARNING | MB_OK);
			}
			//스테이지 2를 클리어 하지 않고 스테이지 2를 눌렀으면
			if (s_Stage2.Clear_Stage == false && Stagelocation_2 == true && (STAGE_2.left <= xPos) && (STAGE_2.right >= xPos) && (STAGE_2.top <= yPos) && (STAGE_2.bottom >= yPos))
			{
				//ok를 눌렀다면
				if (MessageBox(hWnd, "카페로 들어가시겠습니까?", "카운터 알바 게임", MB_ICONQUESTION | MB_YESNO) == IDYES)
				{
					s_Stage2.ExitClick = true;
				}
				//no를 눌렀다면
				else
				{
					g_mychar.g_x = 860;
				}
			}
			//스테이지 2를 클리어 하고 스테이지 2를 눌렀으면
			else if (s_Stage2.Clear_Stage == true && Stagelocation_2 == true && (STAGE_2.left <= xPos) && (STAGE_2.right >= xPos) && (STAGE_2.top <= yPos) && (STAGE_2.bottom >= yPos))
			{
				MessageBox(hWnd, "▶ 입장 불가 ◀\n※ 강토끼씨는 이미 카페 알바를 끝냈습니다 ※", "카운터 알바 게임", MB_ICONWARNING | MB_OK);
			}
		}
		//x를 눌렀으면 게임 시작
		if (s_Stage1.ExitClick == true)
		{
			if ((EXIT_1.left <= xPos) && (EXIT_1.right >= xPos) && (EXIT_1.top <= yPos) && (EXIT_1.bottom >= yPos))
			{
				s_Stage1.ExitClick = false;
				g_mychar.g_x = 0;
				mciSendCommandW(dwID, MCI_CLOSE, 0, NULL);	
				g_checkplay = 0;
				s_Stage1.p_Stage = true;
			}
		}	
		if (s_Stage1.p_Stage == true && (g_checkplay >= 100))
		{
			//빵
			if (s_Stage1.checkChose[0] == false && (Item1.left <= xPos) && (Item1.right >= xPos) && (Item1.top <= yPos) && (Item1.bottom >= yPos))
			{
				if (s_Stage1.ItemClick[5] == true)
				{
					sndPlaySoundA("goodclick.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);

					s_Stage1.notGood = true;
					s_Stage1.GoodPeople += 1;

					//중복 선택 안되게 만듦
					s_Stage1.checkChose[0] = true;

					if (s_Stage1.GoodPeople == 1) { s_Stage1.GoodPeo[0] = true; s_Stage1.PrintPeoPle[0] = 1; }
					if (s_Stage1.GoodPeople == 2) { s_Stage1.GoodPeo[1] = true; s_Stage1.PrintPeoPle[1] = 1; }
					if (s_Stage1.GoodPeople == 3) { s_Stage1.GoodPeo[2] = true; s_Stage1.PrintPeoPle[2] = 1; }
					if (s_Stage1.GoodPeople == 4) { s_Stage1.GoodPeo[3] = true; s_Stage1.PrintPeoPle[3] = 1; }
					if (s_Stage1.GoodPeople == 5) { s_Stage1.GoodPeo[4] = true; s_Stage1.PrintPeoPle[4] = 1; }
				}
				if (s_Stage1.ItemClick[5] == false)
				{
					sndPlaySoundA("badclick.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					s_Stage1.notBad = true;
					s_Stage1.BadPeople += 1;
				}
			}
			//주스
			if (s_Stage1.checkChose[1] == false && (Item3.left <= xPos) && (Item3.right >= xPos) && (Item3.top <= yPos) && (Item3.bottom >= yPos))
			{
				if (s_Stage1.ItemClick[3] == true)
				{
					sndPlaySoundA("goodclick.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);

					s_Stage1.notGood = true;
					s_Stage1.GoodPeople += 1;

					s_Stage1.checkChose[1] = true;

					if (s_Stage1.GoodPeople == 1) { s_Stage1.GoodPeo[0] = true; s_Stage1.PrintPeoPle[0] = 2; }
					if (s_Stage1.GoodPeople == 2) { s_Stage1.GoodPeo[1] = true; s_Stage1.PrintPeoPle[1] = 2; }
					if (s_Stage1.GoodPeople == 3) { s_Stage1.GoodPeo[2] = true; s_Stage1.PrintPeoPle[2] = 2; }
					if (s_Stage1.GoodPeople == 4) { s_Stage1.GoodPeo[3] = true; s_Stage1.PrintPeoPle[3] = 2; }
					if (s_Stage1.GoodPeople == 5) { s_Stage1.GoodPeo[4] = true; s_Stage1.PrintPeoPle[4] = 2; }
				}
				if (s_Stage1.ItemClick[3] == false)
				{
					sndPlaySoundA("badclick.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);

					s_Stage1.notBad = true;
					s_Stage1.BadPeople += 1;
				}
			}
			//햄버거
			if (s_Stage1.checkChose[2] == false && (Item2.left <= xPos) && (Item2.right >= xPos) && (Item2.top <= yPos) && (Item2.bottom >= yPos))
			{
				if (s_Stage1.ItemClick[4] == true)
				{
					sndPlaySoundA("goodclick.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);

					s_Stage1.notGood = true;
					s_Stage1.GoodPeople += 1;

					s_Stage1.checkChose[2] = true;

					if (s_Stage1.GoodPeople == 1) { s_Stage1.GoodPeo[0] = true; s_Stage1.PrintPeoPle[0] = 3; }
					if (s_Stage1.GoodPeople == 2) { s_Stage1.GoodPeo[1] = true; s_Stage1.PrintPeoPle[1] = 3; }
					if (s_Stage1.GoodPeople == 3) { s_Stage1.GoodPeo[2] = true; s_Stage1.PrintPeoPle[2] = 3; }
					if (s_Stage1.GoodPeople == 4) { s_Stage1.GoodPeo[3] = true; s_Stage1.PrintPeoPle[3] = 3; }
					if (s_Stage1.GoodPeople == 5) { s_Stage1.GoodPeo[4] = true; s_Stage1.PrintPeoPle[4] = 3; }
				}
				if (s_Stage1.ItemClick[4] == false)
				{
					sndPlaySoundA("badclick.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);

					s_Stage1.notBad = true;
					s_Stage1.BadPeople += 1;
				}
			}
			//고기
			if (s_Stage1.checkChose[3] == false && (Item4.left <= xPos) && (Item4.right >= xPos) && (Item4.top <= yPos) && (Item4.bottom >= yPos))
			{
				if (s_Stage1.ItemClick[1] == true)
				{
					sndPlaySoundA("goodclick.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);

					s_Stage1.notGood = true;
					s_Stage1.GoodPeople += 1;

					s_Stage1.checkChose[3] = true;

					if (s_Stage1.GoodPeople == 1) { s_Stage1.GoodPeo[0] = true; s_Stage1.PrintPeoPle[0] = 4; }
					if (s_Stage1.GoodPeople == 2) { s_Stage1.GoodPeo[1] = true; s_Stage1.PrintPeoPle[1] = 4; }
					if (s_Stage1.GoodPeople == 3) { s_Stage1.GoodPeo[2] = true; s_Stage1.PrintPeoPle[2] = 4; }
					if (s_Stage1.GoodPeople == 4) { s_Stage1.GoodPeo[3] = true; s_Stage1.PrintPeoPle[3] = 4; }
					if (s_Stage1.GoodPeople == 5) { s_Stage1.GoodPeo[4] = true; s_Stage1.PrintPeoPle[4] = 4; }
				}
				if (s_Stage1.ItemClick[1] == false)
				{
					sndPlaySoundA("badclick.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);

					s_Stage1.notBad = true;
					s_Stage1.BadPeople += 1;
				}
			}
			//우유
			if (s_Stage1.checkChose[4] == false && (Item5.left <= xPos) && (Item5.right >= xPos) && (Item5.top <= yPos) && (Item5.bottom >= yPos))
			{
				if (s_Stage1.ItemClick[2] == true)
				{
					sndPlaySoundA("goodclick.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);

					s_Stage1.notGood = true;
					s_Stage1.GoodPeople += 1;

					s_Stage1.checkChose[4] = true;

					if (s_Stage1.GoodPeople == 1) { s_Stage1.GoodPeo[0] = true; s_Stage1.PrintPeoPle[0] = 5; }
					if (s_Stage1.GoodPeople == 2) { s_Stage1.GoodPeo[1] = true; s_Stage1.PrintPeoPle[1] = 5; }
					if (s_Stage1.GoodPeople == 3) { s_Stage1.GoodPeo[2] = true; s_Stage1.PrintPeoPle[2] = 5; }
					if (s_Stage1.GoodPeople == 4) { s_Stage1.GoodPeo[3] = true; s_Stage1.PrintPeoPle[3] = 5; }
					if (s_Stage1.GoodPeople == 5) { s_Stage1.GoodPeo[4] = true; s_Stage1.PrintPeoPle[4] = 5; }
				}
				if (s_Stage1.ItemClick[2] == false)
				{
					sndPlaySoundA("badclick.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);

					s_Stage1.notBad = true;
					s_Stage1.BadPeople += 1;
				}
			}
			//과자
			if (s_Stage1.checkChose[5] == false && (Item6.left <= xPos) && (Item6.right >= xPos) && (Item6.top <= yPos) && (Item6.bottom >= yPos))
			{
				if (s_Stage1.ItemClick[0] == true)
				{
					sndPlaySoundA("goodclick.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);

					s_Stage1.notGood = true;
					s_Stage1.GoodPeople += 1;

					s_Stage1.checkChose[5] = true;

					if (s_Stage1.GoodPeople == 1) { s_Stage1.GoodPeo[0] = true; s_Stage1.PrintPeoPle[0] = 6; }
					if (s_Stage1.GoodPeople == 2) { s_Stage1.GoodPeo[1] = true; s_Stage1.PrintPeoPle[1] = 6; }
					if (s_Stage1.GoodPeople == 3) { s_Stage1.GoodPeo[2] = true; s_Stage1.PrintPeoPle[2] = 6; }
					if (s_Stage1.GoodPeople == 4) { s_Stage1.GoodPeo[3] = true; s_Stage1.PrintPeoPle[3] = 6; }
					if (s_Stage1.GoodPeople == 5) { s_Stage1.GoodPeo[4] = true; s_Stage1.PrintPeoPle[4] = 6; }
				}
				if (s_Stage1.ItemClick[0] == false)
				{
					sndPlaySoundA("badclick.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);

					s_Stage1.notBad = true;
					s_Stage1.BadPeople += 1;
				}
			}
		}
		//x를 눌렀으면 게임 시작
		if (s_Stage2.ExitClick == true)
		{
			if ((EXIT_2.left <= xPos) && (EXIT_2.right >= xPos) && (EXIT_2.top <= yPos) && (EXIT_2.bottom >= yPos))
			{
				s_Stage2.ExitClick = false;
				g_mychar.g_x = 820;
				mciSendCommandW(dwID, MCI_CLOSE, 0, NULL);
				g_checkplay = 0;
				s_Stage2.p_Stage = true;
			}
		}
		if (s_Stage2.p_Stage == true && (g_checkplay >= 100))
		{
			//아메리카노 종류일때 - 커피콩 물 얼음
			if (s_Stage2.ItemClick[0] == true)
			{
				if (s_Stage2.checkChose[3] == false && (Item10.left <= xPos) && (Item10.right >= xPos) && (Item10.top <= yPos) && (Item10.bottom >= yPos))
				{
					sndPlaySoundA("goodclick.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					s_Stage2.GoodChose += 1;

					//중복 선택 불가능하게 만듦
					s_Stage2.checkChose[3] = true;

					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 0) { s_Stage2.GoodPeo[0] = true; s_Stage2.PrintPeoPle[0] = 6; }
					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 1) { s_Stage2.GoodPeo[1] = true; s_Stage2.PrintPeoPle[1] = 6; }
					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 2) { s_Stage2.GoodPeo[2] = true; s_Stage2.PrintPeoPle[2] = 6; }
				}
				if (s_Stage2.checkChose[6] == false && (Item13.left <= xPos) && (Item13.right >= xPos) && (Item13.top <= yPos) && (Item13.bottom >= yPos))
				{
					sndPlaySoundA("goodclick.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					s_Stage2.GoodChose += 1;

					//중복 선택 불가능하게 만듦
					s_Stage2.checkChose[6] = true;

					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 0) { s_Stage2.GoodPeo[0] = true; s_Stage2.PrintPeoPle[0] = 6; }
					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 1) { s_Stage2.GoodPeo[1] = true; s_Stage2.PrintPeoPle[1] = 6; }
					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 2) { s_Stage2.GoodPeo[2] = true; s_Stage2.PrintPeoPle[2] = 6; }
				}
				if (s_Stage2.checkChose[1] == false && (Item8.left <= xPos) && (Item8.right >= xPos) && (Item8.top <= yPos) && (Item8.bottom >= yPos))
				{
					sndPlaySoundA("goodclick.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					s_Stage2.GoodChose += 1;

					//중복 선택 불가능하게 만듦
					s_Stage2.checkChose[1] = true;

					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 0) { s_Stage2.GoodPeo[0] = true; s_Stage2.PrintPeoPle[0] = 6; }
					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 1) { s_Stage2.GoodPeo[1] = true; s_Stage2.PrintPeoPle[1] = 6; }
					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 2) { s_Stage2.GoodPeo[2] = true; s_Stage2.PrintPeoPle[2] = 6; }
				}
				else if (!((Item10.left <= xPos) && (Item10.right >= xPos) && (Item10.top <= yPos) && (Item10.bottom >= yPos))
					&& !((Item13.left <= xPos) && (Item13.right >= xPos) && (Item13.top <= yPos) && (Item13.bottom >= yPos))
					&& !((Item8.left <= xPos) && (Item8.right >= xPos) && (Item8.top <= yPos) && (Item8.bottom >= yPos)))
				{
					sndPlaySoundA("badclick.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					s_Stage2.BadPeople += 1;
				}
			}	
			//에이드 종류일때 - 가루 음료 얼음
			if (s_Stage2.ItemClick[1] == true)
			{
				if (s_Stage2.checkChose[1] == false && (Item8.left <= xPos) && (Item8.right >= xPos) && (Item8.top <= yPos) && (Item8.bottom >= yPos))
				{
					sndPlaySoundA("goodclick.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					s_Stage2.GoodChose += 1;

					//중복 선택 불가능하게 만듦
					s_Stage2.checkChose[1] = true;

					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 0) { s_Stage2.GoodPeo[0] = true; s_Stage2.PrintPeoPle[0] = 4; }
					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 1) { s_Stage2.GoodPeo[1] = true; s_Stage2.PrintPeoPle[1] = 4; }
					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 2) { s_Stage2.GoodPeo[2] = true; s_Stage2.PrintPeoPle[2] = 4; }
				}
				if (s_Stage2.checkChose[4] == false && (Item11.left <= xPos) && (Item11.right >= xPos) && (Item11.top <= yPos) && (Item11.bottom >= yPos))
				{
					sndPlaySoundA("goodclick.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					s_Stage2.GoodChose += 1;

					//중복 선택 불가능하게 만듦
					s_Stage2.checkChose[4] = true;

					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 0) { s_Stage2.GoodPeo[0] = true; s_Stage2.PrintPeoPle[0] = 4; }
					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 1) { s_Stage2.GoodPeo[1] = true; s_Stage2.PrintPeoPle[1] = 4; }
					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 2) { s_Stage2.GoodPeo[2] = true; s_Stage2.PrintPeoPle[2] = 4; }
				}
				if (s_Stage2.checkChose[2] == false && (Item9.left <= xPos) && (Item9.right >= xPos) && (Item9.top <= yPos) && (Item9.bottom >= yPos))
				{
					sndPlaySoundA("goodclick.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					s_Stage2.GoodChose += 1;

					//중복 선택 불가능하게 만듦
					s_Stage2.checkChose[2] = true;

					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 0) { s_Stage2.GoodPeo[0] = true; s_Stage2.PrintPeoPle[0] = 4; }
					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 1) { s_Stage2.GoodPeo[1] = true; s_Stage2.PrintPeoPle[1] = 4; }
					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 2) { s_Stage2.GoodPeo[2] = true; s_Stage2.PrintPeoPle[2] = 4; }
				}
				else if (!((Item8.left <= xPos) && (Item8.right >= xPos) && (Item8.top <= yPos) && (Item8.bottom >= yPos))
					&& !((Item11.left <= xPos) && (Item11.right >= xPos) && (Item11.top <= yPos) && (Item11.bottom >= yPos))
					&& !((Item9.left <= xPos) && (Item9.right >= xPos) && (Item9.top <= yPos) && (Item9.bottom >= yPos)))
				{
					sndPlaySoundA("badclick.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					s_Stage2.BadPeople += 1;
				}
			}
			//자바칩 종류일때 - 과자 우유 커피콩
			if (s_Stage2.ItemClick[2] == true)
			{
				if (s_Stage2.checkChose[0] == false && (Item7.left <= xPos) && (Item7.right >= xPos) && (Item7.top <= yPos) && (Item7.bottom >= yPos))
				{
					sndPlaySoundA("goodclick.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					s_Stage2.GoodChose += 1;

					//중복 선택 불가능하게 만듦
					s_Stage2.checkChose[0] = true;

					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 0) { s_Stage2.GoodPeo[0] = true; s_Stage2.PrintPeoPle[0] = 5; }
					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 1) { s_Stage2.GoodPeo[1] = true; s_Stage2.PrintPeoPle[1] = 5; }
					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 2) { s_Stage2.GoodPeo[2] = true; s_Stage2.PrintPeoPle[2] = 5; }
				}
				if (s_Stage2.checkChose[5] == false && (Item12.left <= xPos) && (Item12.right >= xPos) && (Item12.top <= yPos) && (Item12.bottom >= yPos))
				{
					sndPlaySoundA("goodclick.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					s_Stage2.GoodChose += 1;

					//중복 선택 불가능하게 만듦
					s_Stage2.checkChose[5] = true;

					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 0) { s_Stage2.GoodPeo[0] = true; s_Stage2.PrintPeoPle[0] = 5; }
					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 1) { s_Stage2.GoodPeo[1] = true; s_Stage2.PrintPeoPle[1] = 5; }
					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 2) { s_Stage2.GoodPeo[2] = true; s_Stage2.PrintPeoPle[2] = 5; }
				}
				if (s_Stage2.checkChose[3] == false && (Item10.left <= xPos) && (Item10.right >= xPos) && (Item10.top <= yPos) && (Item10.bottom >= yPos))
				{
					sndPlaySoundA("goodclick.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					s_Stage2.GoodChose += 1;

					//중복 선택 불가능하게 만듦
					s_Stage2.checkChose[3] = true;

					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 0) { s_Stage2.GoodPeo[0] = true; s_Stage2.PrintPeoPle[0] = 5; }
					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 1) { s_Stage2.GoodPeo[1] = true; s_Stage2.PrintPeoPle[1] = 5; }
					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 2) { s_Stage2.GoodPeo[2] = true; s_Stage2.PrintPeoPle[2] = 5; }
				}
				else if (!((Item7.left <= xPos) && (Item7.right >= xPos) && (Item7.top <= yPos) && (Item7.bottom >= yPos))
					&& !((Item12.left <= xPos) && (Item12.right >= xPos) && (Item12.top <= yPos) && (Item12.bottom >= yPos))
					&& !((Item10.left <= xPos) && (Item10.right >= xPos) && (Item10.top <= yPos) && (Item10.bottom >= yPos)))
				{
					sndPlaySoundA("badclick.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					s_Stage2.BadPeople += 1;
				}
			}
			//카페라떼 종류일때 - 커피콩 우유 얼음
			if (s_Stage2.ItemClick[3] == true)//라떼 종류일때
			{
				if (s_Stage2.checkChose[3] == false && (Item10.left <= xPos) && (Item10.right >= xPos) && (Item10.top <= yPos) && (Item10.bottom >= yPos))
				{
					sndPlaySoundA("goodclick.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					s_Stage2.GoodChose += 1;

					//중복 선택 불가능하게 만듦
					s_Stage2.checkChose[3] = true;

					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 0) { s_Stage2.GoodPeo[0] = true; s_Stage2.PrintPeoPle[0] = 2; }
					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 1) { s_Stage2.GoodPeo[1] = true; s_Stage2.PrintPeoPle[1] = 2; }
					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 2) { s_Stage2.GoodPeo[2] = true; s_Stage2.PrintPeoPle[2] = 2; }
				}
				if (s_Stage2.checkChose[5] == false && (Item12.left <= xPos) && (Item12.right >= xPos) && (Item12.top <= yPos) && (Item12.bottom >= yPos))
				{
					sndPlaySoundA("goodclick.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					s_Stage2.GoodChose += 1;

					//중복 선택 불가능하게 만듦
					s_Stage2.checkChose[5] = true;

					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 0) { s_Stage2.GoodPeo[0] = true; s_Stage2.PrintPeoPle[0] = 2; }
					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 1) { s_Stage2.GoodPeo[1] = true; s_Stage2.PrintPeoPle[1] = 2; }
					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 2) { s_Stage2.GoodPeo[2] = true; s_Stage2.PrintPeoPle[2] = 2; }
				}
				if (s_Stage2.checkChose[1] == false && (Item8.left <= xPos) && (Item8.right >= xPos) && (Item8.top <= yPos) && (Item8.bottom >= yPos))
				{
					sndPlaySoundA("goodclick.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					s_Stage2.GoodChose += 1;

					//중복 선택 불가능하게 만듦
					s_Stage2.checkChose[1] = true;

					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 0) { s_Stage2.GoodPeo[0] = true; s_Stage2.PrintPeoPle[0] = 2; }
					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 1) { s_Stage2.GoodPeo[1] = true; s_Stage2.PrintPeoPle[1] = 2; }
					if (s_Stage2.GoodChose >= 3 && s_Stage2.GoodPeople == 2) { s_Stage2.GoodPeo[2] = true; s_Stage2.PrintPeoPle[2] = 2; }
				}
				else if (!((Item10.left <= xPos) && (Item10.right >= xPos) && (Item10.top <= yPos) && (Item10.bottom >= yPos))
					&& !((Item12.left <= xPos) && (Item12.right >= xPos) && (Item12.top <= yPos) && (Item12.bottom >= yPos))
					&& !((Item8.left <= xPos) && (Item8.right >= xPos) && (Item8.top <= yPos) && (Item8.bottom >= yPos)))
				{
					sndPlaySoundA("badclick.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					s_Stage2.BadPeople += 1;
				}
			}
		}
		else if (msg == WM_DESTROY) PostQuitMessage(0);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void CreateBuffer(HWND hWnd, HDC hDC)
{
	GdiplusStartupInput			gdiplusStartupInput;
	ULONG_PTR					gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	RECT rc;
	GetClientRect(hWnd, &rc);

	g_Bitmap = new Bitmap(rc.right - rc.left, rc.bottom - rc.top);
	g_BackBuffer = new Graphics(g_Bitmap);
	g_BackBuffer->SetPageUnit(Gdiplus::Unit::UnitPixel);

	g_MainBuffer = new Graphics(hDC);
	g_MainBuffer->SetPageUnit(Gdiplus::Unit::UnitPixel);
}

void ReleaseBuffer(HWND hWnd, HDC hDC)
{
	ReleaseDC(hWnd, hDC);

	delete g_Bitmap;
	delete g_BackBuffer;
	delete g_MainBuffer;
}

void OnUpdate(HWND hWnd, DWORD tick)
{
	//메뉴 화면 출력
	if (InGame == false)
	{
		//배경 이미지를 그리는 부분. 0좌표, 0좌표에서 시작하고 1100의 폭과 800의 넓이만큼 그린다
		g_BackImage.Draw(g_BackBuffer, 0, 0, 1100, 800);
		//UI를 그린다
		PlayBar.Draw(g_BackBuffer, 400, 200, 0, 0, 538, 279);
		ExitBar.Draw(g_BackBuffer, 400, 440, 0, 0, 538, 277);	
	}
	//스테이지 화면, 캐릭터 출력
	if (InGame == true)
	{
		//스테이지 배경 이미지
		MainImage.Draw(g_BackBuffer, 0, 0, 1100, 800);

		//주인공 이미지를 그리는 부분
		g_mychar.image.Draw(g_BackBuffer, g_mychar.g_x, g_mychar.g_y,
			g_mychar.ani[g_mychar.index].x, g_mychar.ani[g_mychar.index].y,
			g_mychar.ani[g_mychar.index].width, g_mychar.ani[g_mychar.index].height);

		if (hWnd == NULL)
			return;

		BYTE key[256]; //BYTE : unsigned char
		::GetKeyboardState(key);

		int xDir = 0;

		if (key[VK_LEFT] & 0x80) { xDir = -1; }
		if (key[VK_RIGHT] & 0x80) { xDir = 1; }
		g_mychar.Move(xDir, tick);

		g_Interval += tick;
		g_DirSound += tick;
		g_NextStage += tick;

		if (xDir == 1) //만약 오른쪽으로 간다면 (0~6)
		{
			if (g_DirSound >= 500)
			{
				sndPlaySoundA("jump.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
				g_DirSound = 0;
			}
			g_mychar.RIGHT = true; //오른쪽 애니메이션을 true로
			g_mychar.LEFT = false; //왼쪽 애니메이션을 false로

			if (g_mychar.g_x > 960)
			{
				g_mychar.g_x = 950;
			}
			if (g_Interval >= g_mychar.ani[g_mychar.index].ani_time) //누적시킨 값이 ani_time초를 넘으면
			{
				g_Interval = 0; // 0으로 초기화시켜 다음 애니메이션으로 넘어갈 수 있게 함
				g_mychar.index++; //다음 애니메이션을 불러옴
				if (g_mychar.index >= g_mychar.nFrame - 6) //만약 오른쪽 프레임을 다 돌았다면 (6)
				{
					g_mychar.index = 0; //처음 애니메이션으로 돌아감 (0)
				}
			}
		}
		if (xDir == -1) //만약 왼쪽으로 간다면 (6~12)
		{
			if (g_DirSound >= 500)
			{
				sndPlaySoundA("jump.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
				g_DirSound = 0;
			}
			g_mychar.LEFT = true; //왼쪽 애니메이션을 true로
			g_mychar.RIGHT = false; //오른쪽 애니메이션을 false로

			if (g_mychar.g_x < 0)
			{
				g_mychar.g_x = 10;
			}
			if (g_Interval >= g_mychar.ani[g_mychar.index].ani_time) //누적시킨 값이 ani_time초를 넘으면
			{
				g_Interval = 0; // 0으로 초기화시켜 다음 애니메이션으로 넘어갈 수 있게 함

				g_mychar.index--; //다음 애니메이션을 불러옴
				if (g_mychar.index <= g_mychar.nFrame - 6) //만약 왼쪽 프레임을 다 돌았다면 (6)
				{
					g_mychar.index = g_mychar.nFrame - 1; //처음 애니메이션으로 돌아감 (12)
				}
			}
		}
		//게임 시작시 아무 키보드도 누르지 않았을 때
		if (g_Interval >= g_mychar.ani[g_mychar.index].ani_time && g_mychar.RIGHT == true)
		{
			g_Interval = 0;

			g_mychar.index++;
			if (g_mychar.index >= g_mychar.nFrame - 6)
			{
				g_mychar.index = 0;
			}
		}
		if (g_Interval >= g_mychar.ani[g_mychar.index].ani_time && g_mychar.LEFT == true)
		{
			g_Interval = 0;

			g_mychar.index--;
			if (g_mychar.index <= g_mychar.nFrame - 6)
			{
				g_mychar.index = g_mychar.nFrame - 1;
			}
		}
		//스테이지 1 자리
		if (g_mychar.g_x > 10 && g_mychar.g_x < 60)
		{
			Stagelocation_1 = true;
		}
		else { Stagelocation_1 = false; }
		//스테이지 2 자리
		if (g_mychar.g_x > 880 && g_mychar.g_x < 960)
		{
			Stagelocation_2 = true;
		}
		else { Stagelocation_2 = false; }
	}
	//스테이지 1 들어간다 했으면 게임방법 창 띄운다
	if (s_Stage1.ExitClick == true)
	{
		CounterText.Draw(g_BackBuffer, 500, 50, 0, 0, 680, 680);
	}
	//스테이지 2 들어간다 했으면 게임방법 창 띄운다
	if (s_Stage2.ExitClick == true)
	{
		CafeText.Draw(g_BackBuffer, 50, 120, 0, 0, 873, 424);
	}
	//스테이지 1 시작
	if (s_Stage1.p_Stage == true)
	{
		CounterImage.Draw(g_BackBuffer, 0, 0, 1100, 800);//스테이지 1 이미지 그리기

		mciOpen.lpstrElementName = "game.mp3"; //스테이지 1 배경음악
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)& mciOpen);
		dwID = mciOpen.wDeviceID;
		mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)& m_mciPlayParms);

		srand((unsigned)time(NULL)); //랜덤 값을 위함

		//만족, 불만족 손님 수 윈도우에 출력
		HDC hdc;
		hdc = GetDC(hWnd);
		char buff[128];
		wsprintf(buff, "만족시킨 손님 수 >> %d        불만족시킨 손님 수 >> %d", s_Stage1.GoodPeople, s_Stage1.BadPeople);
		SetWindowText(hWnd, buff);

		//물건 그리기
		if (s_Stage1.OutItem == false)
		{
			//지난 선택 때 true가 됐던 bool형을 false로 초기화
			s_Stage1.notGood = false;
			s_Stage1.notBad = false;

			for (int i = 0; i < 10; i++)
			{
				int k = rand() % 6; //0~5중에서 랜덤으로 k값 생성
				num = k;
				s_Stage1.OutItem = true; //안 돌린다
				//num에 따라 손님, 아이템 좌표를 지정하고 해당되는 아이템을 true로 바꾼다
				if (num == 0) { X = 317; Y = 319; W = 160; H = 268; i_x = 0; i_y = 0; i_w = 164; i_h = 144; s_Stage1.ItemClick[0] = true; }//히퍼 + 빵
				else s_Stage1.ItemClick[0] = false;
				if (num == 1) { X = 0; Y = 314; W = 165; H = 262; i_x = 0; i_y = 0; i_w = 251; i_h = 190; s_Stage1.ItemClick[1] = true; }//꽃 + 햄버거
				else s_Stage1.ItemClick[1] = false;
				if (num == 2) { X = 161; Y = 617; W = 149; H = 271; i_x = 0; i_y = 0; i_w = 183; i_h = 146; s_Stage1.ItemClick[2] = true; }//오징어 + 주스
				else s_Stage1.ItemClick[2] = false;
				if (num == 3) { X = 1752; Y = 888; W = 139; H = 216; i_x = 0; i_y = 0; i_w = 155; i_h = 137; s_Stage1.ItemClick[3] = true; }//빨간 여자 + 고기
				else s_Stage1.ItemClick[3] = false;
				if (num == 4) { X = 1550; Y = 349; W = 132; H = 221; i_x = 0; i_y = 0; i_w = 113; i_h = 172; s_Stage1.ItemClick[4] = true; }//구름탄 거북이 + 우유
				else s_Stage1.ItemClick[4] = false;
				if (num == 5) { X = 308; Y = 714; W = 150; H = 173; i_x = 0; i_y = 0; i_w = 141; i_h = 254; s_Stage1.ItemClick[5] = true; }//물고기 + 과자
				else s_Stage1.ItemClick[5] = false;
			}
		}
		if (s_Stage1.OutItem == true)
		{
			//아이템 그리기
			g_item[num].Item.Draw(g_BackBuffer, g_item[num].x, g_item[num].y, i_x, i_y, i_w, i_h); //k번의 아이템 출력
			//손님 그리기
			g_People.people.Draw(g_BackBuffer, g_People.p_x, g_People.p_y, X, Y, W, H); //손님 출력

			if (s_Stage1.stage_1 == true)
			{
				g_item[num].x += 30;
				g_People.p_x += 30;
			}
			if (s_Stage1.stage_2 == true)
			{
				g_item[num].x += 70;
				g_People.p_x += 70;
			}
			if (s_Stage1.stage_3 == true)
			{
				g_item[num].x += 110;
				g_People.p_x += 110;
			}
			if (g_item[num].x > 1100) //화면 밖으로 가면
			{
				g_item[num].x = 0; //x를 0으로 초기화 시키고
				g_People.p_x = 0;
				s_Stage1.OutItem = false; //다시 돌린다
				//한번 돌고나면 다시 선택 가능
				s_Stage1.checkChose[0] = false;
				s_Stage1.checkChose[1] = false;
				s_Stage1.checkChose[2] = false;
				s_Stage1.checkChose[3] = false;
				s_Stage1.checkChose[4] = false;
				s_Stage1.checkChose[5] = false;

				//맞추거나 틀린 것 없이 화면을 나가면
				if (s_Stage1.notGood == false && s_Stage1.notBad == false)
				{
					s_Stage1.BadPeople += 1;
				}
			}
		}
		//첫번째 손님 이미지
		if (s_Stage1.GoodPeo[0] == true)
		{
			if (s_Stage1.PrintPeoPle[0] == 1) { Fish.people.Draw(g_BackBuffer, 40, 662, 0, 0, 100, 170); }//물고기
			if (s_Stage1.PrintPeoPle[0] == 2) { Woman.people.Draw(g_BackBuffer, 40, 660, 0, 0, 100, 170); }//여자
			if (s_Stage1.PrintPeoPle[0] == 3) { Turtle.people.Draw(g_BackBuffer, 40, 640, 0, 0, 100, 170); }//거북이
			if (s_Stage1.PrintPeoPle[0] == 4) { Flower.people.Draw(g_BackBuffer, 40, 650, 0, 0, 100, 170); }//꽃
			if (s_Stage1.PrintPeoPle[0] == 5) { Squid.people.Draw(g_BackBuffer, 40, 640, 0, 0, 100, 170); }//오징어
			if (s_Stage1.PrintPeoPle[0] == 6) { Hiper.people.Draw(g_BackBuffer, 40, 640, 0, 0, 100, 170); }//히퍼
		}
		//두번째 손님 이미지
		if (s_Stage1.GoodPeo[1] == true)
		{
			if (s_Stage1.PrintPeoPle[1] == 1) { Fish.people.Draw(g_BackBuffer, 120, 662, 0, 0, 100, 170); }//물고기
			if (s_Stage1.PrintPeoPle[1] == 2) { Woman.people.Draw(g_BackBuffer, 120, 660, 0, 0, 100, 170); }//여자
			if (s_Stage1.PrintPeoPle[1] == 3) { Turtle.people.Draw(g_BackBuffer, 120, 640, 0, 0, 100, 170); }//거북이
			if (s_Stage1.PrintPeoPle[1] == 4) { Flower.people.Draw(g_BackBuffer, 120, 650, 0, 0, 100, 170); }//꽃
			if (s_Stage1.PrintPeoPle[1] == 5) { Squid.people.Draw(g_BackBuffer, 120, 640, 0, 0, 100, 170); }//오징어
			if (s_Stage1.PrintPeoPle[1] == 6) { Hiper.people.Draw(g_BackBuffer, 120, 640, 0, 0, 100, 170); }//히퍼
		}
		//세번째 손님 이미지
		if (s_Stage1.GoodPeo[2] == true)
		{
			if (s_Stage1.PrintPeoPle[2] == 1) { Fish.people.Draw(g_BackBuffer, 200, 662, 0, 0, 100, 170); }//물고기
			if (s_Stage1.PrintPeoPle[2] == 2) { Woman.people.Draw(g_BackBuffer, 200, 660, 0, 0, 100, 170); }//여자
			if (s_Stage1.PrintPeoPle[2] == 3) { Turtle.people.Draw(g_BackBuffer, 200, 640, 0, 0, 100, 170); }//거북이
			if (s_Stage1.PrintPeoPle[2] == 4) { Flower.people.Draw(g_BackBuffer, 200, 650, 0, 0, 100, 170); }//꽃
			if (s_Stage1.PrintPeoPle[2] == 5) { Squid.people.Draw(g_BackBuffer, 200, 640, 0, 0, 100, 170); }//오징어
			if (s_Stage1.PrintPeoPle[2] == 6) { Hiper.people.Draw(g_BackBuffer, 200, 640, 0, 0, 100, 170); }//히퍼
		}
		//네번째 손님 이미지
		if (s_Stage1.GoodPeo[3] == true)
		{
			if (s_Stage1.PrintPeoPle[3] == 1) { Fish.people.Draw(g_BackBuffer, 280, 662, 0, 0, 100, 170); }//물고기
			if (s_Stage1.PrintPeoPle[3] == 2) { Woman.people.Draw(g_BackBuffer, 280, 660, 0, 0, 100, 170); }//여자
			if (s_Stage1.PrintPeoPle[3] == 3) { Turtle.people.Draw(g_BackBuffer, 280, 640, 0, 0, 100, 170); }//거북이
			if (s_Stage1.PrintPeoPle[3] == 4) { Flower.people.Draw(g_BackBuffer, 280, 650, 0, 0, 100, 170); }//꽃
			if (s_Stage1.PrintPeoPle[3] == 5) { Squid.people.Draw(g_BackBuffer, 280, 640, 0, 0, 100, 170); }//오징어
			if (s_Stage1.PrintPeoPle[3] == 6) { Hiper.people.Draw(g_BackBuffer, 280, 640, 0, 0, 100, 170); }//히퍼
		}
		//다섯번째 손님 이미지
		if (s_Stage1.GoodPeo[4] == true)
		{
			if (s_Stage1.PrintPeoPle[4] == 1) { Fish.people.Draw(g_BackBuffer, 360, 662, 0, 0, 100, 170); }//물고기
			if (s_Stage1.PrintPeoPle[4] == 2) { Woman.people.Draw(g_BackBuffer, 360, 660, 0, 0, 100, 170); }//여자
			if (s_Stage1.PrintPeoPle[4] == 3) { Turtle.people.Draw(g_BackBuffer, 360, 640, 0, 0, 100, 170); }//거북이
			if (s_Stage1.PrintPeoPle[4] == 4) { Flower.people.Draw(g_BackBuffer, 360, 650, 0, 0, 100, 170); }//꽃
			if (s_Stage1.PrintPeoPle[4] == 5) { Squid.people.Draw(g_BackBuffer, 360, 640, 0, 0, 100, 170); }//오징어
			if (s_Stage1.PrintPeoPle[4] == 6) { Hiper.people.Draw(g_BackBuffer, 360, 640, 0, 0, 100, 170); }//히퍼		
		}
		//만약 3명 맞췄다면 스테이지 2로 들어가기
		if (s_Stage1.GoodPeo[2] == true && s_Stage1.LastGoodPeo == false && s_Stage1.fullGoodPeo[0] == false)
		{
			g_NextStage = 0; //시간을 0으로 만들고
			s_Stage1.fullGoodPeo[0] = true; //fullGoodPeo을 true로 바꿔라
		}
		if (s_Stage1.GoodPeople >= 3 && s_Stage1.stage_1 == true)
		{
			if (g_NextStage >= 100)
			{
				//스테이지 2로 바꾸기
				s_Stage1.stage_2 = true;
				s_Stage1.stage_1 = false;
				//이미지 삭제하기
				s_Stage1.GoodPeo[0] = false;
				s_Stage1.GoodPeo[1] = false;
				s_Stage1.GoodPeo[2] = false;
				//0으로 초기화
				s_Stage1.GoodPeople = 0;
				MessageBox(hWnd, "▶ 스테이지 1 클리어 ◀\n※ 손님의 이동속도가 증가합니다 ※", "카운터 알바 게임", MB_ICONWARNING | MB_OK);
			}
		}
		//만약 4명 맞췄다면 스테이지 3으로 들어가기
		if (s_Stage1.GoodPeo[3] == true && s_Stage1.LastGoodPeo == false && s_Stage1.fullGoodPeo[1] == false)
		{
			g_NextStage = 0;
			s_Stage1.fullGoodPeo[1] = true;
		}
		if (s_Stage1.GoodPeople >= 4 && s_Stage1.stage_2 == true)
		{
			if (g_NextStage >= 100)
			{
				s_Stage1.stage_3 = true;
				s_Stage1.stage_2 = false;
				s_Stage1.GoodPeo[0] = false;
				s_Stage1.GoodPeo[1] = false;
				s_Stage1.GoodPeo[2] = false;
				s_Stage1.GoodPeo[3] = false;
				s_Stage1.GoodPeople = 0;
				MessageBox(hWnd, "▶ 스테이지 2 클리어 ◀\n※ 손님의 이동속도가 더욱 증가합니다 ※", "카운터 알바 게임", MB_ICONWARNING | MB_OK);
			}
		}
		//5번째가 출력되야하고, LastGoodPeo가 false이면서 fullGoodPeo이 false이면
		if (s_Stage1.GoodPeo[4] == true && s_Stage1.LastGoodPeo == false && s_Stage1.fullGoodPeo[2] == false)
		{
			g_NextStage = 0;
			s_Stage1.fullGoodPeo[2] = true;
		}
		//5명 맞췄고 스테이지 3이면
		if (s_Stage1.GoodPeople >= 5 && s_Stage1.stage_3 == true)
		{
			//해당 초가 지나면 게임을 끝내라 (이미지 출력 전에 끝나는 것을 방지하기 위해 만듦)
			if (g_NextStage >= 100)
			{
				mciSendCommandW(dwID, MCI_CLOSE, 0, NULL);

				sndPlaySoundA("clear.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);

				MessageBox(hWnd, "▶ 성공 ◀\n※ 오늘도 무사히 편의점 알바를 마쳤습니다 ※", "카운터 알바 게임", MB_ICONASTERISK | MB_OK);

				s_Stage1.p_Stage = false;
				s_Stage1.Clear_Stage = true;

				//사운드
				mciOpen.lpstrElementName = "main.mp3"; //메인
				mciOpen.lpstrDeviceType = "mpegvideo";
				mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)& mciOpen);
				dwID = mciOpen.wDeviceID;
				mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)& m_mciPlayParms);
			}
		}
		//3번 틀렸다
		if (s_Stage1.BadPeople >= 3)
		{
			if (g_NextStage >= 100)
			{
				mciSendCommandW(dwID, MCI_CLOSE, 0, NULL);

				sndPlaySoundA("failure.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);

				MessageBox(hWnd, "▶ 실패 ◀\n※ 사장님이 당신을 해고했습니다 ※\n※ 강토끼씨는 백수가 되었습니다 ※", "카운터 알바 게임", MB_ICONWARNING | MB_OK);
				exit(0);
			}
		}
	}	
	//스테이지 2 시작
	if (s_Stage2.p_Stage == true)
	{
		Cafe.Draw(g_BackBuffer, 0, 0, 1100, 800); //스테이지 2 이미지 그리기

		mciOpen.lpstrElementName = "game2.mp3"; //스테이지 2 배경음악
		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)& mciOpen);
		dwID = mciOpen.wDeviceID;
		mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)& m_mciPlayParms);

		srand((unsigned)time(NULL)); //랜덤 값을 위함

		//만족, 불만족 손님 수 윈도우에 출력
		HDC hdc;
		hdc = GetDC(hWnd);
		char buff[128];
		wsprintf(buff, "만족시킨 손님 수 >> %d        불만족시킨 손님 수 >> %d        남은 재료 선택 >> %d / 3", s_Stage2.GoodPeople, s_Stage2.BadPeople, s_Stage2.GoodChose);
		SetWindowText(hWnd, buff);

		//물건 그리기
		if (s_Stage2.OutItem == false)
		{
			for (int i = 0; i < 10; i++)
			{
				int k = rand() % 4 + 6; //6~9중에서 랜덤으로 k값 생성
				num = k;
				s_Stage2.OutItem = true; //안 돌린다
				//num에 따라 손님, 아이템 좌표를 지정하고 해당되는 커피와 재료를 true로 바꾼다
				if (num == 6) { X = 317; Y = 319; W = 160; H = 268; i_x = 0; i_y = 0; i_w = 225; i_h = 225;	s_Stage2.ItemClick[0] = true; }//히퍼 + 아메
				else s_Stage2.ItemClick[0] = false;
				if (num == 7) { X = 0; Y = 314; W = 165; H = 262; i_x = 0; i_y = 0; i_w = 119; i_h = 210; s_Stage2.ItemClick[1] = true; }//꽃 + 에이드
				else s_Stage2.ItemClick[1] = false; 
				if (num == 8) { X = 161; Y = 617; W = 149; H = 271; i_x = 0; i_y = 0; i_w = 118; i_h = 160;	s_Stage2.ItemClick[2] = true; }//오징어 + 자바칩
				else s_Stage2.ItemClick[2] = false;
				if (num == 9) { X = 1752; Y = 888; W = 139; H = 216; i_x = 0; i_y = 0; i_w = 113; i_h = 139;  s_Stage2.ItemClick[3] = true;	}//빨간 여자 + 라떼
				else s_Stage2.ItemClick[3] = false;
			}
		}
		if (s_Stage2.OutItem == true)
		{
			g_item[num].y = 300;
			g_People.p_y = 480;

			g_item[num].x += 30;
			g_People.p_x += 30;

			//아이템 그리기
			g_item[num].Item.Draw(g_BackBuffer, g_item[num].x, g_item[num].y, i_x, i_y, i_w, i_h); //num번의 커피 출력
			//손님 그리기
			g_People.people.Draw(g_BackBuffer, g_People.p_x, g_People.p_y, X, Y, W, H); //손님 출력

			if (g_item[num].x >= 350 && g_item[num].x <= 450) //해당 지점이면
			{
				g_item[num].x = 400; //해당 지점에서 멈추기
				g_People.p_x = 400;
			}
			if (s_Stage2.BadPeople == 1 && s_Stage2.notGood == false)//재료를 틀리게 선택했으면
			{		
				g_item[num].x = 0;
				g_People.p_x = 0;
				s_Stage2.GoodChose = 0;
				s_Stage2.OutItem = false; //다시 돌린다
				s_Stage2.notGood = true;
				//다시 선택 가능
				s_Stage2.checkChose[0] = false;
				s_Stage2.checkChose[1] = false;
				s_Stage2.checkChose[2] = false;
				s_Stage2.checkChose[3] = false;
				s_Stage2.checkChose[4] = false;
				s_Stage2.checkChose[5] = false;
				s_Stage2.checkChose[6] = false;
			}
			if (s_Stage2.BadPeople == 2 && s_Stage2.notBad == false)//재료를 틀리게 선택했으면
			{
				g_item[num].x = 0;
				g_People.p_x = 0;
				s_Stage2.GoodChose = 0;
				s_Stage2.OutItem = false; //다시 돌린다
				s_Stage2.notBad = true;
				//다시 선택 가능
				s_Stage2.checkChose[0] = false;
				s_Stage2.checkChose[1] = false;
				s_Stage2.checkChose[2] = false;
				s_Stage2.checkChose[3] = false;
				s_Stage2.checkChose[4] = false;
				s_Stage2.checkChose[5] = false;
				s_Stage2.checkChose[6] = false;
			}
			if (s_Stage2.GoodChose >= 3)//모든 재료를 옳게 선택했으면
			{
				s_Stage2.GoodChose = 0;
				s_Stage2.GoodPeople += 1;
				//다시 선택 가능
				s_Stage2.checkChose[0] = false;
				s_Stage2.checkChose[1] = false;
				s_Stage2.checkChose[2] = false;
				s_Stage2.checkChose[3] = false;
				s_Stage2.checkChose[4] = false;
				s_Stage2.checkChose[5] = false;
				s_Stage2.checkChose[6] = false;

				g_item[num].x = 0;
				g_People.p_x = 0;

				s_Stage2.OutItem = false; //다시 돌린다
			}			
		}
		if (s_Stage2.GoodPeo[0] == true)
		{		
			if (s_Stage2.PrintPeoPle[0] == 2) { Woman.people.Draw(g_BackBuffer, 340, 30, 0, 0, 100, 170); }//여자			
			if (s_Stage2.PrintPeoPle[0] == 4) { Flower.people.Draw(g_BackBuffer, 340, 30, 0, 0, 100, 170); }//꽃
			if (s_Stage2.PrintPeoPle[0] == 5) { Squid.people.Draw(g_BackBuffer, 340, 10, 0, 0, 100, 170); }//오징어
			if (s_Stage2.PrintPeoPle[0] == 6) { Hiper.people.Draw(g_BackBuffer, 340, 10, 0, 0, 100, 170); }//히퍼
		}
		//두번째 손님 이미지
		if (s_Stage2.GoodPeo[1] == true)
		{
			if (s_Stage2.PrintPeoPle[1] == 2) { Woman.people.Draw(g_BackBuffer, 530, 30, 0, 0, 100, 170); }//여자
			if (s_Stage2.PrintPeoPle[1] == 4) { Flower.people.Draw(g_BackBuffer, 530, 30, 0, 0, 100, 170); }//꽃
			if (s_Stage2.PrintPeoPle[1] == 5) { Squid.people.Draw(g_BackBuffer, 530, 10, 0, 0, 100, 170); }//오징어
			if (s_Stage2.PrintPeoPle[1] == 6) { Hiper.people.Draw(g_BackBuffer, 530, 10, 0, 0, 100, 170); }//히퍼
		}
		//세번째 손님 이미지
		if (s_Stage2.GoodPeo[2] == true)
		{
			if (s_Stage2.PrintPeoPle[2] == 2) { Woman.people.Draw(g_BackBuffer, 720, 30, 0, 0, 100, 170); }//여자
			if (s_Stage2.PrintPeoPle[2] == 4) { Flower.people.Draw(g_BackBuffer, 720, 30, 0, 0, 100, 170); }//꽃
			if (s_Stage2.PrintPeoPle[2] == 5) { Squid.people.Draw(g_BackBuffer, 720, 10, 0, 0, 100, 170); }//오징어
			if (s_Stage2.PrintPeoPle[2] == 6) { Hiper.people.Draw(g_BackBuffer, 720, 10, 0, 0, 100, 170); }//히퍼
		}
		//3번째가 출력되야하고, LastGoodPeo가 false이면서 fullGoodPeo이 false이면
		if (s_Stage2.GoodPeo[2] == true && s_Stage2.LastGoodPeo == false && s_Stage2.fullGoodPeo[0] == false)
		{
			g_NextStage = 0;
			s_Stage2.fullGoodPeo[0] = true;
		}
		//3명 맞췄으면
		if (s_Stage2.GoodPeo[2] == true && s_Stage2.GoodPeople >= 3)
		{
			//해당초가 지나면 게임을 끝내라 (이미지 출력 전에 끝나는 것을 방지하기 위해 만듦)
			if (g_NextStage >= 100)
			{
				mciSendCommandW(dwID, MCI_CLOSE, 0, NULL);

				sndPlaySoundA("clear.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);

				MessageBox(hWnd, "▶ 성공 ◀\n※ 오늘도 무사히 카페 알바를 마쳤습니다 ※", "카운터 알바 게임", MB_ICONASTERISK | MB_OK);

				s_Stage2.p_Stage = false;
				s_Stage2.Clear_Stage = true;

				//사운드
				mciOpen.lpstrElementName = "main.mp3"; //메인
				mciOpen.lpstrDeviceType = "mpegvideo";
				mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)& mciOpen);
				dwID = mciOpen.wDeviceID;
				mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)& m_mciPlayParms);
			}
		}
		//3번 틀렸다
		if (s_Stage2.BadPeople >= 3)
		{
			if (g_NextStage >= 100)
			{
				mciSendCommandW(dwID, MCI_CLOSE, 0, NULL);
				sndPlaySoundA("failure.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);

				MessageBox(hWnd, "▶ 실패 ◀\n※ 사장님이 당신을 해고했습니다 ※\n※ 강토끼씨는 백수가 되었습니다 ※", "카운터 알바 게임", MB_ICONWARNING | MB_OK);
				exit(0);
			}
		}
	}
	//스테이지를 모두 클리어했으면
	if (s_Stage1.Clear_Stage == true && s_Stage2.Clear_Stage == true)
	{
		mciSendCommandW(dwID, MCI_CLOSE, 0, NULL);

		MessageBox(hWnd, "▶ 플레이 해주셔서 감사합니다 ◀\n강토끼씨는 모든 알바를 무사히 마치고 등록금을 마련하였습니다", "카운터 알바 게임", MB_ICONWARNING | MB_OK);
		exit(0);
	}
}