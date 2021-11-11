#include "ReadData.h"
#include "FAT32.h"
#include"TxtFile.h"
#include "NTFS.h"
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable::4996)

using namespace std;


int main(int argc, char** argv)
{
	//_setmode(_fileno(stdin), _O_U16TEXT);
	//_setmode(_fileno(stdout), _O_U16TEXT);
	//wcout << "=======================PROJECT 01==========================\n";
	//BYTE bootSector[512];
	//FAT32 volume;
	//vector<TxtFile> txtFiles;
	//wstring vol;
	//wcout << L"Nhập tên ổ đĩa cần đọc, VD nhập (C,D,E): ";
	//wcin >> vol;
	//string Vol1;
	//transform(vol.begin(), vol.end(), std::back_inserter(Vol1), [](char c) {
	//	return (char)c;
	//	});
	//string tempVol = "\\\\.\\" + Vol1 + ":";
	//wstring stemp = s2ws(tempVol);
	//LPCWSTR drive = stemp.c_str();
	//
	//ReadData(drive, 0, bootSector, 512);// ổ đĩa cần đọc, offset đọc, buffer, số byte đọc
	//volume.drive = drive;//lưu tên ổ đĩa lại vào volume, nếu cần dùng đến thì vào thuộc tính drive trong struct này
	//volume.read(bootSector);
	//volume.print();
	//wcout << L"\n========================\n";
	//wcout << L"Nhấn enter để xem cây thư mục\n";
	//getch();
	//wcout << endl;
	//vector<int> rdetClusters = clusterArray(volume,volume.startCluster);
	//vector<byte> rdetData = byteArray(volume,rdetClusters);
	//
	//ReadEntries(0, 0, rdetData, true,volume,txtFiles);
	//wcout << L"\n========================\n";
	//wcout << L"Xuất nội dung các file txt\n";
	//printTxtFile(txtFiles);


	BYTE bootSectorNTFS[512];
	NTFS volume;
	wstring vol;
	wcout << L"Nhập tên ổ đĩa cần đọc, VD nhập (C,D,E): ";
	wcin >> vol;
	string Vol1;
	transform(vol.begin(), vol.end(), std::back_inserter(Vol1), [](char c) {
		return (char)c;
		});
	string tempVol = "\\\\.\\" + Vol1 + ":";
	wstring stemp = s2ws(tempVol);
	LPCWSTR drive = stemp.c_str();
	ReadData(drive, 0, bootSectorNTFS, 512);// ổ đĩa cần đọc, offset đọc, buffer, số byte đọc
	volume.drive = drive;//lưu tên ổ đĩa lại vào volume, nếu cần dùng đến thì vào thuộc tính drive trong struct này

	volume.read(bootSectorNTFS);
	volume.print();
	


}
