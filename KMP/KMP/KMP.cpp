// KMP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "NaiveStringMatcher.h"
int* compute_overlay(string pattern) {
	int nPLen = pattern.size();
	int *overlay_function = new int[nPLen];
	int index;
	overlay_function[0] = -1;
	for (int i = 1; i < nPLen; i++) {
		index = overlay_function[i - 1];
		while (index >= 0 && pattern[i] != pattern[index + 1]) {//查找前面已匹配串的前缀+next是否等于当前后缀+新字符，否则迭代，直到停止
			index = overlay_function[index];
		}
		if (pattern[i] == pattern[index + 1]) {//如果匹配，则将重叠数组设为匹配到的字符的下标
			overlay_function[i] = index + 1;
		} else {
			overlay_function[i] = -1;
		}
	}
	return overlay_function;
}
void KMP(string T, string P) {
	int* next = compute_overlay(P);
	int i = 0;
	int j = 0;
	while (i < T.size() && j < P.size()) {
		if (T[i] == P[j]) {
			i++;
			j++;
		} else if (j == 0) {
			i++;
		} else {
			j = next[j - 1] + 1;//如果失配，则从已匹配串的重叠串的下一个字符重新开始匹配
		}
	}
	if (j == P.size()) {
		std::cout << "done" << endl;
	}
}
int main() {
	string T = "helloworlworld";
	string P = "world";
	NavieStringMatcher(T, P);
	compute_overlay("abaaa");
	KMP("abaababc", "abab");
	return 0;
}

