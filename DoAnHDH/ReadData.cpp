#include "ReadData.h"

#include <vector>


using namespace std;

int ReadData(LPCWSTR  drive, int readPoint, BYTE *sector, int bytes)
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

	if (!ReadFile(device, sector, bytes, &bytesRead, NULL))
	{
		printf("ReadFile: %u\n", GetLastError());

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

//xuất text từ byte*, nếu dùng hàm ReadtoString để in file text, tốc độ rất chậm
void printTextData(byte* data,int size)//có thể không cần đến size
{
	if(data!=nullptr)
	{
		string text((char*)data);

		cout << text;
	}
	
}

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}
