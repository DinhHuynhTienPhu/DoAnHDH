#include "NTFS.h"

void NTFS::read(BYTE* sector)
{
	bytesPerSector = ReadIntReverse(sector, "B", 2); //offset B - 2 bytes
	sectorsPerCluster = ReadIntReverse(sector, "D", 1); //offset D - 1 byte
	sectorsPerTrack = ReadIntReverse(sector, "18", 2); //offset 18 - 2 byte
	headsCount = ReadIntReverse(sector, "1A", 2); //offset 1A - 2 bytes
	sectorStart = ReadIntReverse(sector, "1C", 4); //offset 1C - 4 bytes
	driveSize = ReadIntReverse(sector, "28", 8); //offset 20 - 4 bytes
	startCluster = ReadIntReverse(sector, "30", 8); //offset 30 - 8 bytes
	mtfSize = pow(2.0, abs(converter(sector, "40", 1)));//offset 40 - 1 byte
	sectorsPerIndexBuffer = ReadIntReverse(sector, "44", 1); //offset 44 - 1 byte
}

void NTFS::print() const
{
	cout << "+ So byte cua sector: " << bytesPerSector << " bytes" << endl;
	cout << "+ So sector tren cluster: " << sectorsPerCluster << " sectors" << endl;
	cout << "+ So sector cua track: " << sectorsPerTrack << " sectors" << endl;
	cout << "+ So luong dau doc: " << headsCount << endl;
	cout << "+ Sector bat dau cua o dia logic: " << sectorStart << endl;
	cout << "+ So sector cua o dia: " << driveSize << " sectors" << endl;
	cout << "+ Cluster bat dau cua MFT: " << startCluster << endl;
	cout << "+ Kich thuoc cua MFT: " << mtfSize << " bytes" << endl;
	cout << "+ So cluster cua Index Buffer: " << sectorsPerIndexBuffer << endl;
}

string getBit(int x) {
	string bit, bits;

	for (int i = 0; i < 4; i++)
	{
		bit = 48 + ((x >> (3 - i)) & 1);
		if (bit == "1") {
			bit = "0";
		}
		else {
			bit = "1";
		}
		bits.append(bit);
	}

	return bits;
}

int bitToInt(string bit) {
	int result = 0;

	for (int i = 1; i < bit.size(); i++)
	{
		result += (bit[i] - 48) * (pow(2.0, (7.0 - i)));
	}

	return result;
}

string hexStr(BYTE* data, int len)
{
	std::stringstream ss;
	ss << std::hex;

	for (int i(0); i < len; ++i)
		ss << std::setw(2) << std::setfill('0') << (int)data[i];

	return ss.str();
}

int converter(uint8_t* byte, string offsetHex, int len)
{
	const int offset = stoi(offsetHex, nullptr, 16);

	byte += offset;
	string s = hexStr(byte, len);
	int a = 0;
	int x = hexCharToInt(s[0]);
	int y = hexCharToInt(s[1]);
	string temp;
	string bit;
	temp = getBit(x);
	bit.append(getBit(x));
	bit.append(getBit(y));

	if (bit[0] == '0') {
		a = (bitToInt(bit) + 1) * -1;
	}
	else {
		a = bitToInt(bit) + 1;
	}

	return a;
}