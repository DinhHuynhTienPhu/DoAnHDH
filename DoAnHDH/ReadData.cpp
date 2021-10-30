﻿#include "ReadData.h"


using namespace std;

int ReadSector(LPCWSTR  drive, int readPoint, BYTE sector[512])
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

	}
}


uint64_t ReadIntReverse(uint8_t* byte, string offsetHex, unsigned int count)
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

int hexCharToInt(char a) {
	if (a >= '0' && a <= '9')
		return(a - 48);
	if (a >= 'A' && a <= 'Z')
		return(a - 55);
	return(a - 87);
}
string hexToString(string str) {
	stringstream HexString;
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
	stringstream ss;
	ss << hex;
	for (int i = offset; i < len; i++)
	{
		ss << setw(2) << setfill('0') << (int)data[i];
		ss << " ";
	}
	string result;
	while (!ss.eof())
	{
		string temp;
		ss >> temp;
		result += hexToString(temp);
	}
	return result;
}