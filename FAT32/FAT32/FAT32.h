#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <algorithm>
#include <math.h>
#include <string>
#include <vector>
#include<iostream>
using namespace std;

#define POINT_READ 0
#define BYTES_READ 512
#define STOP_CLUSTER 268435455
//#pragma warning (disable: 4996)

class FAT32
{
private:
	LPCWSTR _drive;
	HANDLE _device;

	int _bytePerSector; // so byte/sector
	int _sectorPerCluster; // so sector/cluster
	int _rsvdSecCnt; // so sector truoc bang FAT
	int _numberOfFAT; // so bang FAT
	int _totSec32; // so sector trong volumn
	int _FATSector32; // so sector trong 1 bang FAT
	int _rootClus; // chi so cluster dau tien cua RDET

private:
	int _level = 0; // dung xac dinh cap do cua tap tin/ thu muc

public:
	FAT32();
	FAT32(LPCWSTR drive);

	// doc sector
	int ReadSector(LPCWSTR _drive, int readPoint, BYTE sector[512]);

	// chuyen tu byte sang int
	int64_t get_Int_From_Bytes(BYTE sector[], int offset, int size);
	// chuyen tu bytes sang string
	wstring get_String_From_Bytes(BYTE sector[], int offset, int size, bool isShort);
	// tim sector dau tien cua cluster
	int find_First_SectorOfCluster(int cluster);
	// lay thong tin cua tap tin
	void get_File_Info(BYTE sector[], int firstCluster);
	// lay ra file cluster
	vector<int> getClusters(int firstCluster);
	// lay ra nhung sector cua cluster tuong ung
	vector<int> get_Sector_Of_FileSectors(vector<int> fileClusters);
	// cau hinh console cac thu muc/ tap tin con
	void printTab();
	// in thong tin FAT32
	void printInfoFAT32();
	// lay thong tin cua cay thu muc
	void getDirectory(int readPoint);
	// lay cluster dau tien cua cay thu muc
	int get_Root_of_Cluster();

	void ReadData(wstring fileExtension, int firstCluster);
};

