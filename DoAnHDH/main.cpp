#include "ReadData.h"
#include "FAT32.h"

using namespace std;
#pragma warning(disable:4996)

int main(int argc, char** argv)
{
	BYTE bootSector[512];
	FAT32 volume;
	LPCWSTR drive = L"\\\\.\\D:";
	

	ReadData(drive, 0, bootSector, 512);// ổ đĩa cần đọc, offset đọc, buffer, số byte đọc
	volume.drive = drive;//lưu tên ổ đĩa lại vào volume, nếu cần dùng đến thì vào thuộc tính drive trong struct này

	volume.read(bootSector);
	volume.print();

}
