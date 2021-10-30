#pragma once
#include <string>
#include <iomanip>
#include<iostream>
#include<sstream>
#include <Windows.h>


int ReadSector(LPCWSTR  drive, int readPoint, BYTE sector[512]);

uint64_t ReadIntReverse(uint8_t* byte, std::string offsetHex, unsigned int count);

int hexCharToInt(char a);

std::string hexToString(std::string str);

std::string ReadtoString(BYTE* data, std::string offsetHex, unsigned int bytes);
