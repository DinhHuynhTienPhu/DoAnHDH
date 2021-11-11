#include "NTFS.h"
#include <algorithm>
#include "ReadData.h"
#include "TxtFile.h"

void NTFS::read(BYTE* sector)
{
	bytesPerSector = ReadIntReverse(sector, "B", 2); //offset B - 2 bytes
	sectorsPerCluster = ReadIntReverse(sector, "D", 1); //offset D - 1 byte
	sectorsPerTrack = ReadIntReverse(sector, "18", 2); //offset 18 - 2 byte
	headsCount = ReadIntReverse(sector, "1A", 2); //offset 1A - 2 bytes
	sectorStart = ReadIntReverse(sector, "1C", 4); //offset 1C - 4 bytes
	driveSize = ReadIntReverse(sector, "28", 8); //offset 20 - 4 bytes
	startCluster = ReadIntReverse(sector, "30", 8); //offset 30 - 8 bytes
	sectorsPerIndexBuffer = ReadIntReverse(sector, "44", 1); //offset 44 - 1 byte
}

void NTFS::print() const
{
	cout << "+ So byte cua sector: " << bytesPerSector << " bytes" << endl;
	cout << "+ So sector tren cluster: " << sectorsPerCluster << " sectors" << endl;
	cout << "+ So sector cua track: " << sectorsPerTrack << " sectors" << endl;
	cout << "+ So luong dau doc: " << headsCount << endl;
	cout << "+ Sector bat dau cua o dia logic: " << sectorStart << " sectors" << endl;
	cout << "+ So sector cua o dia: " << driveSize << " sectors" << endl;
	cout << "+ Cluster bat dau cua MFT: " << startCluster << endl;
	cout << "+ So cluster cua Index Buffer: " << sectorsPerIndexBuffer << endl;
}