#include"FAT32.h"

////Cau hinh cua so Console
//void ConfigureConsoleLayout() {
//    CONSOLE_FONT_INFOEX cfi;
//    cfi.cbSize = sizeof cfi;
//    cfi.nFont = 0;
//    cfi.dwFontSize.X = 0;
//    cfi.dwFontSize.Y = 16;
//    cfi.FontFamily = FF_DONTCARE;
//    cfi.FontWeight = FW_NORMAL;
//
//    wcscpy(cfi.FaceName, L"Consolas");
//    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
//
//    int value = _setmode(_fileno(stdout), 0x00020000);
//}

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

