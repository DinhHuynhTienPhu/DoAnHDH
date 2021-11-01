#include "FAT32.h"

#include "ReadData.h"

void FAT32::read(BYTE* sector)
{
	fatCategory = ReadtoString(sector, "52", 8);// //offset 52 - 8 bytes
	std::stringstream ss;
	ss << std::hex;
	ss << setw(2) << setfill('0') << (int)sector[21];
	ss >> volumeType;
	bytesPerSector = ReadIntReverse(sector, "B", 2); //offset B - 2 bytes
	sectorsPerCluster = ReadIntReverse(sector, "D", 1); //offset D - 1 byte
	reservedSectors = ReadIntReverse(sector, "E", 2); //offset E - 2 bytes
	fatCount = ReadIntReverse(sector, "10", 1); //offset 10 - 1 byte
	sectorsPerTrack = ReadIntReverse(sector, "18", 2); //offset 18 - 2 byte
	headsCount = ReadIntReverse(sector, "1A", 2); //offset 1A - 2 bytes
	hiddenSectors = ReadIntReverse(sector, "1C", 4); //offset 1C - 4 bytes
	volumeSize = ReadIntReverse(sector, "20", 4); //offset 20 - 4 bytes
	fatSize = ReadIntReverse(sector, "24", 4); //offset 24 - 4 bytes
	startCluster = ReadIntReverse(sector, "2C", 4); //offset 2C - 4 bytes
	secondaryInfoSector = ReadIntReverse(sector, "30", 2); //offset 30 - 2 bytes
	bootCopySector = ReadIntReverse(sector, "32", 2); //offset 32 - 2 bytes
}

void FAT32::print() const
{
	cout << "+ Loai FAT: " << fatCategory << endl;
	cout << "+ Loai volume: " << volumeType << endl;
	cout << "+ So byte cua sector: " << bytesPerSector << " bytes" << endl;
	cout << "+ So sector tren cluster: SC = " << sectorsPerCluster << " sectors" << endl;
	cout << "+ So sector thuoc vung Bootsector: SB = " << reservedSectors << " sectors" << endl;
	cout << "+ So bang FAT: NF = " << fatCount << endl;
	cout << "+ So sector cua track: " << sectorsPerTrack << " sectors" << endl;
	cout << "+ So luong dau doc: " << headsCount << endl;
	cout << "+ So sector an truoc volume: " << hiddenSectors << " sectors" << endl;
	cout << "+ Kich thuoc volume: SV = " << volumeSize << " sectors" << endl;
	cout << "+ Kich thuoc moi bang FAT: SF = " << fatSize << " sectors" << endl;
	cout << "+ Cluster bat dau cua RDET: " << startCluster << endl;
	cout << "+ Sector chua thong tin phu (ve cluster trong): " << secondaryInfoSector << endl;
	cout << "+ Sector chua ban sao cua Bootsector: " << bootCopySector << endl;
}

vector<BYTE> FAT32::byteArray(vector<int> cluterArray, FAT32 volume)
{
	vector<BYTE> ByteArray;

	// Duyệt qua mảng các cluster
	for (int i = 0; i < cluterArray.size(); i++)
	{
		int offsetStart = volume.reservedSectors * volume.bytesPerSector + volume.fatCount * volume.fatSize * volume.bytesPerSector + (cluterArray[i] - 2) * volume.sectorsPerCluster;

		// Mỗi cluster có số sector = sectorsPerCluster
		int sizeOfCluster = volume.bytesPerSector * volume.sectorsPerCluster;
		BYTE* cluster = new BYTE[sizeOfCluster];

		// ổ đĩa cần đọc, offset bắt đầu đọc, buffer, số byte đọc
		//ReadData(L"\\\\.\\D:", offsetStart, cluster, sizeOfCluster);
		ReadData(L"\\\\.\\F:", offsetStart, cluster, sizeOfCluster);

		// Nối mảng sector vào ByteArray
		for (int j = 0; j < sizeOfCluster; j++)
		{
			ByteArray.push_back(cluster[j]);
		}
	}

	return ByteArray;
}
