#include "FAT32.h"
#include <algorithm>
#include "ReadData.h"
#include "TxtFile.h"

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

vector<byte> byteArray(FAT32 volume,vector<int> cluterArray)
{

	vector<BYTE> ByteArray;

	// Duyệt qua mảng các cluster
	for (int i = 0; i < cluterArray.size(); i++)
	{
		int offsetStart = (volume.reservedSectors + volume.fatCount * volume.fatSize + (cluterArray[i] - 2) * volume.sectorsPerCluster) * volume.bytesPerSector;
		// Mỗi cluster có số sector = sectorsPerCluster
		int sizeOfCluster = volume.bytesPerSector * volume.sectorsPerCluster;
		BYTE* cluster = new BYTE[sizeOfCluster];

		// ổ đĩa cần đọc, offset bắt đầu đọc, buffer, số byte đọc
		ReadData(volume.drive, offsetStart, cluster, sizeOfCluster);

		// Nối mảng sector vào ByteArray
		for (int j = 0; j < sizeOfCluster; j++)
		{
			ByteArray.push_back(cluster[j]);
		}
	}

	return ByteArray;
}


vector<int> clusterArray(FAT32 volume, int startCluster)
{
	vector<int>result;
	if(startCluster<2)//có trường hợp file rỗng k có có cluster nào (start cluster = 0)
	{
		return result;//trả về luôn vector rỗng
	}
	
	int bytesPerFAT = volume.fatSize * volume.bytesPerSector, //số byte của bảng FAT
		startOffset = volume.reservedSectors * volume.bytesPerSector; //offset đầu tiên của bảng FAT

	BYTE* FAT = new BYTE[bytesPerFAT];
	ReadData(volume.drive,startOffset , FAT, bytesPerFAT);	// đọc bảng FAT

	int cluster = startCluster,
		offset;
	string offsetHex;
	std::stringstream ss;

	while (cluster != 268435455)
	{
		result.push_back(cluster);
		offset = cluster * 4;

		ss.clear();
		ss << std::hex << std::uppercase << offset;
		ss >> offsetHex;
		cluster = ReadIntReverse(FAT, offsetHex, 4);
	}
	return result;
}

vector<bool> ConvertByteToBoolArray(byte b)
{

	vector<bool> result;
	bool value;
	for (int i = 0; i < 8; i++)
	{
		value = (b & (1 << i)) == 0 ? false : true;
		result.push_back(value);
	}


	return result;
}

vector<byte> ReadRawByte(int start, int length, vector<byte> data)
{
	byte value;
	vector<byte> res;
	for (int i = start; i < start + length; i++)
	{
		value = data[i];
		res.push_back(value);
	}
	return res;
}

void printInfoOfMainEntry(vector<BYTE> e)
{
	cout << "  Trang thai:";
	cout << "  ";
	if (ConvertByteToBoolArray(e[11])[0]) cout << "read only ";
	if (ConvertByteToBoolArray(e[11])[1]) cout << "hidden ";
	if (ConvertByteToBoolArray(e[11])[2]) cout << "system ";
	if (ConvertByteToBoolArray(e[11])[3]) cout << "volabel ";
	if (ConvertByteToBoolArray(e[11])[4]) cout << "directory ";
	if (ConvertByteToBoolArray(e[11])[5]) cout << "archive ";

}

void ReadEntries(int start, int tab, vector<BYTE> det, bool isRdet, FAT32 volume, vector<TxtFile> &txtFiles)
{
	int i = start;
	if (!isRdet) i += 64;// bo qua 2 entry dau cua cac entry phu
	while (i < det.size())
	{
		vector<byte> entry = ReadRawByte(i, 32, det);

		if (entry[0] == 0x0 || entry[0] == 0xE5)
		{
			i += 32;
			continue;
		}
		if (entry[11] == 0x0F)//entry phu
		{
			string fileNameToken;
			string fileName;

			while (entry[11] == 0x0F)
			{
				for (int l = 1; l < 32; l++)
				{
					if (l == 1 || l == 3 || l == 5 || l == 7 || l == 9 || l == 14 || l == 16 || l == 18 || l == 20 || l == 22 || l == 24 || l == 28 || l == 30) //chỉ làm được tên file không dấu
					{
						char t = entry[l];
						fileNameToken.push_back(t);
					}
				}
				fileName.insert(0, fileNameToken);
				fileNameToken.clear();
				i += 32;
				entry = ReadRawByte(i, 32, det);

			}

			//
			vector<bool> status = ConvertByteToBoolArray(entry[11]);
			if (status[4])//folder
			{
				folderHandler(fileName, entry, tab,volume,txtFiles);

			}
			else if (status[5])//file
			{
				string extension = fileName.substr(fileName.find_last_of('.') + 1);
				fileHandler(fileName, extension, entry, tab,volume,txtFiles);

			}
		}
		else//entry chinh
		{
			vector<bool> status = ConvertByteToBoolArray(entry[11]);
			if (status[4])//folder
			{
				vector<byte> nameInByte = ReadRawByte(0, 11, entry);
				nameInByte.push_back(0);// them /0 vao cuoi chuoi
				byte* temp = &nameInByte[0];
				string fileName((char*)temp);
				folderHandler(fileName, entry, tab, volume,txtFiles);
			}
			else if (status[5])//file
			{
				vector<byte> nameInByte = ReadRawByte(0, 8, entry);
				vector<byte> entensionInByte = ReadRawByte(8, 3, entry);

				int i = 7;
				while (nameInByte[i] == 0x20)
				{
					nameInByte.pop_back();

					i--;
				}
				nameInByte.push_back(0);// them /0 vao cuoi chuoi
				entensionInByte.push_back(0);// them /0 vao cuoi chuoi

				byte* temp = &nameInByte[0];
				byte* temp2 = &entensionInByte[0];
				string name((char*)temp);
				string extension((char*)temp2);
				string fileName = name + "." + extension;

				fileHandler(fileName, extension, entry, tab, volume, txtFiles);//:)) bug

			}
			else//không là file hoặc folder thì bỏ qua
			{
				i += 32;
				continue;
			}
		}
		i += 32;
	}
}

void folderHandler(string fileName, vector<byte> entry, int tab, FAT32 volume, vector< TxtFile> &txtFiles)
{
	for (int i = 0; i < tab; i++)//tab ra
	{
		cout << "\t";
	}
	cout << fileName;
	cout << " | ";
	printInfoOfMainEntry(entry);
	cout << " | ";

	vector<byte> highWordByte = ReadRawByte(0x14, 2, entry);
	byte* highWord = highWordByte.data();
	vector<byte> lowWordByte = ReadRawByte(0x1A, 2, entry);
	byte* lowWord = lowWordByte.data();
	int startCluster = ReadIntReverse(highWord, "0", 2) * 256 + ReadIntReverse(lowWord, "0", 2);
	vector<int> clusters = clusterArray(volume, startCluster);
	cout << "Cac sector: ";
	if (clusters.size() <= 1)
	{
		for (int i = 0; i < clusters.size(); i++)
		{
			int start = volume.reservedSectors + volume.fatCount * volume.fatSize + (clusters[i] - 2) * volume.sectorsPerCluster;
			int end = volume.reservedSectors + volume.fatCount * volume.fatSize + (clusters[i] - 2) * volume.sectorsPerCluster + volume.sectorsPerCluster - 1;
			cout << start << "->" << end << "; ";
		}
	}
	else
	{
		clusters.push_back(-1);
		int start = volume.reservedSectors + volume.fatCount * volume.fatSize + (clusters[0] - 2) * volume.sectorsPerCluster;
		int end = volume.reservedSectors + volume.fatCount * volume.fatSize + (clusters[0] - 2) * volume.sectorsPerCluster + volume.sectorsPerCluster - 1;
		int nextSector = volume.reservedSectors + volume.fatCount * volume.fatSize + (clusters[1] - 2) * volume.sectorsPerCluster;
		for (int i = 1; i < clusters.size() - 1; i++)
		{
			if (nextSector != end + 1)// nếu 2 cluster không liên tục thì xuất ra luôn, vd: 128->135; 144->151
			{
				cout << start << "->" << end << "; ";
				start = volume.reservedSectors + volume.fatCount * volume.fatSize + (clusters[i] - 2) * volume.sectorsPerCluster;
				end = volume.reservedSectors + volume.fatCount * volume.fatSize + (clusters[i] - 2) * volume.sectorsPerCluster + volume.sectorsPerCluster - 1;
				nextSector = volume.reservedSectors + volume.fatCount * volume.fatSize + (clusters[i + 1] - 2) * volume.sectorsPerCluster;

			}
			else// nếu 2 cluster liên tục thì chưa xuất vội, cộng dồn end đến khi end+1 khác nextSector mới xuất ra một lần
			{
				end = volume.reservedSectors + volume.fatCount * volume.fatSize + (clusters[i] - 2) * volume.sectorsPerCluster + volume.sectorsPerCluster - 1;
				nextSector = volume.reservedSectors + volume.fatCount * volume.fatSize + (clusters[i + 1] - 2) * volume.sectorsPerCluster;
			}
		}
		cout << start << "->" << end << "; ";
	}
	cout << endl;

		//de quy folder, tab + 1

		vector<byte> sdet = byteArray(volume, clusters);
		ReadEntries(0, tab + 1, sdet, false,volume, txtFiles);
}

void fileHandler(string fileName, string extension, vector<byte> entry, int tab, FAT32 volume,vector<TxtFile> &txtFiles)
{
	for (int i = 0; i < tab; i++)
	{
		cout << "\t";
	}
	cout << fileName;
	cout << " | ";
	printInfoOfMainEntry(entry);
	cout << " | ";
	int size = ReadIntReverse(&entry[0], "1C", 4);
	cout << "Kich thuoc: " << size << " B";

	cout << " | ";

	vector<byte> highWordByte = ReadRawByte(0x14, 2, entry);
	byte* highWord = highWordByte.data();
	vector<byte> lowWordByte = ReadRawByte(0x1A, 2, entry);
	byte* lowWord = lowWordByte.data();
	int startCluster = ReadIntReverse(highWord, "0", 2) * 256 + ReadIntReverse(lowWord, "0", 2);
	vector<int> clusters = clusterArray(volume, startCluster);
	cout << "Cac sector: ";
	/*for (int i = 0; i < clusters.size(); i++)
	{
		int start = volume.reservedSectors + volume.fatCount * volume.fatSize + (clusters[i] - 2) * volume.sectorsPerCluster;
		int end = volume.reservedSectors + volume.fatCount * volume.fatSize + (clusters[i] - 2) * volume.sectorsPerCluster + volume.sectorsPerCluster;
		cout << start << "->" << end << "; ";
	}*/

	if (clusters.size() <= 1)
	{
		for (int i = 0; i < clusters.size(); i++)
		{
			int start = volume.reservedSectors + volume.fatCount * volume.fatSize + (clusters[i] - 2) * volume.sectorsPerCluster;
			int end = volume.reservedSectors + volume.fatCount * volume.fatSize + (clusters[i] - 2) * volume.sectorsPerCluster + volume.sectorsPerCluster -1;
			cout << start << "->" << end << "; ";
		}
	}
	else
	{
		clusters.push_back(-1);//vì vòng lặp dưới sẽ truy xuất đến phần tử cuối + 1
		int start = volume.reservedSectors + volume.fatCount * volume.fatSize + (clusters[0] - 2) * volume.sectorsPerCluster;
		int end = volume.reservedSectors + volume.fatCount * volume.fatSize + (clusters[0] - 2) * volume.sectorsPerCluster + volume.sectorsPerCluster-1;
		int nextSector = volume.reservedSectors + volume.fatCount * volume.fatSize + (clusters[1] - 2) * volume.sectorsPerCluster;
		for (int i = 1; i < clusters.size() - 1; i++)
		{
			if (nextSector != end + 1)// nếu 2 cluster không liên tục thì xuất ra luôn, vd: 128->135; 144->151
			{
				cout << start << "->" << end << "; ";
				start = volume.reservedSectors + volume.fatCount * volume.fatSize + (clusters[i] - 2) * volume.sectorsPerCluster;
				end = volume.reservedSectors + volume.fatCount * volume.fatSize + (clusters[i] - 2) * volume.sectorsPerCluster + volume.sectorsPerCluster -1;
				nextSector = volume.reservedSectors + volume.fatCount * volume.fatSize + (clusters[i + 1] - 2) * volume.sectorsPerCluster;

			}
			else// nếu 2 cluster liên tục thì chưa xuất vội, cộng dồn end đến khi end+1 khác nextSector mới xuất ra một lần
			{
				end = volume.reservedSectors + volume.fatCount * volume.fatSize + (clusters[i] - 2) * volume.sectorsPerCluster + volume.sectorsPerCluster -1;
				nextSector = volume.reservedSectors + volume.fatCount * volume.fatSize + (clusters[i + 1] - 2) * volume.sectorsPerCluster;
			}
		}
		cout << start << "->" << end << "; ";
	}

	

	if (extension.find("TXT") != string::npos || extension.find("txt") != string::npos)
	{

		byte* temp = nullptr;
		if (size != 0)
		{
			vector<byte> textData = byteArray(volume, clusters);
			temp = new byte[size + 1];
			copy(&textData[0], &textData[0] + size, temp);
			temp[size] = 0;
		}

		TxtFile newFile{ fileName, temp,size };
		txtFiles.push_back(newFile);
		cout << "Da luu lai file txt, se xuat sau cay thu muc";
	}
	else
	{
		cout << "Hay dung phan mem tuong thich de doc noi dung";
	}
	cout << endl;
}
