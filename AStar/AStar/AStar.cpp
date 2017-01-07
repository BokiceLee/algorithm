// AStar.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;
class CPoint {
public:
	CPoint(int x, int y) :X(x), Y(y), m_pParentPoint(NULL), G(0), F(0), H(0) {

	}
	~CPoint() {

	}
	void calcF() {
		this->F = this->H + this->G;
	}
public:
	int X;
	int Y;
	int G;
	int H;
	int F;
	CPoint* m_pParentPoint;
};

bool CompF(const CPoint* pl, const CPoint* pr) {
	return pl->F < pr->F;
}
class CAstar {
private:
	static const int STEP = 10;
	static const int OBLIQUE = 14;
	typedef std::vector<CPoint*> tPointList;
	tPointList m_listOpen;
	tPointList m_listClose;
	int m_texttureMap[100][100];//m_*��ĳ�Ա����
public:
	CAstar(int textureMap[][12]) {
		for (int i = 0; i < 12; i++) {
			for (int j = 0; j < 12; j++) {
				cout << textureMap[i][j] << ' ';
			}
			cout << endl;
		}
		for (int i = 0; i < 100; i++) {
			for (int j = 0; j < 100; j++) {
				m_texttureMap[i][j] = 0;
			}
		}
		for (int i = 0; i < 100; i++) {
			for (int j = 0; j < 100; j++) {
				m_texttureMap[i][j] = textureMap[i][j];
			}
		}
	}
	CPoint* FindPath(CPoint* start, CPoint* end, bool IsIgnoreCorner) {
		m_listOpen.push_back(start);
		while (m_listOpen.size()) {
			CPoint* tempStart = getMinFPoint();//��ȡFֵ��С�ĵ㣬FΪ��ǰ�ڵ㵽��Ŀ�ĵ�Ԥ��ֵH+�����㵽��ǰ�ڵ����ƶ�ֵG
			removeFromOpenList(tempStart);//����ǰ�ڵ�ӿ����б��Ƴ����Ž��ر��б����������ڲ��Ҹýڵ�
			m_listClose.push_back(tempStart);
			std::vector<CPoint*> surroundPoints = SurroundPoints(tempStart, IsIgnoreCorner);//������Χ�ڵ�
			tPointList::iterator _iter = surroundPoints.begin();
			for (_iter; _iter != surroundPoints.end(); _iter++) {
				CPoint *point = *_iter;
				if (inOpenList(point->X, point->Y)) {//�ڿ����б��У����¼���Fֵ
					FoundPoint(tempStart, point);
				} else {//���ڿ����б��У�����
					NotFoundPoint(tempStart, end, point);
				}
			}
			if (inOpenList(end->X, end->Y)) {//���Ŀ����ڿ����б��У���ֹͣ������·��
				for (int i = 0; i < m_listOpen.size(); i++) {
					if (m_listOpen[i]->X == end->X&&m_listOpen[i]->Y == end->Y) {
						return m_listOpen[i];
					}
				}
			}
		}
		return end;
	}
	bool inCloseList(int x, int y) {
		CPoint* p = new CPoint(x, y);
		tPointList::iterator _iter = m_listClose.begin();
		for (_iter; _iter != m_listClose.end(); _iter++) {
			CPoint* temp = *_iter;
			if (temp->X == p->X&&temp->Y == p->Y) {
				return true;
			}
		}
		if (p) {
			delete p;
			p = NULL;
		}
		return false;
	}
	bool inOpenList(int x, int y) {
		CPoint *p = new CPoint(x, y);
		tPointList::iterator _iter = m_listOpen.begin();
		for (_iter; _iter != m_listOpen.end(); _iter++) {
			CPoint* temp = *_iter;
			if (temp->X == p->X&&temp->Y == p->Y) {
				return true;
			}
		}
		if (p) {
			delete p;
			p = NULL;
		}
		return false;
	}
	bool CanReach(int x, int y) {
		return m_texttureMap[x][y] == 0;
	}
	bool CanReach(CPoint* start, int x, int y, bool IsIgnoreCorner) {
		if (!CanReach(x, y) || inCloseList(x, y)) {//��ǽ�����Ѿ�����ر��б��򲻿ɴ�
			return false;
		} else {
			if ((abs(x - start->X) + abs(y - start->Y)) == 1) {//�������������
				return true;
			} else {
				if (CanReach(abs(x - 1), y) && CanReach(x, abs(y - 1))) {//����ڶԽǣ����ҿ�ͨ���������ҵ���
					return true;
				} else {//����
					return IsIgnoreCorner;
				}
			}
		}
	}
	std::vector<CPoint*> SurroundPoints(CPoint* point, bool IsIgnoreCorner) {
		tPointList surroundPoints;
		for (int x = point->X - 1; x <= point->X + 1; x++) {
			for (int y = point->Y - 1; y <= point->Y + 1; y++) {
				if (CanReach(point, x, y, IsIgnoreCorner)) {
					CPoint* p = new CPoint(x, y);
					surroundPoints.push_back(p);
				} else {

				}
			}
		}
		return surroundPoints;
	}
	CPoint* getMinFPoint() {
		tPointList tempList;
		for (int i = 0; i < (int)m_listOpen.size(); i++) {
			tempList.push_back(m_listOpen[i]);
		}
		sort(tempList.begin(), tempList.end(), CompF);
		if (tempList.size()) {
			return tempList[0];
		}
	}
	void removeFromOpenList(CPoint* point) {
		tPointList::iterator _iter = m_listOpen.begin();
		for (_iter; _iter != m_listOpen.end(); ++_iter) {
			m_listOpen.erase(_iter);
			break;
		}
	}
	void FoundPoint(CPoint* tempStart, CPoint* point) {
		int G = CalcG(tempStart, point);
		if (G < point->G) {
			point->m_pParentPoint = tempStart;
			point->G = G;
			point->calcF();
		}
	}
	void NotFoundPoint(CPoint* tempStart, CPoint* end, CPoint* point) {
		point->m_pParentPoint = tempStart;
		point->G = CalcG(tempStart, point);
		point->H = CalcH(end, point);
		point->calcF();
		m_listOpen.push_back(point);
	}
	int CalcG(CPoint* start, CPoint* point) {
		int G = (abs(point->X - start->X) + abs(point->Y - start->Y)) == 2 ? STEP : OBLIQUE;
		int parentG = point->m_pParentPoint != NULL ? point->m_pParentPoint->G : 0;
		return G + parentG;
	}
	int CalcH(CPoint* end, CPoint* point) {
		int step = abs(point->X - end->X) + abs(point->Y - end->Y);
		return step*STEP;
	}
};
int main() {
	int array[12][12] = {
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1 },
		{ 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1 },
		{ 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
	};
	CAstar* maze = new CAstar(array);
	CPoint* start = new CPoint(1, 1);
	CPoint* end = new CPoint(6, 10);
	CPoint* parent = maze->FindPath(start, end, false);
	while (parent != NULL) {
		cout << parent->X << "," << parent->Y << endl;
		parent = parent->m_pParentPoint;
	}
	return 0;
}

