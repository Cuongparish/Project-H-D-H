#include"FAT32.h"

//Ham khoi tao FAT32
FAT32::FAT32(LPCWSTR drive) {
	this->_drive = drive;
	BYTE sector[BYTES_READ];

	if (ReadSector(_drive, POINT_READ, sector) == 0)
	{
		_bytePerSector = get_Int_From_Bytes(sector, 11, 2);
		_sectorPerCluster = get_Int_From_Bytes(sector, 13, 1);
		_rsvdSecCnt = get_Int_From_Bytes(sector, 14, 2);
		_numberOfFAT = get_Int_From_Bytes(sector, 16, 1);
		if (get_Int_From_Bytes(sector, 19, 2) == 0)
		{
			_totSec32 = get_Int_From_Bytes(sector, 32, 4);
		}
		else
		{
			_totSec32 = get_Int_From_Bytes(sector, 19, 2);

		}
		_FATSector32 = get_Int_From_Bytes(sector, 36, 4);
		_rootClus = get_Int_From_Bytes(sector, 44, 4);
	}
}

// doc sector trong FAT32
int FAT32::ReadSector(LPCWSTR drive, int readPoint, BYTE sector[]) {
	DWORD bytesRead;

	_device = CreateFile(drive,
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (_device == INVALID_HANDLE_VALUE)
	{
		printf("CreateFile: %u\n", GetLastError());
		return 1;
	}

	SetFilePointer(_device, readPoint, NULL, FILE_BEGIN);

	if (!ReadFile(_device, sector, BYTES_READ, &bytesRead, NULL))
	{
		return 1;
	}
	else {
		return 0;
	}
}

// ham in tab tao cau hinh hien thi cay thu muc
void FAT32::printTab() {
	for (int i = 0; i < _level; i++)
	{//wprintf(L"\t");
		cout << "\t";
	}
}

// chuyen doi tu byte sang int
int64_t FAT32::get_Int_From_Bytes(BYTE sector[], int offset, int size) {
	int64_t k = 0;
	memcpy(&k, sector + offset, size);
	return k;
}

// chuyen doi tu byte sang string
wstring FAT32::get_String_From_Bytes(BYTE sector[], int offset, int size, bool isShort) { // bien check co la kieu short
	wstring resuslt;
	for (int i = offset; i < offset + size - 1;) {
		int intValue = 0;
		if (isShort) {
			intValue = get_Int_From_Bytes(sector, i, 2);
			i = i + 2;
		}
		else {
			intValue = get_Int_From_Bytes(sector, i, 1);
			i++;
		}

		wchar_t current_String = static_cast<wchar_t>(intValue);
		resuslt = resuslt + current_String;
	}
	return resuslt;
}

//Lay ra cluster dau tien cua root directory
int FAT32::get_Root_of_Cluster() {
	return _rootClus;
}

//Tim sector dau tien cua cluster
int FAT32::find_First_SectorOfCluster(int cluster) {
	return (cluster - 2) * _sectorPerCluster + _rsvdSecCnt + _numberOfFAT * _FATSector32;
}

//Tim ra nhung sector cua cluster
vector<int> FAT32::getClusters(int firstCluster) {
	vector<int> fileClusters;

	BYTE sector[BYTES_READ];
	int readPoint = _rsvdSecCnt * _bytePerSector;

	SetFilePointer(_device, readPoint, NULL, FILE_BEGIN);
	ReadSector(_drive, readPoint, sector);
	readPoint += BYTES_READ;

	int clusterValue = firstCluster;
	do {
		fileClusters.push_back(clusterValue);
		clusterValue = get_Int_From_Bytes(sector, (4 * clusterValue) % 512, 4);

		if (clusterValue > readPoint / 4)
		{
			SetFilePointer(_device, readPoint, NULL, FILE_BEGIN);
			ReadSector(_drive, readPoint, sector);
			readPoint += BYTES_READ;
		}
	} while (clusterValue != STOP_CLUSTER);

	return fileClusters;
}

//Lay ra nhung sector cua nhung cluster tuong ung
vector<int> FAT32::get_Sector_Of_FileSectors(vector<int> fileClusters) {
	vector<int> fileSectors;

	for (unsigned int i = 0; i < fileClusters.size(); i++)
	{
		{int firstSector = find_First_SectorOfCluster(fileClusters[i]);

		for (int i = 0; i < _sectorPerCluster; i++)
		{
			fileSectors.push_back(firstSector + i);
		}
		}

		return fileSectors;
	}
}
//Loai bo dau cach o cuoi chuoi
wstring rtrim(const wstring & ws) {
	size_t end = ws.find_last_not_of(' ');
	return (end == wstring::npos) ? L"" : ws.substr(0, end + 1);
}

//Thong tin cua FAT32
void FAT32::printInfoFAT32() {
	//cout << "nhin gi";
	cout << "\t\t\t --------- THONG TIN TRONG BOOTSECTOR FAT32 ---------" << endl;
	cout << " So bytes/sector: " << _bytePerSector << endl;
	cout << " So sector/cluster: " << _sectorPerCluster << endl;
	cout << " So sector Reserved: " << _rsvdSecCnt << endl;
	cout << " So bang FAT: " << _numberOfFAT << endl;
	cout << " So sector cua mot bang FAT: " << _FATSector32 << endl;
	cout << " Tong sector volume: " << _totSec32 << endl;
	cout << " Dia chi sector dau tien bang FAT: " << _rsvdSecCnt << endl;
	cout << " Dia chi sector dau tien data: " << _rsvdSecCnt + _numberOfFAT * _FATSector32 << endl;
	//wprintf(L"\t\t\t --------- THONG TIN TRONG BOOTSECTOR FAT32 ---------\n");
	//wprintf(L"So bytes/sector: %d\n", _bytePerSector);
	//wprintf(L"So sector/cluster: %d\n", _sectorPerCluster);
	//wprintf(L"So sector Reserved: %d\n", _rsvdSecCnt);
	//wprintf(L"So bang FAT: %d\n", _numberOfFAT);
	//wprintf(L"So sector cua mot bang FAT: %d\n", _FATSector32);
	//wprintf(L"Tong sector volume: %d\n", _totSec32);
	//wprintf(L"Dia chi sector dau tien bang FAT1: %d\n", _rsvdSecCnt);
	//wprintf(L"Dia chi sector dau tien Data: %d\n", _rsvdSecCnt + _numberOfFAT * _FATSector32);
	//wprintf(L"\n\t\t\t --------------Cay thu muc---------------\n");
}

//Lay thong tin tap tin
void FAT32::get_File_Info(BYTE sector[], int firstCluster)
{
	vector<int> fileClusters = getClusters(firstCluster);
	vector<int> fileSectors = get_Sector_Of_FileSectors(fileClusters);
	printTab();
	//wprintf(L"+Cluster bat dau: %d\n", firstCluster);
	cout << " + Cluster bat dau: " << firstCluster << endl;
	printTab();
	//wprintf(L"+Chiem cac cluster: ");
	cout << "+ Chiem cac cluster: ";

	for (unsigned int i = 0; i < fileClusters.size(); i++)
	{
		//wprintf(L"%d ", fileClusters[i]);
		cout << fileClusters[i];
	}
	cout << endl;
	//wprintf(L"\n");
	printTab();
	//wprintf(L"+Chiem cac sector: ");
	cout << "chiem cac sector: ";
	for (unsigned int i = 0; i < fileSectors.size(); i++)
	{
		//wprintf(L"%d ", fileSectors[i]);
		cout << fileSectors[i];
	}
	//wprintf(L"\n");
	cout << endl;
}

//Lay thong tin size
void FAT32::getSize(BYTE sector[], int index) {
	int64_t fileSize = get_Int_From_Bytes(sector, index + 28, 4) * _bytePerSector;
	printTab();
	//wprintf(L"+Kich co %d Byte\n", fileSize);
	cout << "+ Kich thuoc co " << fileSize << " byte" << endl;
}

//Lay nhung tap tin con trong thu muc goc
void FAT32::getDirectory(int cluster) {
	BYTE sector[BYTES_READ];
	int readPoint = find_First_SectorOfCluster(cluster) * _bytePerSector;

	do {
		SetFilePointer(_device, readPoint, NULL, FILE_BEGIN);
		ReadSector(_drive, readPoint, sector);

		readPoint += 512;

		int index = 0;

		wstring totalEntryName;
		while (index < 512) {
			if (sector[index + 11] == 16) {

				if (sector[index] != '.') {
					if (totalEntryName.size() == 0) {
						wstring mainEntryName = get_String_From_Bytes(sector, index, 12, false);
						printTab();
						//wprintf(L"%s\n", mainEntryName.c_str());
						cout << mainEntryName.c_str() << endl;
					}
					else
					{
						printTab();
						//wprintf(L"%s\n", totalEntryName.c_str());
						cout << totalEntryName.c_str();
					}

					printTab();
					//wprintf(L"+Loai tap tin: Thu muc\n");
					cout << "+ Loai tap tin: Thu muc" << endl;

					int firstCluster = get_Int_From_Bytes(sector, index + 26, 2) +
						(int)pow(16, 2) * get_Int_From_Bytes(sector, index + 20, 2);
					get_File_Info(sector, firstCluster);

					//De quy
					_level++;
					getDirectory(firstCluster);
					_level--;
				}
			}

			if (sector[index + 11] == 32) {
				wstring fileExtension;

				if (totalEntryName.size() == 0) {
					wstring fileName = rtrim(get_String_From_Bytes(sector, index, 8, false));
					fileExtension = rtrim(get_String_From_Bytes(sector, index + 8, 4, false));

					wstring mainEntryName = (fileExtension == L"") ? fileName : fileName + L"." + fileExtension;
					printTab();
					//wprintf(L"%s\n", mainEntryName.c_str());
					cout << mainEntryName.c_str() << endl;
				}
				else {
					int dotIndex = totalEntryName.rfind(L'.');
					fileExtension = (dotIndex == wstring::npos) ? L"" :
						totalEntryName.substr(dotIndex + 1, totalEntryName.length() - dotIndex - 1);
					printTab();
					//wprintf(L"%s\n", totalEntryName.c_str());
					cout << totalEntryName.c_str() << endl;
				}

				printTab();
				//wprintf(L"+Loai tap tin: Tap tin\n");
				cout << "+ Loai tap tin: Tap tin " << endl;

				int firstCluster = get_Int_From_Bytes(sector, index + 26, 2) +
					(int)pow(16, 2) * get_Int_From_Bytes(sector, index + 20, 2);

				if (firstCluster != 0)
					get_File_Info(sector, firstCluster);

				getSize(sector, index);
				/*int fileSize = GetIntValue(sector, index + 28, 4) * bytsPerSec;
				PrintTab();
				wprintf(L"+Kich co %d Byte\n", fileSize);*/
			}

			if (sector[index + 11] == 15) {
				wstring extraEntryName =
					get_String_From_Bytes(sector, index + 1, 10, true) +
					get_String_From_Bytes(sector, index + 14, 12, true) +
					get_String_From_Bytes(sector, index + 28, 4, true);

				totalEntryName = extraEntryName + totalEntryName;
			}
			else
				totalEntryName.clear();

			index += 32;
		}
	} while (sector[0] != 0);
}
