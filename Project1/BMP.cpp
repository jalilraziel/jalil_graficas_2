#include "BMP.h"

char *LoadBMP(char *filename, MybmpInfoHeader *bInfoHeader)
{

	FILE *f;
	MybmpFileHeader header;
	char *imgdata;
	uint16_t type;

	fopen_s(&f, filename, "r");
	if (!f) {
		return NULL;
	}
	fread(&type, sizeof(uint16_t), 1, f);
	if (type != 0x4D42){
		fclose(f);
		return NULL;
	}
	fread(&header, sizeof(bmpFileHeader), 1, f);

	fread(bInfoHeader, sizeof(bmpInfoHeader), 1, f);

	imgdata = (char*)malloc(bInfoHeader->imgsize);

	fseek(f, header.offset, SEEK_SET);

	fread(imgdata, bInfoHeader->imgsize, 1, f);

	fclose(f);
	return imgdata;
}