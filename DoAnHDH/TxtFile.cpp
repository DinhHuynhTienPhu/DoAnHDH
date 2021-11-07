#pragma once
#include "TxtFile.h"
#include "FAT32.h"
#include "ReadData.h"

void printTxtFile(vector<TxtFile> Files)
{
	string  FileName;
	byte* FileData;
	int FileSize;
	for (int i = 0; i < Files.size(); i++)
	{
		FileName = Files.at(i).name;
		FileData = Files.at(i).data;
		FileSize = Files.at(i).size;
		cout << "FileName: " << FileName;
		
		cout << endl;
		printTextData(FileData, FileSize);
		cout << "\n=====================\n";
	}
}
