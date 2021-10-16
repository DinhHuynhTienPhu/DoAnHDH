#include <windows.h>
#include <iomanip>      // std::setw
#include <stdio.h>
#include <cstdint>
#include<iostream>
#include<sstream>
using namespace std;
#pragma warning(disable:4996)


#pragma region  code cua thay
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
#pragma endregion


uint64_t ReadIntReverse(uint8_t* byte,int start, unsigned int count)
{
    /*truyền vào toàn bộ mảng, vị trí bắt đầu (decimal), số lượng byte đọc       
       trả về giá trị cần đọc(decimal)*/

    byte += start;
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
    ReadSector(L"\\\\.\\G:", 0, sector);

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
    else if (a >= 'A' && a <= 'Z')
        return(a - 55);
    else
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


string ReadtoString(BYTE* data, int offset, unsigned int bytes)
{

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
int main(int argc, char** argv)
{
    BYTE sector[512];
    ReadSector(L"\\\\.\\G:", 0, sector);

    //demo đọc thử
    cout << "So byte tren sector: " << endl;
    cout << ReadIntReverse(sector,11,2) << endl;

    cout << "\nLoai fat: " << ReadtoString(sector, 82, 5);

    // sử dung code này để đọc fakebootsector
    //BYTE* test= ReadFakeBootSector();
    //for (int i = 0; i < 512;i++) {
    //    if (sector[i] == test[i]) cout << "this byte is right ";
    //    else cout << "Something is wrong i can feel it";
    //}


}
