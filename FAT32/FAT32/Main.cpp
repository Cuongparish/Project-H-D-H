#include"FAT32.h"


int main() {
    //ConfigureConsoleLayout();
    wstring driveName;
    cout << "Nhap ten o dia: ";
    wcin >> driveName;
    driveName = L"\\\\.\\" + driveName + L":";
    LPCWSTR drive = driveName.c_str();

    FAT32 fat32(drive);

    fat32.printInfoFAT32();
    int rootClus = fat32.get_Root_of_Cluster();
    fat32.getDirectory(rootClus);

    return 0;
}

