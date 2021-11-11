#pragma warning(disable:4996)
#pragma once
#include <string>
#include <Windows.h>
#include<vector>
using namespace std;
struct TxtFile
{
	wstring name;
	byte* data;
	int size;
};
void  printTxtFile(vector<TxtFile> Files);
