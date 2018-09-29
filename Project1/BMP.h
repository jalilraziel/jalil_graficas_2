#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct  MybmpFileHeader
{
	uint32_t size;
	uint16_t resv1;
	uint16_t resv2;
	uint32_t offset;
} bmpFileHeader;

struct MybmpInfoHeader
{
	uint32_t headersize;
	uint32_t width;
	uint32_t height;
	uint16_t planes;
	uint16_t bpp;
	uint32_t compress;
	uint32_t imgsize;
	uint32_t bpmx;
	uint32_t bpmy;
	uint32_t colors;
	uint32_t imxtcolors;
} bmpInfoHeader;

char *LoadBMP(char , MybmpInfoHeader*);

