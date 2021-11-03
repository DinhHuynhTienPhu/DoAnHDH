#pragma once
#include <string>
#include <iomanip>
#include<iostream>
#include<sstream>
#include <Windows.h>
#include<string>



typedef struct DirectoryEntry {
    uint8_t name[8];
    uint8_t fileType[3];
    uint8_t attrib;
    uint8_t userattrib;
    uint8_t Millisecondtime;

    uint16_t createtime;
    uint16_t createdate;
    uint16_t accessdate;
    uint16_t clusterhigh;

    uint16_t modifiedtime;
    uint16_t modifieddate;
    uint16_t clusterlow;
    uint8_t filesize[4];

} mainEntry;

typedef struct ShortFileName {
    uint8_t sequenceNo;            // Sequence number, 0xe5 for delete file
    uint8_t fileName_Part1[10];    // file name part - 5 UNICODE 
    uint8_t fileattribute;         // File attibute 

    uint8_t reserved_1;
    uint8_t checksum;

    uint8_t fileName_Part2[12];    // 6 WORD 
    uint8_t NOTHING[2];
    uint8_t fileName_Part3[4];     // 2 WORD NEXT
} extraEntry;


int ReadData(LPCWSTR drive, int readPoint, BYTE* sector, int bytes);// thêm tham số bytes cần đọc -> đây không còn là hàm đọc sector nên t đổi tên

uint64_t ReadIntReverse(uint8_t* byte, std::string offsetHex, unsigned int count);

int hexCharToInt(char a);

std::string hexToString(std::string str);

std::string ReadtoString(BYTE* data, std::string offsetHex, unsigned int bytes);
std::string ReadtoStringi(BYTE* data, int offset, unsigned int bytes);
