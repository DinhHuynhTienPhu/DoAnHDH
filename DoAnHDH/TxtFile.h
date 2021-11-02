#pragma warning(disable:4996)
#pragma once
#include <string>
#include <Windows.h>
#include<vector>
using namespace std;
struct TxtFile
{
	string name;
	BYTE * data;
	int size;
	// nếu được thì xuất luôn có dấu
};
void  printTxtFile(vector<TxtFile> Files);
