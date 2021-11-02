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





	//Test hàm byteArray()

	// Cách dùng hàm byteArray: truyền vào vector các cluster của dữ liệu(rdet, hoặc file txt), struct FAT32
	// Để dùng được data đã đọc, cần có size của data đó -> tính như công thức dataSize
	vector<int> a;
	a.push_back(88206);
	a.push_back(116020);
	a.push_back(116021);
	a.push_back(116022);
	a.push_back(116023);
	a.push_back(116024);
	a.push_back(116025);//Đây là các cluster của 1 file text trong USB của t, t push vào vector a để text thử
	byte* data = volume.byteArray(a, volume);
	int dataSize = a.size() * volume.sectorsPerCluster * volume.bytesPerSector;//Vì hàm ReadtoString() cần truyền số byte cần đọc, ở đây mình cần đọc hết mảng data luôn -> size bằng số cluster(size vector) *volume.sectorsPerCluster * volume.bytesPerSector -> số byte(số phần tử) trong mảng data
	cout << ReadtoString(data, "0", dataSize);
}
