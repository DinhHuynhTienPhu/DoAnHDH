#include "ReadData.h"
#include "FAT32.h"

using namespace std;
#pragma warning(disable:4996)

int main(int argc, char** argv)
{
	BYTE bootSector[512];
	//ReadData(L"\\\\.\\D:", 0, bootSector, 512);// ổ đĩa cần đọc, offset đọc, buffer, số byte đọc
	ReadData(L"\\\\.\\F:", 0, bootSector,512);// ổ đĩa cần đọc, offset đọc, buffer, số byte đọc

	FAT32 volume;
	volume.read(bootSector);
	volume.print();

	// Đoạn này tớ test, đọc vào rồi xuất ra xem thử
	// Vì nó đọc 1 cluster = 8 sector nên khi xuất nó dư ra 8 dòng Success! í các cậu
	
	//vector<int> a;
	//a.push_back(2); // usb của tớ chỉ dùng 1 cluster và bắt đầu từ 2 nên tớ hard code lun
	//
	//vector<BYTE> b = volume.byteArray(a, volume);

	//cout << "size of b: " << b.size() << endl;

	//for (int i = 0; i < b.size(); i++)
	//{
	//	cout << i + 1 << ". '" << b[i] << "'" << endl;
	//}
}
