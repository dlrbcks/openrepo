#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>
#include <time.h>

#define DINO_BOTTOM_Y 17     // 공룡의 Y 위치
#define CACTUS_BOTTOM_Y 25   // 선인장 X 위치
#define CACTUS_BOTTOM_X 45   // 선인장 Y 위치

void ClearDino(int dino_Y);     // 공룡그린 것 지움
void ClearCactus(int cactus_X); // 선인장(장애물) 그런 것 지움

void CursorView(char show);
void SetColor(unsigned short text);
void SetConsoleView();
void DrawDino(int dino_Y);
void GotoXY(int x, int y);
void DrawCactus(int cactus_X);
int IsCollision(const int cactus_X, const int dino_Y);
int GetKeyDown();
void DrawGameOver(const int score);

// 색상
enum {
	BLACK,        // 0
	DARK_BLUE,    // 1
	DARK_GREEN,   // 2
	DARK_SKYBLUE, // 3
	DARK_RED,     // 4
	DARK_VOILET,  // 5
	DARK_YELLOW,  // 6
	GRAY,         // 7
	DARK_GRAY,    // 8
	BLUE,         // 9
	GREEN,        // 10
	SKYBLUE,      // 11
	RED,          // 12
	VOILET,       // 13
	YELLOW,       // 14
	WHITE,        // 15
};

int main() {
	SetConsoleView();
	while (true) {
		// 게임 시작시 초기화
		int isJumping = false;
		int isBottom = true;
		const int gravity = 3;

		int dino_Y = DINO_BOTTOM_Y;
		int cactus_X = CACTUS_BOTTOM_X;

		int score = 0;
		clock_t start, curr;    // 점수 변수 초기화
		start = clock();        // 시작시간 초기화

		while (true) { // 게임 한 판에 대한 루프
			// 충돌체크 트리의 x값과 공룡의 y값으로 판단
			if (IsCollision(cactus_X, dino_Y)) break;

			// Space키가 눌렸고, 바닥이 아닐때 점프
			// Space Key ASCII : 32
			if (GetKeyDown() == ' ' && isBottom) {
				isJumping = true;
				isBottom = false;
			}

			// 점프중이라면 Y를 감소, 점프가 끝났으면 Y를 증가.
			if (isJumping) dino_Y -= gravity;
			else dino_Y += gravity;

			// Y가 계속해서 증가하는걸 막기위해 바닥을 지정.
			if (dino_Y >= DINO_BOTTOM_Y) {
				dino_Y = DINO_BOTTOM_Y;
				isBottom = true;
			}

			// 점프의 맨위를 찍으면 점프가 끝난 상황.
			if (dino_Y <= 3) isJumping = false;

			// 선인장(장애물)이 왼쪽으로 (x음수) 가도록하고
			// 선인장(장애물)의 위치가 왼쪽 끝으로가면 다시 오른쪽 끝으로 소환.
			cactus_X -= 2;
			if (cactus_X <= 0) cactus_X = CACTUS_BOTTOM_X;

			DrawDino(dino_Y);         // 공룡 그리기
			DrawCactus(cactus_X);     // 선인장 그리기

			curr = clock();         // 현재시간 받아오기
			if (((curr - start) / CLOCKS_PER_SEC) >= 1) {   // 1초가 넘었을 경우...
				score++;            // 스코어 UP
				start = clock();    // 시작시간 초기화
			}

			Sleep(60);               // Game Speed 설정
			// system("cls")         // clear
			ClearDino(dino_Y);        // 공룡 지우기
			ClearCactus(cactus_X);    // 선인장(장애물) 지우기

			SetColor(WHITE);
			// 점수출력을 1초마다 해주는 것이 아니라 항상 출력 해주면서, 1초가 지났을때 ++ 해줍니다.
			// 커서를 가운데 위쪽으로 옮긴다. 콘솔창이 cols=100 이니까 2*x이므로 22정도 넣어줌
			GotoXY(22, 0); printf("Score : %d ", score);     // 점수 출력해줌.
			GotoXY(20, 2); printf("점프 : Space Key");
		}
		// 게임 오버 메뉴
		DrawGameOver(score);
	}
}

// 콘솔 텍스트 색상 변경해주는 함수
void SetColor(unsigned short text) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text);
}

//커서의 위치를 x, y로 이동하는 함수
void GotoXY(int x, int y) {
	COORD Pos;
	Pos.X = 2 * x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

// 키보드의 입력을 받고, 입력된 키의 값을 반환하는 함수
int GetKeyDown() {
	if (_kbhit() != 0) return _getch();
	return false;
}

// 충돌했으면 true, 아니면 false
int IsCollision(const int cactus_X, const int dino_Y) {
	// 트리의 X가 공룡의 몸체쪽에 있을때,
	// 공룡의 높이가 충분하지 않다면 충돌로 처리
	GotoXY(0, 0);
	printf("cactusX : %2d, dinoY : %2d", cactus_X, dino_Y); // 이런식으로 적절한 X, Y를 찾습니다.
	if (cactus_X <= 8 && cactus_X >= 2 && dino_Y > 12) return true;
	return false;
}

//선인장을 그리는 함수
void DrawCactus(int cactus_X) {
	SetColor(GREEN);
	GotoXY(cactus_X, CACTUS_BOTTOM_Y);
	printf("  $");
	GotoXY(cactus_X, CACTUS_BOTTOM_Y + 1);
	printf("$ $  ");
	GotoXY(cactus_X, CACTUS_BOTTOM_Y + 2);
	printf("$ $ $");
	GotoXY(cactus_X, CACTUS_BOTTOM_Y + 3);
	printf("$$$$$");
	GotoXY(cactus_X, CACTUS_BOTTOM_Y + 4);
	printf("  $  ");
}

void DrawDino(int dino_Y) {
	SetColor(SKYBLUE);
	GotoXY(0, dino_Y);
	static int legFlag = true;
	printf("        $$$$$$$ \n");		// 8, 7, 1
	printf("       $$ $$$$$$\n");		// 7, 2, 1, 6
	printf("       $$$$$$$$$\n");		// 7, 9
	printf("$      $$$      \n");		// 1, 6, 3, 6
	printf("$$     $$$$$$$  \n");		// 2, 5, 7, 2
	printf("$$$   $$$$$     \n");		// 3, 3, 5, 5
	printf(" $$  $$$$$$$$$$ \n");		// 1, 2, 2, 10, 1
	printf(" $$");									// 1, 2
	SetColor(RED);  printf("원광대");
	SetColor(SKYBLUE);
	printf("$$$    \n");						// 3, 4
	printf("  $$$");								// 2, 3
	SetColor(BLUE);  printf("컴소공");
	SetColor(SKYBLUE);
	printf("$    \n");							// 1, 4
	printf("    $$$$$$$$    \n");		// 4, 8, 4
	printf("     $$$$$$     \n");		// 5, 6, 5
	if (legFlag) {
		printf("     $    $$$   \n");		// 5, 1, 4, 3, 3
		printf("     $$         ");			// 5, 2, 9
		legFlag = false;
	}
	else {
		printf("     $    $$$   \n");		// 5, 1, 4, 3, 3
		printf("          $$    ");			// 10, 2, 4
		legFlag = true;
	}
}

// 커서 On / Off
void CursorView(char show) {
	CONSOLE_CURSOR_INFO ConsoleCursor;
	ConsoleCursor.bVisible = show;   // 커서를 보일지 말지 결정(0:안보임, 0 이외:보임)
	ConsoleCursor.dwSize = 1;        // 커서의 크기를 결정(1~100 사이만 가능)
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ConsoleCursor);
}

// 콘솔 창의 크기와 제목을 지저하는 함수
void SetConsoleView() {
	// Console창의 가로 크기와 세로 크기를 지정
	system("mode con:cols=120 lines=30");
	// Console창의 제목창 표시
	system("title 창의실무프로젝트 구글 공룡 게임 [By. YongWun Kim]");
	// 커서 안보이게...
	CursorView(false);
}

// 충돌 했을때 게임오버 그려줌
void DrawGameOver(const int score) {
	system("cls");
	SetColor(YELLOW);
	GotoXY(22, 8);
	printf("========== GAME OVER ==========");
	GotoXY(22, 10);
	printf("Score: %d", score);
	printf("\nPress any key to restart...");
	system("pause");
	system("cls");
}


// Clear 함수는 기존의 전체 화면 지우기를 최소화하도록 수정합니다.

void ClearDino(int dinoY) {
	// 공룡이 위치한 곳만 지우도록 변경
	GotoXY(0, dinoY);
	for (int i = 0; i < 12; i++) printf("                \n");
}

void ClearCactus(int cactusX) {
	// 장애물만 지우도록 수정
	for (int i = 0; i < 5; i++) {
		GotoXY(cactusX, CACTUS_BOTTOM_Y + i);
		printf("     "); // 장애물 지우기
	}
}


void InitializeGame(int* dino_Y, int* cactus_X, int* score) {
	*dino_Y = DINO_BOTTOM_Y;
	*cactus_X = CACTUS_BOTTOM_X;
	*score = 0;
}

void UpdateScore(clock_t* start, clock_t* curr, int* score) {
	*curr = clock();
	if (((*curr - *start) / CLOCKS_PER_SEC) >= 1) {
		(*score)++;
		*start = clock();
	}
}
