#include "ReadData.h"
#include "FAT32.h"

using namespace std;
#pragma warning(disable:4996)



int main(int argc, char** argv)
{
	BYTE bootSector[512];
	ReadData(L"\\\\.\\D:", 0, bootSector,512);// ổ đĩa cần đọc, offset đọc, buffer, số byte đọc

	FAT32 volume;
	volume.read(bootSector);
	volume.print();
}
