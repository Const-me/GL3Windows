#pragma once
#include <stdint.h>

enum BC4Mode
{
	BC4_UNORM = 0,
	BC4_SNORM = 1
};

enum BC5Mode
{
	BC5_UNORM = 0,
	BC5_SNORM = 1
};

/*
Decompresses one block of a BC1 (DXT1) texture and stores the resulting pixels at the appropriate offset in 'image'.

uint32_t x:						x-coordinate of the first pixel in the block.
uint32_t y:						y-coordinate of the first pixel in the block.
uint32_t stride:				stride of a scanline in bytes.
const uint8_t* blockStorage:	pointer to the block to decompress.
uint32_t* image:				pointer to image where the decompressed pixel data should be stored.
*/
void DecompressBlockBC1( uint32_t x, uint32_t y, uint32_t stride, const uint8_t* blockStorage, unsigned char* image );

/*
Decompresses one block of a BC3 (DXT5) texture and stores the resulting pixels at the appropriate offset in 'image'.

uint32_t x:						x-coordinate of the first pixel in the block.
uint32_t y:						y-coordinate of the first pixel in the block.
uint32_t stride:				stride of a scanline in bytes.
const uint8_t *blockStorage:	pointer to the block to decompress.
uint32_t *image:				pointer to image where the decompressed pixel data should be stored.
*/
void DecompressBlockBC3( uint32_t x, uint32_t y, uint32_t stride, const uint8_t* blockStorage, unsigned char* image );

/*
Decompresses one block of a BC2 (DXT3) texture and stores the resulting pixels at the appropriate offset in 'image'.

uint32_t x:						x-coordinate of the first pixel in the block.
uint32_t y:						y-coordinate of the first pixel in the block.
uint32_t stride:				stride of a scanline in bytes.
const uint8_t *blockStorage:	pointer to the block to decompress.
uint32_t *image:				pointer to image where the decompressed pixel data should be stored.
*/
void DecompressBlockBC2( uint32_t x, uint32_t y, uint32_t stride, const uint8_t* blockStorage, unsigned char* image );

/*
Decompresses one block of a BC4 texture and stores the resulting pixels at the appropriate offset in 'image'.

uint32_t x:						x-coordinate of the first pixel in the block.
uint32_t y:						y-coordinate of the first pixel in the block.
uint32_t stride:				stride of a scanline in bytes.
const uint8_t* blockStorage:	pointer to the block to decompress.
float* image:					pointer to image where the decompressed pixel data should be stored.
*/
void DecompressBlockBC4( uint32_t x, uint32_t y, uint32_t stride, enum BC4Mode mode, const uint8_t* blockStorage, unsigned char* image );

/*
Decompresses one block of a BC5 texture and stores the resulting pixels at the appropriate offset in 'image'.

uint32_t x:						x-coordinate of the first pixel in the block.
uint32_t y:						y-coordinate of the first pixel in the block.
uint32_t stride:				stride of a scanline in bytes.
const uint8_t* blockStorage:	pointer to the block to decompress.
float* image:					pointer to image where the decompressed pixel data should be stored.
*/
void DecompressBlockBC5( uint32_t x, uint32_t y, uint32_t stride, enum BC5Mode mode, const uint8_t* blockStorage, unsigned char* image );