#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>

#include "TxtFile.h"
using namespace std;
struct FAT32
{
	LPCWSTR drive;
	string fatCategory;// //offset 52 - 8 bytes
	string volumeType;//offset 15 - 1 byte, Loại volume được nhận biết bằng chuỗi thập lục phân (f8 là đĩa cứng)
	int bytesPerSector; //offset B - 2 bytes
	int sectorsPerCluster; //offset D - 1 byte
	int reservedSectors; //offset E - 2 bytes
	int fatCount; //offset 10 - 1 byte
	int sectorsPerTrack; //offset 18 - 2 byte
	int headsCount; //offset 1A - 2 bytes
	int hiddenSectors; //offset 1C - 4 bytes
	int volumeSize; //offset 20 - 4 bytes
	int fatSize; //offset 24 - 4 bytes
	int startCluster; //offset 2C - 4 bytes
	int secondaryInfoSector; //offset 30 - 2 bytes
	int bootCopySector; //offset 32 - 2 bytes
	void read(BYTE* sector);
	void print() const;
	
};

vector<byte> byteArray(FAT32 volume, vector<int> cluterArray);
vector<int> clusterArray(FAT32 volume, int startCluster);

vector<bool> ConvertByteToBoolArray(byte b);
vector<byte> ReadRawByte(int start, int length, vector<byte> data);//t doi ten vi o duoi t co xai de tach byte ra

void printInfoOfMainEntry(vector< BYTE> e);

void ReadEntries(int start, int tab, vector<BYTE> det, bool isRdet,FAT32 volume,vector<TxtFile> &txtFiles);

void folderHandler(string fileName, vector< byte> entry, int tab, FAT32 volume, vector< TxtFile> &txtFiles);
void fileHandler(string fileName, string extension, vector<byte> entry, int tab, FAT32 volume, vector<TxtFile> &txtFiles);
