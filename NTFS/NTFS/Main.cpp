#include "Header.h"

int main(int argc, char** argv)
{
	// xử lí nhập tên ổ đĩa
	wstring disk_name;
	cout << "Input your disk name : ";
	wcin >> disk_name;
	disk_name = L"\\\\.\\" + disk_name + L":";
	LPCWSTR drive = disk_name.c_str();

	// xử lí đọc
	BYTE* sector = new BYTE[512];
	int flag = ReadSector(drive, 0, sector);

	// nếu đọc thành công
	if (flag == 1)
	{

		int control = 0;
		do
		{
			system("cls");
			cout << "------------MENU------------" << endl;
			cout << " 1. View the information " << endl;
			cout << " 2. Display directory tree " << endl;
			cout << " 3. Exit" << endl;
			int Choose = 0;
			cout << "Choose : ";
			do
			{
				cin >> Choose;
				if (cin.fail())
				{
					Choose = 0;
					cin.clear();
					cin.ignore();
				}
			} while (Choose < 1 || Choose>9);
			//1. View the information
			if (Choose == 1)
			{
				Read_BPB(sector, drive);
				system("pause");
			}
			//2. Display directory tree
			if (Choose == 2)
			{
				Read_Tree(sector, drive);
				system("pause");
			}

			//3. Exit
			if (Choose == 3)
			{
				char check = ' ';
				cout << "Do you want to exit the program? (y/n) : ";
				cin >> check;
				if (check == 'y' || check == 'Y')
				{
					control = 1;
				}
				else
				{
					control = 0;
				}
			}
		} while (control == 0);
		// giải phóng bộ nhớ
		delete[] sector;

	}
	else
	{
		cout << " Reading failed !!!!!!" << endl;
		return 0;
	}
}