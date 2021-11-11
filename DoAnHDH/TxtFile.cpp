#pragma once
#include "TxtFile.h"
#include "FAT32.h"
#include "ReadData.h"

void printTxtFile(vector<TxtFile> Files)
{
	wstring  FileName;
	byte* FileData;
	int FileSize;
	for (int i = 0; i < Files.size(); i++)
	{
		wcout << "File " << i+1 << ": \n";
		FileName = Files.at(i).name;
		FileData = Files.at(i).data;
		FileSize = Files.at(i).size;
		wcout << "- FileName: " << FileName;
		wcout << endl;
		wcout << "- Content:\n";
		printTextData(FileData, FileSize);
		wcout << "\n=====================\n";
	}
}
