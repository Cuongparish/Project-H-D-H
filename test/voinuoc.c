#include "syscall.h"
#include "copyright.h"

void main()
{
	char value;	// gia tri ky tu doc tu file
	int voi1 = 0; // voi 1
	int voi2 = 0; // voi 2
	bool flagRs;	// danh dau xong file result
	SpaceId  fileVN, fileRS;	// Bien id cho file

	// Xu ly voi nuoc
	while(true){
		Wait("temp");

		// Mo file result.txt de ghi voi nao su dung
		fileRS = Open("result.txt", 0);
		if(fileRS == -1){		//?
			Signal("sinhvien");
			return;
		}
		
		while(true){
			Wait("voinuoc");
			value = 0;			
			// Mo file voi nuoc .txt de doc dung tich
			fileVN = Open("voinuoc.txt", 1);
			if(fileVN == -1){
				Close(fileRS);
				Signal("sinhvien");
			
				return;
			}
		
			int thetich = 0;
			flagRs = false;
			int count = 0;
			while(true){			
				if(Read(&value, 1, fileVN)  == -2){	
					Close(fileVN);			
					break;
				}
				if(value != '-'){
					thetich = thetich * 10 + (value - 48);
					count++;
				}
				else{
					flagRs = true;
					Close(fileVN);
					break;			
				}
			
			}
			
			if(thetich != 0)
			{
				// Dung voi 1
				if(voi1 <= voi2){
					voi1 += thetich;
					Write(&thetich, count, fileRS);
					Write("1", 1, fileRS);
				}
				else{ // Dung voi 2					
					voi2 += thetich;
					Write(&thetich, count, fileRS);
					Write("2", 1, fileRS);
				}
			}
		
			if(flagRs == true){
				voi1 = voi2 = 0;
				Close(fileRS);				
				Signal("sinhvien");
				break;				
			}

			Signal("sinhvien");
		}
	}		
}
