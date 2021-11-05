#pragma warning(disable:4996)
#pragma once
#include <string>
#include <Windows.h>
#include<vector>
using namespace std;
struct TxtFile
{
	vector<vector<uint8_t>> name;
	BYTE * data;
	int size;
	// nếu được thì xuất luôn có dấu
};
void  printTxtFile(vector<TxtFile> Files);
