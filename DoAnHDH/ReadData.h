#pragma once
#include <string>
#include <iomanip>
#include<iostream>
#include<sstream>
#include <Windows.h>

int ReadData(LPCWSTR drive, int readPoint, BYTE* sector, int bytes);// thêm tham số bytes cần đọc -> đây không còn là hàm đọc sector nên t đổi tên

uint64_t ReadIntReverse(uint8_t* byte, std::string offsetHex, unsigned int count);

int hexCharToInt(char a);

std::string hexToString(std::string str);

std::string ReadtoString(BYTE* data, std::string offsetHex, unsigned int bytes);

std::string printTextData(BYTE* data, int size);//xuất text từ byte*, nếu dùng hàm ReadtoString để in file text, tốc độ rất chậm
