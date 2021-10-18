#include <windows.h>
#include <iomanip>      // std::setw
#include <stdio.h>
#include <cstdint>
#include<iostream>
#include<sstream>
using namespace std;
#pragma warning(disable:4996)


#pragma region  code cua thay
int ReadSector(LPCWSTR  drive, int readPoint, BYTE sector[512], bool& isRead)
{
	int retCode = 0;
	DWORD bytesRead;
	HANDLE device = NULL;
	device = CreateFile(drive,    // Drive to open
		GENERIC_READ,           // Access mode
		FILE_SHARE_READ | FILE_SHARE_WRITE,        // Share Mode
		NULL,                   // Security Descriptor
		OPEN_EXISTING,          // How to create
		0,                      // File attributes
		NULL);                  // Handle to template

	if (device == INVALID_HANDLE_VALUE) // Open Error
	{
		printf("CreateFile: %u\n", GetLastError());

		return 1;
	}

	SetFilePointer(device, readPoint, NULL, FILE_BEGIN);//Set a Point to Read

	if (!ReadFile(device, sector, 512, &bytesRead, NULL))
	{
		printf("ReadFile: %u\n", GetLastError());

	}
	else
	{
		printf("Success!\n");
		isRead = true;

	}
}
#pragma endregion


uint64_t ReadIntReverse(uint8_t* byte,string offsetHex, unsigned int count)
{
	/*truyền vào toàn bộ mảng, vị trí bắt đầu (hexString), số lượng byte đọc       
	   trả về giá trị cần đọc(decimal)*/
	const int offset = stoi(offsetHex, nullptr, 16);

	byte += offset;
	uint64_t result = 0;
	int i;
	for (i = count - 1; i >= 0; i--)
		result = (result << 8) | byte[i];

	return result;
}
BYTE* ReadFakeBootSector() {
	/*Tui đã lưu 512byte của bootsector về dưới dạng tập tin nhị phân, sử dụng hàm này để đọc nó lên fakebootsector.fake*/

	FILE* fptr;
	BYTE sector2[512];

	if ((fptr = fopen("fakebootsector.fake", "rb")) == NULL) {
		printf("Error! opening file");

		exit(1);
	}

	for (int i = 0; i < 512; i++)
	{
		fread(&sector2[i], sizeof(BYTE), 1, fptr);
	}
	fclose(fptr);
	return sector2;
}
void WriteFakeBootSector() {
	/*Sử dụng hàm này để lưu bootsector đọc được về thành file nhị phân fakebootsector.fake*/
	BYTE sector[512];
	bool isRead=false;
	ReadSector(L"\\\\.\\G:", 0, sector,isRead);

	FILE* fptr;

	if ((fptr = fopen("fakebootsector.fake", "wb")) == NULL) {
		printf("Error! opening file");

		exit(1);
	}

	for (int i = 0; i < 512; i++)
	{
		fwrite(&sector[i], sizeof(BYTE), 1, fptr);
	}
	fclose(fptr);

}
int hexCharToInt(char a) {
	if (a >= '0' && a <= '9')
		return(a - 48);
	if (a >= 'A' && a <= 'Z')
		return(a - 55);
		return(a - 87);
}
string hexToString(string str) {
	std::stringstream HexString;
	for (int i = 0; i < str.length(); i++) {
		char a = str.at(i++);
		char b = str.at(i);
		int x = hexCharToInt(a);
		int y = hexCharToInt(b);
		HexString << (char)((16 * x) + y);
	}
	return HexString.str();
}


string ReadtoString(BYTE* data, string offsetHex, unsigned int bytes)//Tùng sửa tham số offset hàm này
{
	const int offset = stoi(offsetHex, nullptr, 16);

	int len = offset + bytes;
	std::stringstream ss;
	ss << std::hex;
	for (int i = offset; i < len; i++)
	{
		ss << setw(2) << setfill('0') << (int)data[i];
		ss << " ";
	}
	string result;
	while (!ss.eof())
	{
		std::string temp;
		ss >> temp;
		result += hexToString(temp);
	}
	return result;
}




void ReadBootSector(BYTE* sector)//In cac thong tin o boot sector
{
	string fatCategory = ReadtoString(sector, "52", 8);// //offset 52 - 8 bytes
	std::stringstream ss;
	ss << std::hex;
	ss << setw(2) << setfill('0') << (int)sector[21];
	string volumeType;//offset 15 - 1 byte, Loại volume được nhận biết bằng chuỗi thập lục phân (f8 là đĩa cứng)
	ss >> volumeType;
	int bytesPerSector = ReadIntReverse(sector, "B", 2); //offset B - 2 bytes
	//cout<<"So byte cua sector: " << bytesPerSector;//"So byte cua sector: " -- chuỗi bên trái là cái này
	//An xuất các thông tin dưới đây(phần chuỗi bên trái xuất giống slide của thầy ), dùng hàm ReadIntReverse() như ví dụ trên, trình bày đẹp mắt nhưng đùng màu mè:))
	//An làm xong, Mai kiểm tra lại thông tin được xuất với slide của thầy và xuất thêm đơn vị vào sau (bytes, sector, nếu là index thì bỏ qua)
	int sectorsPerCluster = ReadIntReverse(sector, "D", 1); //offset D - 1 byte
	int reservedSectors = ReadIntReverse(sector, "E", 2); //offset E - 2 bytes
	int fatCount = ReadIntReverse(sector, "10", 1); //offset 10 - 1 byte
	int sectorsPerTrack = ReadIntReverse(sector, "18", 2); //offset 18 - 2 byte
	int headsCount = ReadIntReverse(sector, "1A", 2); //offset 1A - 2 bytes
	int hiddenSectors = ReadIntReverse(sector, "1C", 4); //offset 1C - 4 bytes
	int volumeSize = ReadIntReverse(sector, "20", 4); //offset 20 - 4 bytes
	int fatSize = ReadIntReverse(sector, "24", 4); //offset 24 - 4 bytes
	int startCluster = ReadIntReverse(sector, "2C", 4); //offset 2C - 4 bytes
	int secondaryInfoSector = ReadIntReverse(sector, "30", 2); //offset 30 - 2 bytes
	int bootCopySector = ReadIntReverse(sector, "32", 2); //offset 32 - 2 bytes

	cout << "+ Loai FAT: " << fatCategory<<endl;
	cout << "+ Loai volume: " << volumeType<<endl;
	cout << "+ So byte cua sector: " << bytesPerSector << " bytes" << endl;
	cout << "+ So sector tren cluster: SC = " << sectorsPerCluster << " sectors" << endl;
	cout << "+ So sector thuoc vung Bootsector: SB = " << reservedSectors << " sectors" <<endl;
	cout << "+ So bang FAT: NF = " << fatCount << endl;
	cout << "+ So sector cua track: " << sectorsPerTrack << " sectors" << endl;
	cout << "+ So luong dau doc: " << headsCount << endl;
	cout << "+ So sector an truoc volume: " << hiddenSectors << " sectors" << endl;
	cout << "+ Kich thuoc volume: SV = " << volumeSize << " sectors" << endl;
	cout << "+ Kich thuoc moi bang FAT: SF = " << fatSize << " sectors" << endl;
	cout << "+ Cluster bat dau cua RDET: " << startCluster << endl;
	cout << "+ Sector chua thong tin phu (ve cluster trong): " << secondaryInfoSector << endl;
	cout << "+ Sector chua ban sao cua Bootsector: " << bootCopySector << endl;




	//cout << volumeType << endl << fatCategory;
}


int main(int argc, char** argv)
{
	BYTE sector[512];
	bool isRead =false;
	ReadSector(L"\\\\.\\D:", 0, sector, isRead);// nếu dùng USB thì thay 'D' bằng tên ký tự của USB


	// sử dung code này để đọc fakebootsector
	//BYTE* test= ReadFakeBootSector();
	//for (int i = 0; i < 512;i++) {
	//    if (sector[i] == test[i]) cout << "this byte is right ";
	//    else cout << "Something is wrong i can feel it";
	//}

	if(isRead==true)
		ReadBootSector(sector);

}
