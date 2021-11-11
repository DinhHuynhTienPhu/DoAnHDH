#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>

#include "TxtFile.h"
using namespace std;
struct NTFS
{
	LPCWSTR drive;
	int bytesPerSector; //offset B - 2 bytes
	int sectorsPerCluster; //offset D - 1 byte
	int sectorsPerTrack; //offset 18 - 2 bytes
	int headsCount; //offset 1A - 2 bytes
	int sectorStart; //offset 1C - 4 bytes
	int driveSize; //offset 28 - 8 bytes
	int startCluster; //offset 30 - 8 bytes
	int sectorsPerIndexBuffer; //offset 44 - 1 byte
	void read(BYTE* sector);
	void print() const;
};