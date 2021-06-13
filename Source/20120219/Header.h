#pragma once
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>

using namespace std;

#pragma pack(1)

struct BmpHeader
{
	char signature[2];
	uint32_t fileSize;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t dataOffset;
};

struct BmpDib
{
	uint32_t dibSize;
	int32_t	 imageWidth;
	int32_t  imageHeight;
	uint16_t colorPlaneCount;
	uint16_t pixelSize;
	uint32_t compressMethod;
	uint32_t bitmapByteCount;
	int32_t  horizontalResolution;
	int32_t  verticalResolution;
	uint32_t colorCount;
	uint32_t importantColorCount;
};

struct Colormap8
{
	uint8_t blue;
	uint8_t green;
	uint8_t red;
	uint8_t reverse;
};


struct Color24
{
	uint8_t blue;
	uint8_t green;
	uint8_t red;
};

struct Color32
{
	uint8_t alpha;
	uint8_t blue;
	uint8_t green;
	uint8_t red;
};

struct Bmpfile8
{
	BmpHeader header;
	BmpDib dib;
	Colormap8 colormap[256];
	uint8_t** pixelarray;
};

struct Bmpfile24
{
	BmpHeader header;
	BmpDib dib;
	Color24** pixelarray;
};

struct Bmpfile32
{
	BmpHeader header;
	BmpDib dib;
	Color32** pixelarray;
};

void readBmpHeader(ifstream& f, BmpHeader& inp);
void readDib(ifstream& f, BmpDib& inp);

void readColorTable8(ifstream& f, Bmpfile8& inp);

void readPixel8(ifstream& f, Bmpfile8& inp);
void readPixel24(ifstream& f, Bmpfile24& inp);
void readPixel32(ifstream& f, Bmpfile32& inp);


void Convert24_8(Bmpfile24& inp, Bmpfile8& out);
void Convert32_8(Bmpfile32& inp, Bmpfile8& out);

void resizeBmp8(Bmpfile8 inp, Bmpfile8& out, int S);
void resizeBmp24(Bmpfile24 inp, Bmpfile24& out, int S);
void resizeBmp32(Bmpfile32 inp, Bmpfile32& out, int S);

void writeBmp8(ofstream& f, Bmpfile8& inp);
void writeBmp24(ofstream& f, Bmpfile24& inp);
void writeBmp32(ofstream& f, Bmpfile32& inp);


void releaseBmpPixelArray8(uint8_t** pixelarray, int rowCount);
void releaseBmpPixelArray24(Color24** pixelarray, int rowCount);
void releaseBmpPixelArray32(Color32** pixelarray, int rowCount);