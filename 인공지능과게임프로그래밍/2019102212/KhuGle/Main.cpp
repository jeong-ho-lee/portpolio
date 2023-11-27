//
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//
#include "KhuGleWin.h"
#include "KhuGleSignal.h"
#include <iostream>
#include <random>
#include <chrono>
#include <fstream>
#include <string>

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

class CKhuGleGraphLayer : public CKhuGleLayer
{
public:
	int m_nCurrentCnt;
	std::vector<std::vector<double>> m_Data;
	std::vector<double> m_MaxData;
	int m_nDataTotal;
	double m_TrainAccuacy, m_TrainLoss;

	CKhuGleGraphLayer(int nW, int nH, KgColor24 bgColor, int nDataTotal, CKgPoint ptPos = CKgPoint(0, 0));
	void SetMaxData(int nIndex, double Value);
	void DrawBackgroundImage();
};

CKhuGleGraphLayer::CKhuGleGraphLayer(int nW, int nH, KgColor24 bgColor, int nDataTotal, CKgPoint ptPos)
	: m_MaxData(nDataTotal), m_Data(nDataTotal), CKhuGleLayer(nW, nH, bgColor, ptPos)
{
	m_bgColor = bgColor;
	m_nCurrentCnt = 0;
	m_nDataTotal = nDataTotal;

	m_TrainAccuacy = 0.;
	m_TrainLoss = 2.5;
}

void CKhuGleGraphLayer::SetMaxData(int nIndex, double Value)
{
	m_MaxData[nIndex] = Value;
}

void CKhuGleGraphLayer::DrawBackgroundImage()
{
	for(int y = 0 ; y < m_nH ; y++)
		for(int x = 0 ; x < m_nW ; x++)
		{
			m_ImageBgR[y][x] = KgGetRed(m_bgColor);
			m_ImageBgG[y][x] = KgGetGreen(m_bgColor);
			m_ImageBgB[y][x] = KgGetBlue(m_bgColor);
		}

	int xx0, yy0, xx1, yy1;
	for(int k = 0 ; k < m_nDataTotal ; ++k)
	{
		KgColor24 Color = KG_COLOR_24_RGB(k%2*255, k/2%2*255, k/4%2*255);
		for(int i = 0 ; i < m_nCurrentCnt ; ++i) {
			xx1 = i*m_nW/m_nCurrentCnt;
			yy1 = (int)(m_nH - m_Data[k][i]*m_nH/m_MaxData[k] - 1);
			if(yy1 < 0) yy1 = 0;
			if(yy1 >= m_nH) yy1 = m_nH-1;

			if(i > 0) {
				CKhuGleSprite::DrawLine(m_ImageBgR, m_ImageBgG, m_ImageBgB, m_nW, m_nH, 
					xx0, yy0, xx1, yy1, Color);
			}

			xx0 = xx1;
			yy0 = yy1;
		}
	}
}

class CKhuGleRocLayer : public CKhuGleLayer
{
public:
	std::vector<std::pair<int, double>> m_Data;
	std::vector<std::pair<double, double>> m_Positive;
	std::vector<std::string> m_Head;
	std::vector<std::vector<double>> m_ReadData;

	CKhuGleRocLayer(int nW, int nH, KgColor24 bgColor, CKgPoint ptPos = CKgPoint(0, 0))
		: CKhuGleLayer(nW, nH, bgColor, ptPos)
	{
		MakeData();
		ComputePositives();
		DrawBackgroundImage();
	}
	void MakeData();
	void ComputePositives();
	void DrawBackgroundImage();
};

bool DistanceSort(std::pair<double, int> first, std::pair<double, int> second) {
	return first.first < second.first;
}

void CKhuGleRocLayer::MakeData()
{
	m_Data.clear();

	srand((unsigned)time(0));
	std::vector<bool> Train(m_ReadData.size());

	int nTrainCnt = 0, nTestCnt = 0;
	for (int i = 0; i < m_ReadData.size(); i++) {
		if (rand() % 5 < 4) {
			Train[i] = true;
			nTrainCnt++;
		}
		else {
			Train[i] = false;
			nTestCnt++;
		}
	}

	std::cout << "\n*************************" << std::endl;
	std::cout << "Total Data : " << m_ReadData.size() << std::endl;
	std::cout << "Train Data : " << nTrainCnt << std::endl;
	std::cout << "Test Data : " << nTestCnt << std::endl;

	srand((unsigned)time(0));
	int nK = rand() % 13 + 2;

	std::cout << "Test : Tau > k / 2 (k = " << nK << ")\n";

	double nTP = 0; double nFP = 0; double nFN = 0; double nTN = 0;
	std::vector<std::pair<double, int>> NN;

	for (int i = 0; i < m_ReadData.size(); i++) {
		NN.clear();
		double Dist = 0;

		if (Train[i]) continue;
		auto testData = m_ReadData[i];

		for (int k = 0; k < m_ReadData.size(); k++) {
			if (!Train[k]) continue;
			auto trainData = m_ReadData[k];

			for (int j = 0; j < testData.size(); ++j) {
				double data1 = testData[j];
				double data2 = trainData[j];

				Dist += (data1 - data2) * (data1 - data2);
			}
			Dist = sqrt(Dist);
			NN.push_back({ Dist, (int)m_ReadData[k][m_ReadData[k].size() - 1] });
		}

		std::sort(NN.begin(), NN.end(), DistanceSort);

		int decision = 0;
		for (int k = 0; k < nK; ++k) decision += NN[k].second;

		m_Data.push_back({ (int)m_ReadData[i][m_ReadData[i].size() - 1], (double)decision / (double)nK });

		if (decision > nK / 2.) decision = 1;
		else decision = 0;

		if (decision == 1) {
			int real = (int)m_ReadData[i][m_ReadData[i].size() - 1];
			if (real == 1) nTP++;
			else nFP++;
		}
		else {
			int real = (int)m_ReadData[i][m_ReadData[i].size() - 1];
			if (real == 1) nFN++;
			else nTN++;
		}
	}
	std::cout << "True Positive Rate : " << (double)nTP / (double)(nTP + nFN) * 100 << "%\n";
	std::cout << "False Positive Rate : " << (double)nFP / (double)(nTN + nFP) * 100 << "%\n";
	std::cout << "Accuracy : " << (double)(nTP + nTN) / (double)(nTP + nFP + nFN + nTN) * 100 << "%\n";
}

void CKhuGleRocLayer::ComputePositives()
{
	m_Positive.clear();

	for(int nThreshold = 0 ; nThreshold <= 100 ; nThreshold += 1)
	{
		double TP = 0, FP = 0;
		int nPositiveCnt = 0;
		for(auto &Data : m_Data)
		{
			if(Data.second >= nThreshold/100.)
			{
				if(Data.first == 1)
					TP++;
				else
					FP++;
			}

			if(Data.first == 1)
				nPositiveCnt++;
		}

		TP /= nPositiveCnt;
		FP /= (m_Data.size()-nPositiveCnt);

		m_Positive.push_back({TP, FP});
	}
}

void CKhuGleRocLayer::DrawBackgroundImage()
{
	for(int y = 0 ; y < m_nH ; y++)
		for(int x = 0 ; x < m_nW ; x++)
		{
			m_ImageBgR[y][x] = KgGetRed(m_bgColor);
			m_ImageBgG[y][x] = KgGetGreen(m_bgColor);
			m_ImageBgB[y][x] = KgGetBlue(m_bgColor);
		}

	int xx0, yy0, xx1, yy1;
	bool bFirst = true;
	for(auto &Positive : m_Positive)
	{
		xx1 = (int)(Positive.second * (m_nW-1));
		yy1 = m_nH-(int)(Positive.first * (m_nH-1))-1;

		if(!bFirst)
			CKhuGleSprite::DrawLine(m_ImageBgR, m_ImageBgG, m_ImageBgB, m_nW, m_nH, 
				xx0, yy0, xx1, yy1, KG_COLOR_24_RGB(0, 255, 0));

		bFirst = false;
		
		xx0 = xx1;
		yy0 = yy1;
	}
}

void ReadCsv(std::string FileName, std::vector<std::string>& Head, std::vector<std::vector<double>>& Data) {
	Head.clear();
	Data.clear();

	std::ifstream ifs;

	ifs.open(FileName);
	if (!ifs.is_open()) return;

	std::string LineString = "";
	std::string Delimeter = ",";
	bool bHead = true;
	while (getline(ifs, LineString))
	{
		std::vector<double> RowData;

		unsigned int nPos = 0, nFindPos;
		do {
			nFindPos = LineString.find(Delimeter, nPos);
			if (nFindPos == std::string::npos) nFindPos = LineString.length();

			if (bHead)
				Head.push_back(LineString.substr(nPos, nFindPos - nPos));
			else
				RowData.push_back(std::stod(LineString.substr(nPos, nFindPos - nPos)));

			nPos = nFindPos + 1;
		} while (nFindPos < LineString.length());

		if (bHead) {
			bHead = false;
		}
		else
			Data.push_back(RowData);
	}

	ifs.close();
}

class CPerformance : public CKhuGleWin
{
public:
	CKhuGleRocLayer *m_pRocLayer;
		
	CPerformance(int nW, int nH, std::string CsvPath);
	void Update();
};

CPerformance::CPerformance(int nW, int nH, std::string CsvPath) : CKhuGleWin(nW, nH) 
{
	m_pScene = new CKhuGleScene(520, 530, KG_COLOR_24_RGB(100, 100, 150));

	m_pRocLayer = new CKhuGleRocLayer(480, 480, KG_COLOR_24_RGB(150, 150, 200), CKgPoint(20, 30));
	m_pScene->AddChild(m_pRocLayer);

	ReadCsv(CsvPath, m_pRocLayer->m_Head, m_pRocLayer->m_ReadData);

	for (auto& column : m_pRocLayer->m_Head) std::cout << column << ", ";
	std::cout << std::endl;

	std::vector<double> mean(m_pRocLayer->m_Head.size() - 1);
	std::vector<double> sd(m_pRocLayer->m_Head.size() - 1);
	for (auto& read : m_pRocLayer->m_ReadData)
		for (int k = 0; k < mean.size(); ++k) {
			mean[k] += read[k];
			sd[k] += read[k] * read[k];
		}

	for (int k = 0; k < mean.size(); k++) {
		mean[k] /= m_pRocLayer->m_ReadData.size();

		sd[k] = sd[k] / m_pRocLayer->m_ReadData.size() - mean[k] * mean[k];
		sd[k] = sqrt(sd[k]);
	}

	int nPrintCnt = 0;
	for (auto& read : m_pRocLayer->m_ReadData) {
		for (auto& column : read) std::cout << column << ", ";
		std::cout << std::endl;
		
		if (nPrintCnt++ > 10) break;
	}

	for (auto& read : m_pRocLayer->m_ReadData)
		for (int k = 0; k < mean.size(); k++) read[k] = (read[k] - mean[k]) / sd[k];

	std::cout << "\nZ - Score Normalization" << std::endl;
	nPrintCnt = 0;
	for (auto& read : m_pRocLayer->m_ReadData) {
		for (auto& column : read) std::cout << column << ", ";
		std::cout << std::endl;

		if (nPrintCnt++ > 10) break;
	}

	m_pRocLayer->MakeData();
	m_pRocLayer->ComputePositives();
	m_pRocLayer->DrawBackgroundImage();
}

void CPerformance::Update()
{
	if(m_bKeyPressed['N'])
	{
		m_pRocLayer->MakeData();
		m_pRocLayer->ComputePositives();
		m_pRocLayer->DrawBackgroundImage();

		m_bKeyPressed['N'] = false;
	}

	m_pScene->Render();
	DrawSceneTextPos("Performance Evaluation", CKgPoint(0, 0));

	CKhuGleWin::Update();
}

int main()
{
	std::string CsvPath;
	CsvPath = std::string("C:\\인공지능과게임프로그래밍\\2019102212\\Run\\diabetes.csv");

	CPerformance *pPerformance = new CPerformance(520, 530, CsvPath);

	KhuGleWinInit(pPerformance);

	return 0;
}