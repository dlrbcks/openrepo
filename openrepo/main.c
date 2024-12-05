#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>
#include <time.h>

#define DINO_BOTTOM_Y 17     // ������ Y ��ġ
#define CACTUS_BOTTOM_Y 25   // ������ X ��ġ
#define CACTUS_BOTTOM_X 45   // ������ Y ��ġ

void ClearDino(int dino_Y);     // ����׸� �� ����
void ClearCactus(int cactus_X); // ������(��ֹ�) �׷� �� ����

void CursorView(char show);
void SetColor(unsigned short text);
void SetConsoleView();
void DrawDino(int dino_Y);
void GotoXY(int x, int y);
void DrawCactus(int cactus_X);
int IsCollision(const int cactus_X, const int dino_Y);
int GetKeyDown();
void DrawGameOver(const int score);

// ����
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
		// ���� ���۽� �ʱ�ȭ
		int isJumping = false;
		int isBottom = true;
		const int gravity = 3;

		int dino_Y = DINO_BOTTOM_Y;
		int cactus_X = CACTUS_BOTTOM_X;

		int score = 0;
		clock_t start, curr;    // ���� ���� �ʱ�ȭ
		start = clock();        // ���۽ð� �ʱ�ȭ

		while (true) { // ���� �� �ǿ� ���� ����
			// �浹üũ Ʈ���� x���� ������ y������ �Ǵ�
			if (IsCollision(cactus_X, dino_Y)) break;

			// SpaceŰ�� ���Ȱ�, �ٴ��� �ƴҶ� ����
			// Space Key ASCII : 32
			if (GetKeyDown() == ' ' && isBottom) {
				isJumping = true;
				isBottom = false;
			}

			// �������̶�� Y�� ����, ������ �������� Y�� ����.
			if (isJumping) dino_Y -= gravity;
			else dino_Y += gravity;

			// Y�� ����ؼ� �����ϴ°� �������� �ٴ��� ����.
			if (dino_Y >= DINO_BOTTOM_Y) {
				dino_Y = DINO_BOTTOM_Y;
				isBottom = true;
			}

			// ������ ������ ������ ������ ���� ��Ȳ.
			if (dino_Y <= 3) isJumping = false;

			// ������(��ֹ�)�� �������� (x����) �������ϰ�
			// ������(��ֹ�)�� ��ġ�� ���� �����ΰ��� �ٽ� ������ ������ ��ȯ.
			cactus_X -= 2;
			if (cactus_X <= 0) cactus_X = CACTUS_BOTTOM_X;

			DrawDino(dino_Y);         // ���� �׸���
			DrawCactus(cactus_X);     // ������ �׸���

			curr = clock();         // ����ð� �޾ƿ���
			if (((curr - start) / CLOCKS_PER_SEC) >= 1) {   // 1�ʰ� �Ѿ��� ���...
				score++;            // ���ھ� UP
				start = clock();    // ���۽ð� �ʱ�ȭ
			}

			Sleep(60);               // Game Speed ����
			// system("cls")         // clear
			ClearDino(dino_Y);        // ���� �����
			ClearCactus(cactus_X);    // ������(��ֹ�) �����

			SetColor(WHITE);
			// ��������� 1�ʸ��� ���ִ� ���� �ƴ϶� �׻� ��� ���ָ鼭, 1�ʰ� �������� ++ ���ݴϴ�.
			// Ŀ���� ��� �������� �ű��. �ܼ�â�� cols=100 �̴ϱ� 2*x�̹Ƿ� 22���� �־���
			GotoXY(22, 0); printf("Score : %d ", score);     // ���� �������.
			GotoXY(20, 2); printf("���� : Space Key");
		}
		// ���� ���� �޴�
		DrawGameOver(score);
	}
}

// �ܼ� �ؽ�Ʈ ���� �������ִ� �Լ�
void SetColor(unsigned short text) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text);
}

//Ŀ���� ��ġ�� x, y�� �̵��ϴ� �Լ�
void GotoXY(int x, int y) {
	COORD Pos;
	Pos.X = 2 * x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

// Ű������ �Է��� �ް�, �Էµ� Ű�� ���� ��ȯ�ϴ� �Լ�
int GetKeyDown() {
	if (_kbhit() != 0) return _getch();
	return false;
}

// �浹������ true, �ƴϸ� false
int IsCollision(const int cactus_X, const int dino_Y) {
	// Ʈ���� X�� ������ ��ü�ʿ� ������,
	// ������ ���̰� ������� �ʴٸ� �浹�� ó��
	GotoXY(0, 0);
	printf("cactusX : %2d, dinoY : %2d", cactus_X, dino_Y); // �̷������� ������ X, Y�� ã���ϴ�.
	if (cactus_X <= 8 && cactus_X >= 2 && dino_Y > 12) return true;
	return false;
}

//�������� �׸��� �Լ�
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
	SetColor(RED);  printf("������");
	SetColor(SKYBLUE);
	printf("$$$    \n");						// 3, 4
	printf("  $$$");								// 2, 3
	SetColor(BLUE);  printf("�ļҰ�");
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

// Ŀ�� On / Off
void CursorView(char show) {
	CONSOLE_CURSOR_INFO ConsoleCursor;
	ConsoleCursor.bVisible = show;   // Ŀ���� ������ ���� ����(0:�Ⱥ���, 0 �̿�:����)
	ConsoleCursor.dwSize = 1;        // Ŀ���� ũ�⸦ ����(1~100 ���̸� ����)
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ConsoleCursor);
}

// �ܼ� â�� ũ��� ������ �����ϴ� �Լ�
void SetConsoleView() {
	// Consoleâ�� ���� ũ��� ���� ũ�⸦ ����
	system("mode con:cols=120 lines=30");
	// Consoleâ�� ����â ǥ��
	system("title â�ǽǹ�������Ʈ ���� ���� ���� [By. YongWun Kim]");
	// Ŀ�� �Ⱥ��̰�...
	CursorView(false);
}

// �浹 ������ ���ӿ��� �׷���
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


// Clear �Լ��� ������ ��ü ȭ�� ����⸦ �ּ�ȭ�ϵ��� �����մϴ�.

void ClearDino(int dinoY) {
	// ������ ��ġ�� ���� ���쵵�� ����
	GotoXY(0, dinoY);
	for (int i = 0; i < 12; i++) printf("                \n");
}

void ClearCactus(int cactusX) {
	// ��ֹ��� ���쵵�� ����
	for (int i = 0; i < 5; i++) {
		GotoXY(cactusX, CACTUS_BOTTOM_Y + i);
		printf("     "); // ��ֹ� �����
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
