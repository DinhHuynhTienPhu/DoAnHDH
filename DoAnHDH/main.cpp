#include "ReadData.h"
#include "FAT32.h"
#include "NTFS.h"
#include"TxtFile.h"
#include<conio.h>
using namespace std;
#pragma warning(disable:4996)




int main(int argc, char** argv)
{
	//cout << "=======================PROJECT 01==========================\n";
	//BYTE bootSector[512];
	//FAT32 volume;
	//vector<TxtFile> txtFiles;
	//string vol;
	//cout << "Nhap ten o dia can doc, VD nhap(C, D, E): ";
	//cin >> vol;
	//string tempVol = "\\\\.\\" + vol + ":";
	//wstring stemp = s2ws(tempVol);
	//LPCWSTR drive = stemp.c_str();
	//ReadData(drive, 0, bootSector, 512);// ổ đĩa cần đọc, offset đọc, buffer, số byte đọc
	//volume.drive = drive;//lưu tên ổ đĩa lại vào volume, nếu cần dùng đến thì vào thuộc tính drive trong struct này

	//volume.read(bootSector);
	//volume.print();
	//cout << "\n========================\n";
	//cout << "Nhan Enter de xem cay thu muc:";
	//getch();
	//cout << endl;
	//vector<int> rdetClusters = clusterArray(volume,volume.startCluster);
	//vector<byte> rdetData = byteArray(volume,rdetClusters);
	//
	//ReadEntries(0, 0, rdetData, true,volume,txtFiles);
	//cout << "\n========================\n";
	//cout << "\nXuat noi dung cac file txt\n";
	//printTxtFile(txtFiles);

	BYTE bootSectorNTFS[512];
	NTFS volume;
	string vol;
	cout << "Nhap ten o dia can doc, VD nhap(C, D, E): ";
	cin >> vol;
	string tempVol = "\\\\.\\" + vol + ":";
	wstring stemp = s2ws(tempVol);
	LPCWSTR drive = stemp.c_str();
	ReadData(drive, 0, bootSectorNTFS, 512);// ổ đĩa cần đọc, offset đọc, buffer, số byte đọc
	volume.drive = drive;//lưu tên ổ đĩa lại vào volume, nếu cần dùng đến thì vào thuộc tính drive trong struct này

	volume.read(bootSectorNTFS);
	volume.print();
}
