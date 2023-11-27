//
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//
#include "KhuGleWin.h"
#include <iostream>
#include <random>
#include <chrono>
#include "KhuGleSignal.h"

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

class CStitchingLayer : public CKhuGleLayer {
public:
	CKhuGleSignal m_LeftImage, m_RightImage, m_Image;

	CStitchingLayer(int nW, int nH, KgColor24 bgColor, CKgPoint ptPos = CKgPoint(0, 0))
		: CKhuGleLayer(nW, nH, bgColor, ptPos) {}
	
	void DrawBackgroundImage();
};

void CStitchingLayer::DrawBackgroundImage()
{
	for (int y = 0; y < m_nH; y++)
		for (int x = 0; x < m_nW; x++)
		{
			m_ImageBgR[y][x] = KgGetRed(m_bgColor);
			m_ImageBgG[y][x] = KgGetGreen(m_bgColor);
			m_ImageBgB[y][x] = KgGetBlue(m_bgColor);
		}

	if (m_LeftImage.m_Red && m_LeftImage.m_Green && m_LeftImage.m_Blue)
	{
		for (int y = 0; y < m_LeftImage.m_nH && y < m_nH; ++y)
			for (int x = 0; x < m_LeftImage.m_nW && x < m_nW; ++x)
			{
				m_ImageBgR[y][x] = m_LeftImage.m_Red[y][x];
				m_ImageBgG[y][x] = m_LeftImage.m_Green[y][x];
				m_ImageBgB[y][x] = m_LeftImage.m_Blue[y][x];
			}
	}

	if (m_RightImage.m_Red && m_RightImage.m_Green && m_RightImage.m_Blue)
	{
		int OffsetX = 600, OffsetY = 0;
		for (int y = 0; y < m_RightImage.m_nH && y + OffsetY < m_nH; ++y)
			for (int x = 0; x < m_RightImage.m_nW && x + OffsetX < m_nW; ++x)
			{
				m_ImageBgR[y + OffsetY][x + OffsetX] = m_RightImage.m_Red[y][x];
				m_ImageBgG[y + OffsetY][x + OffsetX] = m_RightImage.m_Green[y][x];
				m_ImageBgB[y + OffsetY][x + OffsetX] = m_RightImage.m_Blue[y][x];
			}
	}

	if (m_Image.m_Red && m_Image.m_Green && m_Image.m_Blue)
	{
		int OffsetX = 150, OffsetY = 250;
		for (int y = 0; y < m_Image.m_nH && y + OffsetY < m_nH; ++y)
			for (int x = 0; x < m_Image.m_nW && x + OffsetX < m_nW; ++x)
			{
				m_ImageBgR[y + OffsetY][x + OffsetX] = m_Image.m_Red[y][x];
				m_ImageBgG[y + OffsetY][x + OffsetX] = m_Image.m_Green[y][x];
				m_ImageBgB[y + OffsetY][x + OffsetX] = m_Image.m_Blue[y][x];
			}
	}
}

class CStitching : public CKhuGleWin {
public:
	CStitchingLayer *m_pStitchingLayer;

	CStitching(int nW, int nH, char* ImagePath1, char* ImagePath2);
	void Update();
};

CStitching::CStitching(int nW, int nH, char* ImagePath1, char* ImagePath2)
	: CKhuGleWin(nW, nH) {
	m_pScene = new CKhuGleScene(1040, 780, KG_COLOR_24_RGB(100, 100, 150));
	m_pStitchingLayer = new CStitchingLayer(900, 700, KG_COLOR_24_RGB(150, 150, 200), CKgPoint(70, 40));

	m_pStitchingLayer->m_LeftImage.ReadBmp(ImagePath1);
	m_pStitchingLayer->m_RightImage.ReadBmp(ImagePath2);
	m_pStitchingLayer->m_Image.m_nW = 600;
	m_pStitchingLayer->m_Image.m_nH = 400;

	m_pStitchingLayer->m_Image.m_Red = cmatrix(m_pStitchingLayer->m_Image.m_nH, m_pStitchingLayer->m_Image.m_nW);
	m_pStitchingLayer->m_Image.m_Green = cmatrix(m_pStitchingLayer->m_Image.m_nH, m_pStitchingLayer->m_Image.m_nW);
	m_pStitchingLayer->m_Image.m_Blue = cmatrix(m_pStitchingLayer->m_Image.m_nH, m_pStitchingLayer->m_Image.m_nW);

	for (int y = 0; y < m_pStitchingLayer->m_Image.m_nH; ++y)
		for (int x = 0; x < m_pStitchingLayer->m_Image.m_nW; ++x) {
			m_pStitchingLayer->m_Image.m_Red[y][x] = 0;
			m_pStitchingLayer->m_Image.m_Green[y][x] = 0;
			m_pStitchingLayer->m_Image.m_Blue[y][x] = 0;
		}
	m_pStitchingLayer->DrawBackgroundImage();
	
	m_pScene->AddChild(m_pStitchingLayer);
}

int max = 0;

void CStitching::Update()
{
	if(m_bKeyPressed['S'] || m_bKeyPressed['L'])
	{
		CKgPoint m_point1[10] = { {290,39}, {263,86}, {294,167}, {219, 68}, {263,117}, {228,96}, {197,95},{281,73},{199,146},{280,176} };
		CKgPoint m_point2[10] = { {102,47}, {77,89}, {101,172},  {30, 71},  {75,123}, {37,100}, {61,103},{118,99},{90,152}, {171,88} };

		double** A = dmatrix(6, 6);
		double** B = dmatrix(20, 6);
		double** InverseA = dmatrix(6, 6);
		double* y = new double[6];
		double* z = new double[20];
		double* w = new double[6];

		// estimate w
		if (m_bKeyPressed['S']) {
			int temp[3] = { 0, 0, 0 };

			do {
				temp[0] = rand() % 10;
				temp[1] = rand() % 10;
				temp[2] = rand() % 10;
			} while (temp[0] == temp[1] || temp[0] == temp[2] || temp[1] == temp[2]);

			for (int row = 0; row < 6; row++)
				if (!(row % 2)) {
					A[row][0] = m_point1[temp[row / 2]].X;
					A[row][1] = m_point1[temp[row / 2]].Y;
					A[row][2] = 1;

					for (int col = 3; col < 6; col++) A[row][col] = 0;

					y[row] = m_point2[temp[row / 2]].X;
					y[row + 1] = m_point2[temp[row / 2]].Y;
				}
				else {
					for (int col = 0; col < 3; col++) A[row][col] = 0;

					A[row][3] = m_point1[temp[row / 2]].X;
					A[row][4] = m_point1[temp[row / 2]].Y;
					A[row][5] = 1;
				}

			InverseMatrix(A, InverseA, 6);

			for (int i = 0; i < 6; i++) {
				w[i] = 0;
				for (int j = 0; j < 6; j++) {
					w[i] += InverseA[i][j] * y[j];
				}
			}
		} else {
			for (int row = 0; row < 20; row++)
				if (!(row % 2)) {
					B[row][0] = m_point1[row / 2].X;
					B[row][1] = m_point1[row / 2].Y;
					B[row][2] = 1;

					for (int col = 3; col < 6; col++) B[row][col] = 0;

					z[row] = m_point2[row / 2].X;
					z[row + 1] = m_point2[row / 2].Y;
				}
				else {
					for (int col = 0; col < 3; col++) B[row][col] = 0;

					B[row][3] = m_point1[row / 2].X;
					B[row][4] = m_point1[row / 2].Y;
					B[row][5] = 1;
				}

			LeastSquared(B, w, z, 20, 6, true, 0.9);
		}

		int inliersCnt = 0;
		for (int i = 0; i < 10; ++i) {
			// count inliers
			int newX = w[0] * m_point1[i].X + w[1] * m_point1[i].Y + w[2];
			int newY = w[3] * m_point1[i].X + w[4] * m_point1[i].Y + w[5];
			int temp = std::sqrt(std::pow((newX - m_point2[i].X), 2) + std::pow((newY - m_point2[i].Y), 2));

			if (temp < 8) inliersCnt++;
		}

		std::cout << inliersCnt << std::endl;

		CKgPoint OrgPos(0, 100);
		for (int y = 0; y < m_pStitchingLayer->m_Image.m_nH; ++y) {
			for (int x = 0; x < m_pStitchingLayer->m_Image.m_nW; ++x) {
				m_pStitchingLayer->m_Image.m_Red[y][x] = 0;
				m_pStitchingLayer->m_Image.m_Green[y][x] = 0;
				m_pStitchingLayer->m_Image.m_Blue[y][x] = 0;

				if (y - OrgPos.Y > 0 && y - OrgPos.Y < m_pStitchingLayer->m_LeftImage.m_nH &&
					x - OrgPos.X > 0 && x - OrgPos.X < m_pStitchingLayer->m_LeftImage.m_nW) {
					m_pStitchingLayer->m_Image.m_Red[y][x] = m_pStitchingLayer->m_LeftImage.m_Red[y - OrgPos.Y][x - OrgPos.X];
					m_pStitchingLayer->m_Image.m_Green[y][x] = m_pStitchingLayer->m_LeftImage.m_Green[y - OrgPos.Y][x - OrgPos.X];
					m_pStitchingLayer->m_Image.m_Blue[y][x] = m_pStitchingLayer->m_LeftImage.m_Blue[y - OrgPos.Y][x - OrgPos.X];
				}

				int newX = w[0] * (x - OrgPos.X) + w[1] * (y-OrgPos.Y) + w[2];
				int newY = w[3] * (x - OrgPos.X) + w[4] * (y-OrgPos.Y) + w[5];

				if (newY > 0 && newY < m_pStitchingLayer->m_RightImage.m_nH &&
					newX > 0 && newX < m_pStitchingLayer->m_RightImage.m_nW) {
					m_pStitchingLayer->m_Image.m_Red[y][x] = m_pStitchingLayer->m_RightImage.m_Red[newY][newX];
					m_pStitchingLayer->m_Image.m_Green[y][x] = m_pStitchingLayer->m_RightImage.m_Green[newY][newX];
					m_pStitchingLayer->m_Image.m_Blue[y][x] = m_pStitchingLayer->m_RightImage.m_Blue[newY][newX];
				}
			}
		}
		
		free_dmatrix(A, 6, 6);
		free_dmatrix(B, 20, 6);
		free_dmatrix(InverseA, 6, 6);
		delete[] y;
		delete[] z;
		delete[] w;

		m_bKeyPressed['S'] = m_bKeyPressed['L'] = false;
	}

	m_pStitchingLayer->DrawBackgroundImage();
	m_pScene->Render();
	DrawSceneTextPos("Stitching", CKgPoint(0, 0));
	
	CKhuGleWin::Update();
}

int main()
{
	char ExePath[MAX_PATH], ImagePath1[MAX_PATH], ImagePath2[MAX_PATH];

	GetModuleFileName(NULL, ExePath, MAX_PATH);

	int i;
	int LastBackSlash = -1;
	int nLen = strlen(ExePath);
	for (i = nLen - 1; i >= 0; i--)
	{
		if (ExePath[i] == '\\') {
			LastBackSlash = i;
			break;
		}
	}

	if (LastBackSlash >= 0)
		ExePath[LastBackSlash] = '\0';

	sprintf(ImagePath1, "%s\\%s", ExePath, "panorama_image1.bmp");
	sprintf(ImagePath2, "%s\\%s", ExePath, "panorama_image2.bmp");

	CStitching* pStitching = new CStitching(1040, 780, ImagePath1, ImagePath2);
	KhuGleWinInit(pStitching);

	return 0;
}