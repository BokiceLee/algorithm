// Dijkstra.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <limits>
#include <vector>
#include <algorithm>
using namespace std;
class CPoint {
public:
	int id;
	int length;
	int last_id;
	CPoint(int id, int length, int last_id) :id(id), length(length), last_id(last_id) {
		;
	}
};
bool CompF(const CPoint* pl, const CPoint* pr) {
	return pl->length < pr->length;
}
int main() {
	int map[6][6] = {
		{0,6,1,5,INT_MAX,INT_MAX },
		{6,0,5,INT_MAX,3,INT_MAX },
		{1,5,0,5,6,4},
		{5,INT_MAX,5,0,INT_MAX,2},
		{INT_MAX,3,6,INT_MAX,0,6},
		{INT_MAX,INT_MAX,4,2,6,0}
	};
	vector<CPoint*> CPList;
	CPoint* cp = new CPoint(0, 0, -1);
	for (int i = 0; i < 6; i++) {
		CPoint* cp = new CPoint(i, map[0][i], map[0][i] == INT_MAX ? -1 : 0);
		CPList.push_back(cp);
	}
	vector<CPoint*> Res;
	vector<CPoint*>::iterator first;
	CPoint* tempPoint;
	while (CPList.size() > 0) {
		sort(CPList.begin(), CPList.end(), CompF);
		first = CPList.begin();
		tempPoint = *first;
		Res.push_back(tempPoint);
		CPList.erase(first);
		//与first相连并且在CPList中的，更新length
		vector<CPoint*>::iterator _iter = CPList.begin();
		for (_iter; _iter != CPList.end(); _iter++) {
			if (map[(*_iter)->id][tempPoint->id] != INT_MAX) {
				if ((*_iter)->length > map[(*_iter)->id][tempPoint->id] + tempPoint->length) {
					(*_iter)->length = map[(*_iter)->id][tempPoint->id] + tempPoint->length;
					(*_iter)->last_id = tempPoint->id;
				}
			}
		}
	}
	return 0;
}