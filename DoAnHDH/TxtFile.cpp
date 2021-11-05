#pragma once
#include "TxtFile.h"
#include "FAT32.h"
#include "ReadData.h"

void printTxtFile(vector<TxtFile> Files)
{
	vector<vector<uint8_t>>  FileName;
	byte* FileData;
	int FileSize;
	for (int i = 0; i < Files.size(); i++)
	{
		FileName = Files.at(i).name;
		FileData = Files.at(i).data;
		FileSize = Files.at(i).size;
		cout << "FileName: ";
		for (int y = FileName.size() - 1; y >= 0; y--) {
			for (int p = 0; p < FileName[y].size(); p++)
				cout << (char)FileName[y][p];
		}
		cout << endl;
		cout << printTextData(FileData, FileSize);
		cout << "\n=====================\n";
	}
}
