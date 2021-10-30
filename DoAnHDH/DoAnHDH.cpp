#include "ReadData.h"
#include "FAT32.h"

using namespace std;
#pragma warning(disable:4996)



int main(int argc, char** argv)
{
	BYTE bootSector[512];
	ReadSector(L"\\\\.\\D:", 0, bootSector);// nếu dùng USB thì thay 'D' bằng tên ký tự của USB

	FAT32 volume;
	volume.read(bootSector);
	volume.print();
}
