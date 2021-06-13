#include "Header.h"

using namespace std;

//Luu tru duong dan file
char filename_in[512], filename_out[512];

//Ham gioi thieu chuong trinh, thanh vien
void welcome() {
	cout << endl;
	cout << "--------------BAI TAP BMP--------------\n";
	cout << "|     Thuc hien: Nguyen Minh Tri      |\n";
	cout << "|           MSSV: 20120219            |\n";
	cout << "|        GVHD: Pham Minh Hoang        |\n";
	cout << "---------------------------------------\n";
}

//Yeu cau nhap dia chi file doc, xuat
void InputFileIn() {
	cout << "\nNhap vao dia chi doc file .bmp (VD: D:/lena_inp.bmp): ";
	cin.getline(filename_in, 512);
}

void InputFileOut() {
	cout << "\nNhap vao dia chi xuat file .bmp (VD: D:/lena_out.bmp): ";
	cin.getline(filename_out, 512);
}

//Ham xu ly convert anh 24/32 BPP sang 8 BPP
void convertBMPto8(ifstream &fi, ofstream &fo) {
	/*Khai bao cac cau truc de luu tru cac 
	cau truc trong file BMP duoc nhap */
	BmpDib Dib;
	Bmpfile8 BmpOutput;

	//Luu tru phan Dib cua file BMP
	readDib(fi, Dib);

	if (Dib.pixelSize == 24) {
		//Khai bao cau truc file BMP 24bpp de doc
		Bmpfile24 BmpInput;

		//Doc cac thanh phan trong file
		readBmpHeader(fi, BmpInput.header);
		readDib(fi, BmpInput.dib);
		readPixel24(fi, BmpInput);
		
		//Chuyen file 24bpp xuong 8bpp
		Convert24_8(BmpInput, BmpOutput);
		
		//Xuat file 8bpp theo duong dan
		writeBmp8(fo, BmpOutput);
		
		//Giai phong bo nho da cap phat
		releaseBmpPixelArray24(BmpInput.pixelarray, BmpInput.dib.imageHeight);
		releaseBmpPixelArray8(BmpOutput.pixelarray, BmpOutput.dib.imageHeight);
	}
	//Tuong tu...
	else if (Dib.pixelSize == 32) {
		Bmpfile32 BmpInput;
		readBmpHeader(fi, BmpInput.header);
		readDib(fi, BmpInput.dib);
		readPixel32(fi, BmpInput);
		
		Convert32_8(BmpInput, BmpOutput);
		
		writeBmp8(fo, BmpOutput);

		releaseBmpPixelArray32(BmpInput.pixelarray, BmpInput.dib.imageHeight);
		releaseBmpPixelArray8(BmpOutput.pixelarray, BmpOutput.dib.imageHeight);
	}
	else {
		cout << "Bits per Pixel khong phai 24bit hoac 32bit!";
	}
}

//Ham zoom anh BMP theo yeu cau
void zoomBMP(ifstream& fi, ofstream& fo, int S) {
	BmpDib Dib;
	readDib(fi, Dib);
	
	if (Dib.pixelSize == 8) {
		//Khai bao cau truc file 8bpp de doc va xuat
		Bmpfile8 BmpInput, BmpOutput;
		
		//Doc cac cau truc file 8bpp
		readBmpHeader(fi, BmpInput.header);
		readDib(fi, BmpInput.dib);
		readPixel8(fi, BmpInput);
		
		//Ham xu ly, resize anh 8bpp theo yeu cau
		resizeBmp8(BmpInput, BmpOutput, S);
		
		//Viet cau truc xuong duong dan
		writeBmp8(fo, BmpOutput);
		
		//Giai phong bo nho da cap phat
		releaseBmpPixelArray8(BmpInput.pixelarray, BmpInput.dib.imageHeight);
		releaseBmpPixelArray8(BmpOutput.pixelarray, BmpOutput.dib.imageHeight);
	}
	// Tuong tu...
	else if (Dib.pixelSize == 24) {
		Bmpfile24 BmpInput, BmpOutput;
		
		readBmpHeader(fi, BmpInput.header);
		readDib(fi, BmpInput.dib);
		readPixel24(fi, BmpInput);
		
		resizeBmp24(BmpInput, BmpOutput, S);
		
		writeBmp24(fo, BmpOutput);
		
		releaseBmpPixelArray24(BmpInput.pixelarray, BmpInput.dib.imageHeight);
		releaseBmpPixelArray24(BmpOutput.pixelarray, BmpOutput.dib.imageHeight);
	}
	else if (Dib.pixelSize == 32) {
		Bmpfile32 BmpInput, BmpOutput;
		
		readBmpHeader(fi, BmpInput.header);
		readDib(fi, BmpInput.dib);
		readPixel32(fi, BmpInput);
		
		resizeBmp32(BmpInput, BmpOutput, S);
		
		writeBmp32(fo, BmpOutput);
		
		releaseBmpPixelArray32(BmpInput.pixelarray, BmpInput.dib.imageHeight);
		releaseBmpPixelArray32(BmpOutput.pixelarray, BmpOutput.dib.imageHeight);
	}
	else {
		cout << "Bits per Pixel khong phai 8bit, 24bit hoac 32bit!";
	}
}

//Main program
int main(int argc, char* argv[])
{
	welcome();

	//Dung de chay chuong trinh console
	if (argc == 1) {
		//Menu chuong trinh
		cout << "\nMenu chuong trinh:\n";
		cout << "1. Convert anh 24/32 BPP sang anh 8 BPP.\n";
		cout << "2. Thu nho anh 8/24/32 BPP theo thong so S.\n";
		cout << "0. Ngung chuong trinh.\n";
		cout << "\nXin moi ban nhap lua chon: ";

		int choice = -1; cin >> choice;
		while (choice < 0 && choice > 2) {
			cout << "Lua chon khong hop le. Xin nhap lai.\n";
			cin >> choice;
		}
		
		if(choice == 0)
			return 0;
		
		cin.ignore();
		InputFileIn();
		InputFileOut();

		ifstream fi(filename_in, ios::binary);
		ofstream fo(filename_out, ios::binary);
		
		//Error handler
		if (fi.fail() || fo.fail()) {
			cout << "Loi doc/mo file! Vui long kiem tra.\n";
			cout << filename_in << " " << filename_out;
			return 0;
		}

		if (choice == 1) {
			convertBMPto8(fi, fo);
		}

		if (choice == 2) {
			cout << "\nNhap vao S la thong so zoom: ";
			int S; cin >> S;
			zoomBMP(fi, fo, S);
		}

		cout << "\nDa thuc hien xong yeu cau. Thoat chuong trinh...\n";
	}
	
	//Error handler - Xu ly khi nhap sai
	if (argc > 1 && argc < 4 || argc > 5) {
		cout << "Lenh Nhap Vao Khong Hop Le!\n";
		cout << "Cau truc cau lenh: 20120219.exe -conv <Input Path> <Output Path>\n";
		cout << "20120219.exe -zoom <Input Path> <Output Path> <ratio>\n";
		return 0;
	}

	//Ham xu ly argv -conv
	if (argc == 4) {
		
		//Error handler - Khu vuc xu ly loi

		//Kiem tra argv co dung yeu cau hay khong
		if (strcmp(argv[1], "-conv") != 0) {
			cout << "Lenh Nhap Vao Khong Hop Le !\n";
			cout << "Cau truc cau lenh: 20120219.exe -conv <Input Path> <Output Path>\n";
			return 0;
		}

		//Tao cac luong doc viet file BMP
		ifstream fi(argv[2], ios::binary);
		ofstream fo(argv[3], ios::binary);

		//Error handler
		if (fi.fail() || fo.fail()) {
			cout << "Loi doc/mo file! Vui long kiem tra.";
			return 0;
		}

		convertBMPto8(fi, fo);
	}

	//Ham xu ly argv - zoom
	if (argc == 5) {

		//Error handler - Khu vuc xu ly loi

		//Kiem tra argv co dung yeu cau hay khong
		if (strcmp(argv[1], "-zoom") != 0) {
			cout << "Lenh Nhap Vao Khong Hop Le !\n";
			cout << "Cau truc cau lenh: 20120219.exe -zoom <Input Path> <Output Path> S\n";
			return 0;
		}
		
		//Tao cac luong doc viet file BMP
		ifstream fi(argv[2], ios::binary);
		ofstream fo(argv[3], ios::binary);

		//Error handler
		if (fi.fail() || fo.fail()) {
			cout << "Loi doc/mo file! Vui long kiem tra.";
			return 0;
		}

		//Chuyen S tu char sang int
		int S = atoi(argv[4]);
		
		zoomBMP(fi, fo, S);
	}

	return 0;
}
