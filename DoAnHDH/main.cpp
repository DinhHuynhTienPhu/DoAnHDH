#include "ReadData.h"
#include "FAT32.h"
#include <array>

using namespace std;
#pragma warning(disable:4996)


  bool* ConvertByteToBoolArray(byte b)
{
	// prepare the return result
	  bool result[8];// = new bool[8];

	// check each bit in the byte. if 1 set to true, if 0 set to false
	for (int i = 0; i < 8; i++)
		result[i] = (b & (1 << i)) == 0 ? false : true;

	// reverse the array
	int start = 0, end = 7;
	while (start < end)
	{
		int temp = result[start];
		result[start] = result[end];
		result[end] = temp;
		start++;
		end--;
	}

	return result;
}
BYTE* ReadRawEntry(int i, BYTE* mang) {
	BYTE e[32];
	for (int j = 0; j < 32; j++) {
		e[j] = mang[i];
		i++;
	}
	return e;
}
void printEntry(BYTE* e) {
	for (int i = 0; i < 32; i++) {
		if (i % 16 == 0) cout<<endl;
		printf("%d", e[i]);
		cout << "\t";
	}
}
void printNameOfExtraEntry(BYTE* e) {
	
	for (int l = 1; l < 32; l++) {
		if (l  == 1||l==3||l==5||l==7||l==9||l==14||l==16||l==18||l==20||l==22||l==24||l==28||l==30) {
			cout << (char)e[l]<<" ";
		}
	}
}

void printInfoOfMainEntry(BYTE* e) {
	cout << "  ";
	cout << e[8] << e[9] << e[10];
	if (ConvertByteToBoolArray(e[11])[0]) cout << "read only ";
	if (ConvertByteToBoolArray(e[11])[1]) cout << "hidden ";
	if (ConvertByteToBoolArray(e[11])[2]) cout << "system ";
	if (ConvertByteToBoolArray(e[11])[3]) cout << "volabel ";
	if (ConvertByteToBoolArray(e[11])[4]) cout << "directory ";
	if (ConvertByteToBoolArray(e[11])[5]) cout << "archive ";
	
}

void ReadRDETOrSDet(int sectorbatdau, int solantab, BYTE* mang,int sizemang) { //function đọc RDETorSDET, truyền vào sector bắt đầu và số lần tab (số lần tab cũng có nghĩa là số thư mục cha của rdet/sdet này, cũng có nghĩa là số lần đệ quy) 
	//ý tưởng của hàm này là : có đặt vòng while, mỗi lần vòng while chạy xong cũng có nghĩa là đã đọc xong một fileorfolder của rdetorsdet
	int i = sectorbatdau; 
	//while (i<sizemang-33) {
		BYTE* e = ReadRawEntry(i, mang); //đọc thô 1 entry
		cout << endl;

		//printEntry(e);
		//if (e[11] == 0x00) break;
		if (e[11] != 0x0F) { //entry chinh
			cout << "\nchih\n";
			//nếu như e là chính => fileorfolder không có e phụ => lưu thông tin vào fileorfolder => nhảy đến bước xử lý file or folder
			for (int t = 0; t < solantab; t++)cout << "\t";
			for (int t = 0; t < 16; t++) cout << e[t];
			cout << endl;
			for (int t = 16; t < 32; t++) cout << e[t];
		}
		 if (e[11]== 0x0F) {   //nếu e là phụ => chạy vòng while để đọc hết entry phụ(dồn vào tempname), đọc tiếp cả entry chính ngay sau đó =>lưu thông tin vào fileorfolder=>nhảy đến bước xử lý file or folder
			e = ReadRawEntry(i, mang);
			//for (int t = 0; t < solantab+1; t++) cout << "  ";
			printNameOfExtraEntry(e);

			while (true) {
				i += 32;
				BYTE* e2 = ReadRawEntry(i, mang);

				if (e2[11] == 0x0F) {
					//cout << "\ne2laphu";
					//printEntry(e2);
					//printNameOfExtraEntry(e2);
				}
				else if (e2[11] != 0x0F) {
					//cout << "\ne2lachinh";
					//in thong tin chinh o day
					//printInfoOfMainEntry(e2);
					break;
				}
			}
		//}

		/*print info here*/
		//xu ly fileorfolder
		//if (filorfolder.isfile) {//nếu là file thì chỉ đơn giản xuất ra màn hình}
	//	else if (fileorfolder.isfolder) {
			//nếu là folder thì gọi lại hàm đọc ReadRDETOrSDet, truyền vào cluster bắt đầu của sdet của folder này, solantab+1 (thụt vô dòng vì là folder con)
	//		ReadRDETOrSDet(fileorfolder.GetSectorBatDau(), solantab + 1)
	//	}


	//	}
	}

}
int main(int argc, char** argv)
{
	BYTE bootSector[512];
	FAT32 volume;
	LPCWSTR drive = L"\\\\.\\G:";
	

	ReadData(drive, 0, bootSector, 512);// ổ đĩa cần đọc, offset đọc, buffer, số byte đọc
	volume.drive = drive;//lưu tên ổ đĩa lại vào volume, nếu cần dùng đến thì vào thuộc tính drive trong struct này

	volume.read(bootSector);
	volume.print();

	cout << "\nVi tri cluster (theo sector): " << (3050 + 2 * 14859 + (2 - 2) * 16)<<endl ;

	BYTE cluster[512 * 16];
	ReadData(drive, (3050 + 2 * 14859 + (2 - 2) * 16) * 512, cluster, 512 * 16);
	for (int i = 0; i < 16; i++) cout << i << "\t";
	for (int i = 0; i < 512 * 10; i++) {
		if (i % 16 == 0) cout << endl;
		cout << cluster[i] << "\t";
		
	}
	cout << "\ntry1\n";
	ReadRDETOrSDet(128, 0, cluster, 512 * 16);
	
	//for (int i = 128; i < 128 + 32; i++) cout << cluster[i] << "\t";
	//cout << "\ntry2\n";
	//BYTE* e = ReadRawEntry(128, cluster);
	//printEntry(e);

	// 
	//cout << "\nTry1 \n";
	//ReadRDETOrSDet(0, 0, cluster,512*16);


	//Test hàm byteArray()

	// Cách dùng hàm byteArray: truyền vào vector các cluster của dữ liệu(rdet, hoặc file txt), struct FAT32
	// Để dùng được data đã đọc, cần có size của data đó -> tính như công thức dataSize
	//vector<int> a;
	//a.push_back(88206);
	//a.push_back(116020);
	//a.push_back(116021);
	//a.push_back(116022);
	//a.push_back(116023);
	//a.push_back(116024);
	//a.push_back(116025);//Đây là các cluster của 1 file text trong USB của t, t push vào vector a để text thử
	//byte* data = volume.byteArray(a, volume);
	//int dataSize = a.size() * volume.sectorsPerCluster * volume.bytesPerSector;//Vì hàm ReadtoString() cần truyền số byte cần đọc, ở đây mình cần đọc hết mảng data luôn -> size bằng số cluster(size vector) *volume.sectorsPerCluster * volume.bytesPerSector -> số byte(số phần tử) trong mảng data
	//cout << ReadtoString(data, "0", dataSize);


}
