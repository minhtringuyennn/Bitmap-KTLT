#include "Header.h"

using namespace std;

//Doc header BMP
void readBmpHeader(ifstream& f, BmpHeader& inp)
{
	//Error handler
	if (f.fail())
		return;
	//Di chuyen con tro den dau file
	f.seekg(0, f.beg);
	/*Doc tat ca bit trong phan BMP Header 
	va chuyen vao cau truc BmpHeader inp*/
	f.read((char*)&inp, sizeof(BmpHeader));
}

//Doc dib BMP
void readDib(ifstream& f, BmpDib& inp)
{
	//Error handler
	if (f.fail())
		return;
	//Di chuyen con tro den sau vi tri BmpHeader
	f.seekg(sizeof(BmpHeader), f.beg);
	/*Doc tat ca bit trong phan BMP Dib
	va chuyen vao cau truc BmpDib inp*/
	f.read((char*)&inp, sizeof(BmpDib));
}

//Ham doc bang mau 
void readColorTable8(ifstream& f, Bmpfile8& inp)
{
	//Error handler
	if (f.fail())
		return;
	//Di chuyen con tro den sau vi tri BmpHeader
	f.seekg(sizeof(BmpHeader), f.beg);
	//Di chuyen con tro den vi tri cua colormap (chi o file 8bpp)
	f.seekg(inp.dib.dibSize, f.cur);

	//Doc va ghi vao inp.colormap
	for (int i = 0; i < 256; i++)
	{
		f.read((char*)&inp.colormap[i], sizeof(Color32));
	}
}

//Ham doc pixel file 8bpp
void readPixel8(ifstream& f, Bmpfile8& inp)
{
	//Error handler
	if (f.fail())
		return;
	//Tinh padding, rieng anh 32bpp khong co padding
	char paddingCount = (4 - inp.dib.imageWidth % 4) % 4;
	//Di chuyen tro chuot den phan luu tru pixel
	f.seekg(inp.header.dataOffset, f.beg);
	//Khoi tao mang dong theo chieu cao
	inp.pixelarray = new uint8_t * [inp.dib.imageHeight];
	//Doc pixel theo tung hang
	for (int i = 0; i < inp.dib.imageHeight; i++)
	{
		/*Khoi tao pixel o cac cot, vi 8bpp nen chi 
		dung 1 byte mau; 24bpp dung 3 byte; 32bpp dung 4 byte.*/
		inp.pixelarray[i] = new uint8_t[inp.dib.imageWidth];
		
		for (int j = 0; j < inp.dib.imageWidth; j++)
			//Doc thuoc tinh pixel
			f.read((char*)&inp.pixelarray[i][j], sizeof(uint8_t));
		//Cach mot doan padding
		f.seekg(paddingCount, f.cur);
	}
}
//Tuong tu...
void readPixel24(ifstream& f, Bmpfile24& inp)
{
	if (f.fail())
		return;
	
	char paddingCount = (4 - (inp.dib.imageWidth * 3) % 4) % 4;
	
	f.seekg(inp.header.dataOffset, f.beg);
	
	inp.pixelarray = new Color24 * [inp.dib.imageHeight];
	
	for (int i = 0; i < inp.dib.imageHeight; i++)
	{
		inp.pixelarray[i] = new Color24[inp.dib.imageWidth];
		
		for (int j = 0; j < inp.dib.imageWidth; j++)
			f.read((char*)&inp.pixelarray[i][j], sizeof(Color24));
		
		f.seekg(paddingCount, f.cur);
	}
}
//Tuong tu...
void readPixel32(ifstream& f, Bmpfile32& inp)
{
	if (f.fail())
		return;
	
	f.seekg(inp.header.dataOffset, f.beg);
	
	inp.pixelarray = new Color32 * [inp.dib.imageHeight];
	
	for (int i = 0; i < inp.dib.imageHeight; i++)
	{
		inp.pixelarray[i] = new Color32[inp.dib.imageWidth];
		
		for (int j = 0; j < inp.dib.imageWidth; j++)
			f.read((char*)&inp.pixelarray[i][j], sizeof(Color32));
	}
}

void Convert24_8(Bmpfile24& inp, Bmpfile8& out)
{
	//Tam thoi gan header va dib cua file cu sang file moi
	out.header = inp.header;
	out.dib = inp.dib;
	//Tao color table cho file 8bpp
	for (int i = 0; i < 256; i++) {
		out.colormap[i].red = i;
		out.colormap[i].green = i;
		out.colormap[i].blue = i;
		out.colormap[i].reverse = 0;
	}
	//Khoi tao mang dong theo chieu cao
	out.pixelarray = new uint8_t * [inp.dib.imageHeight];
	//Doc pixel theo tung hang
	for (int i = 0; i < inp.dib.imageHeight; i++) {
		//Khoi tao pixel o cac cot, vi 8bpp nen chi dung 1 byte luu du lieu 
		out.pixelarray[i] = new uint8_t[inp.dib.imageWidth];
		for (int j = 0; j < inp.dib.imageWidth; j++) {
			//Tinh mau pixel moi
			uint8_t _gray_adjust = (inp.pixelarray[i][j].blue + inp.pixelarray[i][j].red + inp.pixelarray[i][j].green) / 3;
			//Ghi mau vao mang pixel cho file BMP moi
			out.pixelarray[i][j] = _gray_adjust;
		}
	}
}
//Tuong tu...
void Convert32_8(Bmpfile32& inp, Bmpfile8& out)
{
	out.header = inp.header;
	out.dib = inp.dib;

	for (int i = 0; i < 256; i++) {
		out.colormap[i].red = i;
		out.colormap[i].green = i;
		out.colormap[i].blue = i;
		out.colormap[i].reverse = 0;
	}

	out.pixelarray = new uint8_t * [inp.dib.imageHeight];
	for (int i = 0; i < inp.dib.imageHeight; i++) {
		out.pixelarray[i] = new uint8_t[inp.dib.imageWidth];
		for (int j = 0; j < inp.dib.imageWidth; j++) {
			uint8_t _gray_adjust = (inp.pixelarray[i][j].blue + inp.pixelarray[i][j].green + inp.pixelarray[i][j].red) / 3;
			out.pixelarray[i][j] = _gray_adjust;
		}
	}
}

/* 
Tinh ra mau trung binh dua tren so pixel tu 
hinh chu nhat tao boi top x, top y, bottom x, bottom y.

Vi cau truc cac file BMP voi BPP khac nhau se khac nhau 
nen tao ra 3 ham rieng biet de xu ly va tra ve dung
cau truc cho tung BPP.
*/

uint8_t calcAvarage8(uint8_t** pixelarray, int tx, int ty, int bx, int by) {
	//Pixel dich se tra ve
	uint8_t res;
	res = 0;

	//Bien tong
	int res_sum = 0;
	
	//Cong tat ca gia tri pixel lai va tinh trung bình
	for (int i = tx; i <= bx; i++)
		for (int j = ty; j <= by; j++) {
			res_sum += pixelarray[i][j];
		}
	res = res_sum / ((bx - tx + 1) * (by - ty + 1));
	//Tra ve pixel dich
	return res;
}
//Tuong tu...
Color24 calcAvarage24(Color24** pixelarray, int tx, int ty, int bx, int by) {
	Color24 res;
	int res_sum[3];
	res_sum[0] = res_sum[1] = res_sum[2] = 0;
	
	for (int i = tx; i <= bx; i++)
		for (int j = ty; j <= by; j++) {
			res_sum[0] += pixelarray[i][j].blue;
			res_sum[1] += pixelarray[i][j].green;
			res_sum[2] += pixelarray[i][j].red;
		}
	
	res.blue = res_sum[0] / ((bx - tx + 1) * (by - ty + 1));
	res.green = res_sum[1] / ((bx - tx + 1) * (by - ty + 1));
	res.red = res_sum[2] / ((bx - tx + 1) * (by - ty + 1));
	
	return res;
}
//Tuong tu...
Color32 calcAvarage32(Color32** pixelarray, int tx, int ty, int bx, int by) {
	Color32 res;
	int res_sum[4];
	res_sum[0] = res_sum[1] = res_sum[2] = res_sum[3] = 0;
	
	for (int i = tx; i <= bx; i++)
		for (int j = ty; j <= by; j++) {
			res_sum[0] += pixelarray[i][j].blue;
			res_sum[1] += pixelarray[i][j].green;
			res_sum[2] += pixelarray[i][j].red;
			res_sum[3] += pixelarray[i][j].alpha;
		}
	
	res.blue = res_sum[0] / ((bx - tx + 1) * (by - ty + 1));
	res.green = res_sum[1] / ((bx - tx + 1) * (by - ty + 1));
	res.red = res_sum[2] / ((bx - tx + 1) * (by - ty + 1));
	res.alpha = res_sum[3] / ((bx - tx + 1) * (by - ty + 1));
	
	return res;
}
//Ham resize anh 8bpp
void resizeBmp8(Bmpfile8 inp, Bmpfile8& out, int S) {
	// Tam thoi luu lai thong tin Header va Dib cu
	out.header = inp.header;
	out.dib = inp.dib;
	// width va height cua hinh moi
	int old_w = inp.dib.imageWidth;
	int old_h = inp.dib.imageHeight;
	int new_w = (inp.dib.imageWidth + S - 1) / S;
	int new_h = (inp.dib.imageHeight + S - 1) / S;
	//Tao color table (chi co o anh 8bpp)
	for (int i = 0; i < 256; i++) {
		out.colormap[i].red = i;
		out.colormap[i].green = i;
		out.colormap[i].blue = i;
		out.colormap[i].reverse = 0;
	}
	// Thay doi thong tin dib
	out.dib.imageWidth = new_w;
	out.dib.imageHeight = new_h;
	out.pixelarray = new uint8_t * [new_h];
	// Down sampling
	for (int i = 0; i < new_h; i++) {
		out.pixelarray[i] = new uint8_t[new_w];
		for (int j = 0; j < new_w; j++) {
			int tx = i * S, ty = j * S; // top left of current kernel
			int bx = min(tx + S, old_h) - 1, by = min(ty + S, old_w) - 1; // bottom right of current kernel
			// update new pixel value by average of old pixel
			out.pixelarray[i][j] = calcAvarage8(inp.pixelarray, tx, ty, bx, by);
		}
	}
}
//Tuong tu
void resizeBmp24(Bmpfile24 inp, Bmpfile24& out, int S) {
	out.header = inp.header;
	out.dib = inp.dib;
	// width and height of new picture
	int old_w = inp.dib.imageWidth;
	int old_h = inp.dib.imageHeight;
	int new_w = (inp.dib.imageWidth + S - 1) / S;
	int new_h = (inp.dib.imageHeight + S - 1) / S;

	// change bmp info
	out.dib.imageWidth = new_w;
	out.dib.imageHeight = new_h;
	out.pixelarray = new Color24 * [new_h];
	// down sampling
	for (int i = 0; i < new_h; i++) {
		out.pixelarray[i] = new Color24[new_w];
		for (int j = 0; j < new_w; j++) {
			int tx = i * S, ty = j * S; // top left of current kernel
			int bx = min(tx + S, old_h) - 1, by = min(ty + S, old_w) - 1; // bottom right of current kernel
			// update new pixel value by average of old pixel
			out.pixelarray[i][j] = calcAvarage24(inp.pixelarray, tx, ty, bx, by);
		}
	}
}
//Tuong tu
void resizeBmp32(Bmpfile32 inp, Bmpfile32& out, int S) {
	out.header = inp.header;
	out.dib = inp.dib;
	// width and height of new picture
	int old_w = inp.dib.imageWidth;
	int old_h = inp.dib.imageHeight;
	int new_w = (inp.dib.imageWidth + S - 1) / S;
	int new_h = (inp.dib.imageHeight + S - 1) / S;

	// change bmp info
	out.dib.imageWidth = new_w;
	out.dib.imageHeight = new_h;
	out.pixelarray = new Color32 * [new_h];

	for (int i = 0; i < new_h; i++) {
		out.pixelarray[i] = new Color32[new_w];
		for (int j = 0; j < new_w; j++) {
			int tx = i * S, ty = j * S; // top left of current kernel
			int bx = min(tx + S, old_h) - 1, by = min(ty + S, old_w) - 1; // bottom right of current kernel
			// update new pixel value by average of old pixel
			out.pixelarray[i][j] = calcAvarage32(inp.pixelarray, tx, ty, bx, by);
		}
	}
}

void writeBmp8(ofstream& f, Bmpfile8& inp)
{
	//Ghi lai tat ca thong tin dib cho file bmp moi
	inp.dib.dibSize = sizeof(inp.dib);
	inp.dib.pixelSize = 8;
	inp.dib.compressMethod = 0;
	inp.dib.horizontalResolution = 0;
	inp.dib.verticalResolution = 0;
	//Tinh padding count
	char paddingCount = (4 - inp.dib.imageWidth % 4) % 4;
	inp.dib.bitmapByteCount = inp.dib.imageHeight * (inp.dib.imageWidth + paddingCount);
	inp.header.fileSize = sizeof(BmpHeader) + sizeof(BmpDib) + inp.dib.bitmapByteCount;
	//Di chuyen con tro den dau file
	f.seekp(0, f.beg);
	//Xuat cac thanh phan cua file BMP
	f.write((char*)&inp.header, sizeof(BmpHeader));
	f.write((char*)&inp.dib, sizeof(BmpDib));
	//Xuat colortable chi danh cho file 8bpp
	f.write((char*)&inp.colormap, sizeof(inp.colormap));
	
	//Ghi pixel data vao file BMP moi
	for (int i = 0; i < inp.dib.imageHeight; i++)
	{
		for (int j = 0; j < inp.dib.imageWidth; j++)
			f.write((char*)&inp.pixelarray[i][j], sizeof(uint8_t));
		f.write("\0\0\0\0", paddingCount);
	}
}
//Tuong tu...
void writeBmp24(ofstream& f, Bmpfile24& inp)
{
	inp.dib.dibSize = sizeof(inp.dib);
	inp.dib.pixelSize = 24;
	inp.dib.compressMethod = 0;
	inp.dib.horizontalResolution = 0;
	inp.dib.verticalResolution = 0;
	char paddingCount = (4 - (inp.dib.imageWidth * 3) % 4) % 4;
	inp.dib.bitmapByteCount = inp.dib.imageHeight * (inp.dib.imageWidth * 3 + paddingCount);
	inp.header.fileSize = sizeof(BmpHeader) + sizeof(BmpDib) + inp.dib.bitmapByteCount;
	f.seekp(0, f.beg);
	f.write((char*)&inp.header, sizeof(BmpHeader));
	f.write((char*)&inp.dib, sizeof(BmpDib));
	for (int i = 0; i < inp.dib.imageHeight; i++)
	{
		for (int j = 0; j < inp.dib.imageWidth; j++)
			f.write((char*)&inp.pixelarray[i][j], sizeof(Color24));
		f.write("\0\0\0\0", paddingCount);
	}
}
//Tuong tu...
void writeBmp32(ofstream& f, Bmpfile32& inp)
{
	inp.dib.dibSize = sizeof(inp.dib);
	inp.dib.pixelSize = 32;
	inp.dib.compressMethod = 0;
	inp.dib.horizontalResolution = 0;
	inp.dib.verticalResolution = 0;
	inp.dib.bitmapByteCount = inp.dib.imageHeight * inp.dib.imageWidth * 4;
	inp.header.fileSize = sizeof(BmpHeader) + sizeof(BmpDib) + inp.dib.bitmapByteCount;
	f.seekp(0, f.beg);
	f.write((char*)&inp.header, sizeof(BmpHeader));
	f.write((char*)&inp.dib, sizeof(BmpDib));
	for (int i = 0; i < inp.dib.imageHeight; i++)
	{
		for (int j = 0; j < inp.dib.imageWidth; j++)
			f.write((char*)&inp.pixelarray[i][j], sizeof(Color32));
	}
}

//Giai phong bo nho cho mang dong
void releaseBmpPixelArray8(uint8_t** pixelarray, int rowCount)
{
	for (int i = 0; i < rowCount; i++)
		delete[]pixelarray[i];

	delete[]pixelarray;
}
//Tuong tu
void releaseBmpPixelArray24(Color24** pixelarray, int rowCount)
{
	for (int i = 0; i < rowCount; i++)
		delete[]pixelarray[i];

	delete[]pixelarray;
}
//Tuong tu
void releaseBmpPixelArray32(Color32** pixelarray, int rowCount)
{
	for (int i = 0; i < rowCount; i++)
		delete[]pixelarray[i];

	delete[]pixelarray;
}