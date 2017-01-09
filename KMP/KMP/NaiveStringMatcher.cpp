#include "stdafx.h"
#include <iostream>
#include <string>
using namespace std;
void NavieStringMatcher(string T, string P) {
	int nTLength=T.size();//Ä£°å
	int nPLength=P.size();
	int j;
	for (int i = 0; i < nTLength - nPLength+1; i++) {
		for (j = 0; j < nPLength; j++) {
			if (T[i + j] != P[j]) {
				break;
			}
		}
		if (j == nPLength) {
			cout << i << endl;
			break;
		}
	}
}