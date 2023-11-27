#include "KhuGleWin.h"
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <fstream>

#pragma warning(disable:4996)

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

std::string difficulty;
std::time_t start, finish;
std::ifstream fin;
std::ofstream fout;
bool time_ = true;
std::string stt;

class CGameLayout : public CKhuGleWin
{
public:
	CKhuGleLayer* m_pGameLayer;

	std::vector<std::vector<CKhuGleSprite*>> m_pRects;
	std::vector<std::vector<bool>> m_Mines;
	std::vector<std::vector<bool>> m_Opens;
	std::vector<std::vector<int>> m_Nums;
	std::vector<std::vector<bool>> m_Flags;

	CGameLayout(int nW, int nH);
	void Update();
	void Open(int row, int col);

	CKgPoint m_LButtonStart, m_LButtonEnd;
	int m_nLButtonStatus;
	int size;
	int mine;
	bool trap;
	bool win;
	int endrow, endcol;
};

CGameLayout::CGameLayout(int nW, int nH) : CKhuGleWin(nW, nH)
{
	start = time(NULL);

	m_nLButtonStatus = 0;

	m_pScene = new CKhuGleScene(nW, nH, KG_COLOR_24_RGB(255, 255, 255));

	m_pGameLayer = new CKhuGleLayer(nW - 50, nH - 50, KG_COLOR_24_RGB(200, 200, 255), CKgPoint(25, 25));

	m_pScene->AddChild(m_pGameLayer);

	if (difficulty == "BackYard") size = 10;
	else if (difficulty == "WarZone") size = 15;
	else size = 30;

	mine = 0;

	trap = false;
	win = false;

	endrow = endcol = 0;

	for (int row = 0; row < size; row++) {
		std::vector<CKhuGleSprite*> m_pRect;
		for (int col = 0; col < size; col++) {
			m_pRect.push_back(new CKhuGleSprite(GP_STYPE_RECT, GP_CTYPE_KINEMATIC, CKgLine(CKgPoint(25 * row, 25 * col),
				CKgPoint(25 * (row + 1) - 1, 25 * (col + 1) - 1)), KG_COLOR_24_RGB(255 * row / size, 100, 255 * col / size), false, 30));

			m_pGameLayer->AddChild(m_pRect[col]);
		}
		m_pRects.push_back(m_pRect);
	}

	srand((unsigned int)time(NULL));

	int mine_limit = size* size / 5;

	for (int row = 0; row < size; row++) {
		std::vector<bool> m_Mine;
		for (int col = 0; col < size; col++) {
			if (mine_limit > mine) {
				if (rand() % 10 < 2) {
					m_Mine.push_back(true);
					mine++;
				}
				else m_Mine.push_back(false);
			}
			else m_Mine.push_back(false);
		}
		m_Mines.push_back(m_Mine);
	}

	for (int row = 0; row < size; row++) {
		std::vector<int> m_Num;
		for (int col = 0; col < size; col++) {
			if (m_Mines[row][col]) m_Num.push_back(-1);
			else {
				int num = 0;

				//row == 0
				if (!row) {
					if (m_Mines[row + 1][col]) num++;
					if (!col) {
						if (m_Mines[row][col + 1]) num++;
						if (m_Mines[row + 1][col + 1]) num++;
					}
					else if (col == size - 1) {
						if (m_Mines[row][col - 1]) num++;
						if (m_Mines[row + 1][col - 1]) num++;
					}
					else {
						if (m_Mines[row][col + 1]) num++;
						if (m_Mines[row][col - 1]) num++;
						if (m_Mines[row + 1][col + 1]) num++;
						if (m_Mines[row + 1][col - 1]) num++;
					}
				}
				else if (row == size - 1) {
					if (m_Mines[row - 1][col]) num++;
					if (!col) {
						if (m_Mines[row][col + 1]) num++;
						if (m_Mines[row - 1][col + 1]) num++;
					}
					else if (col == size - 1) {
						if (m_Mines[row][col - 1]) num++;
						if (m_Mines[row - 1][col - 1]) num++;
					}
					else {
						if (m_Mines[row][col + 1]) num++;
						if (m_Mines[row][col - 1]) num++;
						if (m_Mines[row - 1][col + 1]) num++;
						if (m_Mines[row - 1][col - 1]) num++;
					}
				}
				else {
					if (m_Mines[row + 1][col]) num++;
					if (m_Mines[row - 1][col]) num++;
					if (!col) {
						if (m_Mines[row - 1][col + 1]) num++;
						if (m_Mines[row][col + 1]) num++;
						if (m_Mines[row + 1][col + 1]) num++;
					}
					else if (col == size - 1) {
						if (m_Mines[row - 1][col - 1]) num++;
						if (m_Mines[row][col - 1]) num++;
						if (m_Mines[row + 1][col - 1]) num++;
					}
					else {
						if (m_Mines[row - 1][col + 1]) num++;
						if (m_Mines[row - 1][col - 1]) num++;
						if (m_Mines[row][col + 1]) num++;
						if (m_Mines[row][col - 1]) num++;
						if (m_Mines[row + 1][col + 1]) num++;
						if (m_Mines[row + 1][col - 1]) num++;
					}
				}
				m_Num.push_back(num);
			}
		}
		m_Nums.push_back(m_Num);
	}

	for (int row = 0; row < size; row++) {
		std::vector<bool> m_Open;
		for (int col = 0; col < size; col++) m_Open.push_back(false);
		m_Opens.push_back(m_Open);
	}

	for (int row = 0; row < size; row++) {
		std::vector<bool> m_Flag;
		for (int col = 0; col < size; col++) m_Flag.push_back(false);
		m_Flags.push_back(m_Flag);
	}
}

void CGameLayout::Open(int row, int col) {
	if (m_Opens[row][col]) return;
	else if (m_Mines[row][col]) return;
	else {
		if (m_Nums[row][col] > 0) {
			m_Opens[row][col] = true;
			return;
		}
		else {
			m_Opens[row][col] = true;
			if (!row) {
				Open(row + 1, col);
				if (!col) {
					Open(row, col + 1);
					Open(row + 1, col + 1);
				}
				else if (col == size - 1) {
					Open(row, col - 1);
					Open(row + 1, col - 1);
				}
				else {
					Open(row, col + 1);
					Open(row, col - 1);
					Open(row + 1, col + 1);
					Open(row + 1, col - 1);
				}
			}
			else if (row == size - 1) {
				Open(row - 1, col);
				if (!col) {
					Open(row, col + 1);
					Open(row - 1, col + 1);
				}
				else if (col == size - 1) {
					Open(row, col - 1);
					Open(row - 1, col - 1);
				}
				else {
					Open(row, col + 1);
					Open(row, col - 1);
					Open(row - 1, col + 1);
					Open(row - 1, col - 1);
				}
			}
			else {
				Open(row + 1, col);
				Open(row - 1, col);
				if (!col) {
					Open(row - 1, col + 1);
					Open(row, col + 1);
					Open(row + 1, col + 1);
				}
				else if (col == size - 1) {
					Open(row - 1, col - 1);
					Open(row, col - 1);
					Open(row + 1, col - 1);
				}
				else {
					Open(row - 1, col + 1);
					Open(row - 1, col - 1);
					Open(row, col + 1);
					Open(row, col - 1);
					Open(row + 1, col + 1);
					Open(row + 1, col - 1);
				}
			}
		}
	}
}

void CGameLayout::Update()
{
	int open_num = 0;
	int flag = 0;
	for (int row = 0; row < size; row++)
		for (int col = 0; col < size; col++) {
			if (m_Opens[row][col]) open_num++;
			if (m_Flags[row][col]) flag++;
		}

	win = size * size - mine == open_num;

	if (trap) {
		m_pScene->Render();
		DrawSceneTextPos("X", CKgPoint(25 * (endrow + 1) + 5, 25 * (endcol + 1) + 1), KG_COLOR_24_RGB(255, 0, 0));
		DrawSceneTextPos("You Lose! Time :", CKgPoint(0, 0), KG_COLOR_24_RGB(0, 0, 0));
		DrawSceneTextPos(std::to_string((int)(finish - start)).c_str(), CKgPoint(165, 0), KG_COLOR_24_RGB(0, 0, 0));

		for (int row = 0; row < size; row++)
			for (int col = 0; col < size; col++) {
				if (m_Opens[row][col]) {
					if (m_Nums[row][col]) DrawSceneTextPos(std::to_string(m_Nums[row][col]).c_str(), CKgPoint(25 * (row + 1) + 7, 25 * (col + 1) + 1),
						KG_COLOR_24_RGB(0, 0, 0));
					m_pRects[row][col]->ChangeColor(KG_COLOR_24_RGB(200, 200, 255), true);
				}
				if (m_Flags[row][col]) DrawSceneTextPos("*", CKgPoint(25 * (row + 1) + 8, 25 * (col + 1) + 5), KG_COLOR_24_RGB(255, 0, 0));
			}
	}
	else if (win) {
		if (time_) {
			finish = time(NULL);
			time_ = false;
			fin.open("LeaderBoard.txt");
			int t = 0;
			if (size == 10) t = 0;
			else if (size == 15) t = 3;
			else if (size == 30) t = 6;

			std::vector<int> num;
			std::vector<std::string> str;

			for (int k = 0; k < 9; k++) {
				getline(fin, difficulty);
				str.push_back(difficulty);
				getline(fin, difficulty);
				num.push_back(stoi(difficulty));
			}
			fin.close();

			if (num[t] > (int)(finish - start)) {
				num[t + 2] = num[t + 1];
				num[t + 1] = num[t];
				num[t] = (int)(finish - start);

				str[t + 2] = str[t + 1];
				str[t + 1] = str[t];
				str[t] = stt;
			}
			else if (num[t + 1] > (int)(finish - start)) {
				num[t + 2] = num[t + 1];
				num[t + 1] = (int)(finish - start);

				str[t + 2] = str[t + 1];
				str[t + 1] = stt;
			}
			else if (num[t + 2] > (int)(finish - start)) {
				num[t + 2] = (int)(finish - start);

				str[t + 2] = stt;
			}

			fout.open("LeaderBoard.txt");

			for (int k = 0; k < 9; k++) {
				fout << str[k] << '\n';
				fout << std::to_string(num[k]);
				if (k != 8) fout << '\n';
			}

			fout.close();
		}
		m_pScene->Render();
		DrawSceneTextPos("You Win! Congrats! Time :", CKgPoint(0, 0), KG_COLOR_24_RGB(0, 0, 0));
		DrawSceneTextPos(std::to_string((int)(finish - start)).c_str(), CKgPoint(257, 0), KG_COLOR_24_RGB(0, 0, 0));

		for (int row = 0; row < size; row++)
			for (int col = 0; col < size; col++) {
				if (m_Opens[row][col]) {
					if (m_Nums[row][col]) DrawSceneTextPos(std::to_string(m_Nums[row][col]).c_str(), CKgPoint(25 * (row + 1) + 7, 25 * (col + 1) + 1),
						KG_COLOR_24_RGB(0, 0, 0));
					m_pRects[row][col]->ChangeColor(KG_COLOR_24_RGB(200, 200, 255), true);
				}
				if (m_Flags[row][col]) DrawSceneTextPos("*", CKgPoint(25 * (row + 1) + 8, 25 * (col + 1) + 5), KG_COLOR_24_RGB(255, 0, 0));
			}
	}
	else {
		if (m_bMousePressed[0]) {
			int row = m_MousePosX / 25 - 1;
			int col = m_MousePosY / 25 - 1;

			if (row > -1 && row < size && col > -1 && col < size) {
				Open(row, col);
				if (m_Mines[row][col]) {
					trap = true;
					endrow = row;
					endcol = col;
					finish = time(NULL);
				}
			}
			m_bMousePressed[0] = false;
		}

		if (m_bMousePressed[2]) {
			int row = m_MousePosX / 25 - 1;
			int col = m_MousePosY / 25 - 1;

			if (row > -1 && row < size && col > -1 && col < size && !m_Opens[row][col]) m_Flags[row][col] = !m_Flags[row][col];

			m_bMousePressed[2] = false;
		}

		m_pScene->Render();
		DrawSceneTextPos(difficulty.c_str(), CKgPoint(0, 0), KG_COLOR_24_RGB(0, 0, 0));
		DrawSceneTextPos("Mines Remaining :", CKgPoint(95, 0), KG_COLOR_24_RGB(0, 0, 0));
		DrawSceneTextPos(std::to_string(mine - flag).c_str(), CKgPoint(275, 0), KG_COLOR_24_RGB(0, 0, 0));

		for (int row = 0; row < size; row++)
			for (int col = 0; col < size; col++) {
				if (m_Opens[row][col]) {
					if (m_Nums[row][col]) DrawSceneTextPos(std::to_string(m_Nums[row][col]).c_str(), CKgPoint(25 * (row + 1) + 7, 25 * (col + 1) + 1),
						KG_COLOR_24_RGB(0, 0, 0));
					m_pRects[row][col]->ChangeColor(KG_COLOR_24_RGB(200, 200, 255), true);
				}
				if (m_Flags[row][col]) DrawSceneTextPos("*", CKgPoint(25 * (row + 1) + 8, 25 * (col + 1) + 5), KG_COLOR_24_RGB(255, 0, 0));
			}
	}

	CKhuGleWin::Update();
}

int main()
{
	int input = 5;

	std::cout << "Input Your Name : ";
	std::cin >> stt;

	fin.open("LeaderBoard.txt");
	if (!fin) {
		fout.open("LeaderBoard.txt");
		fout << std::string("AAA\n999\nAAA\n999\nAAA\n999\nAAA\n999\nAAA\n999\nAAA\n999\nAAA\n999\nAAA\n999\nAAA\n999");
		fout.close();
	}
	fin.close();

	while (input) {

		time_ = true;
		std::cout << "|-------------------------|\n";
		std::cout << "|* M I N E S W E E P E R *|\n";
		std::cout << "|-------------------------|\n";
		std::cout << "|    Select Difficulty    |\n";
		std::cout << "|                         |\n";
		std::cout << "|    1. BackYard          |\n";
		std::cout << "|    2. WarZone           |\n";
		std::cout << "|    3. DMZ               |\n";
		std::cout << "|    4. LeaderBoard       |\n";
		std::cout << "|-------------------------|\n";
		std::cout << "|    0. Quit              |\n";
		std::cout << "|-------------------------|\n\n>> ";

		std::cin >> input;

		if (input == 1) {
			difficulty = "BackYard";

			CGameLayout* pGameLayout = new CGameLayout(300, 300);

			KhuGleWinInit(pGameLayout);
		}
		else if (input == 2) {
			difficulty = "WarZone";

			CGameLayout* pGameLayout = new CGameLayout(425, 425);

			KhuGleWinInit(pGameLayout);
		}
		else if (input == 3) {
			difficulty = "DMZ";

			CGameLayout* pGameLayout = new CGameLayout(800, 800);

			KhuGleWinInit(pGameLayout);
		}
		else if (input == 4) {
			std::cout << "\nLeaderBoard\n\nBackYard\n\n";

			fin.open("LeaderBoard.txt");
			for (int i = 0; i < 3; i++) {
				std::cout << i + 1 << ". ";
				getline(fin, difficulty);
				std::cout << difficulty << ' ';
				getline(fin, difficulty);
				std::cout << difficulty << '\n';
			}

			std::cout << "\nWarZone\n\n";

			for (int i = 0; i < 3; i++) {
				std::cout << i + 1 << ". ";
				getline(fin, difficulty);
				std::cout << difficulty << ' ';
				getline(fin, difficulty);
				std::cout << difficulty << '\n';
			}

			std::cout << "\nDMZ\n\n";

			for (int i = 0; i < 3; i++) {
				std::cout << i + 1 << ". ";
				getline(fin, difficulty);
				std::cout << difficulty << ' ';
				getline(fin, difficulty);
				std::cout << difficulty << '\n';
			}

			std::cout << '\n';
			fin.close();
		}
		else if (input == 0) {
			std::cout << "";
		}
		else {
			std::cout << "Wrong Command!\n";
		}
	}

	return 0;
}