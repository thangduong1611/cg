#include "rgbimage.h"
#include "color.h"
#include "assert.h"
#include <stdio.h>

#define WORD  uint16_t
#define DWORD uint32_t
#define LONG  int32_t

typedef struct tagBITMAPFILEHEADER
{
    WORD    bfType;        // must be 'BM'
    DWORD   bfSize;        // size of the whole .bmp file
    WORD    bfReserved1;   // must be 0
    WORD    bfReserved2;   // must be 0
    DWORD   bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
    DWORD  biSize;            // size of the structure
    LONG   biWidth;           // image width
    LONG   biHeight;          // image height
    WORD   biPlanes;          // bitplanes
    WORD   biBitCount;         // resolution
    DWORD  biCompression;     // compression
    DWORD  biSizeImage;       // size of the image
    LONG   biXPelsPerMeter;   // pixels per meter X
    LONG   biYPelsPerMeter;   // pixels per meter Y
    DWORD  biClrUsed;         // colors used
    DWORD  biClrImportant;    // important colors
} BITMAPINFOHEADER;

RGBImage::RGBImage( unsigned int Width, unsigned int Height)
{
    m_Height = Height;
    m_Width = Width;
    m_Image = new Color[Width * Height];
}

RGBImage::~RGBImage()
{
}

void RGBImage::setPixelColor( unsigned int x, unsigned int y, const Color& c)
{
    m_Image[x + y*m_Width] = c;
}

const Color& RGBImage::getPixelColor( unsigned int x, unsigned int y) const
{
    return m_Image[x + y*m_Width];
}

unsigned int RGBImage::width() const
{
	return m_Width;
}
unsigned int RGBImage::height() const
{
    return m_Height;
}

unsigned char RGBImage::convertColorChannel( float v)
{
    if(v <= 0.0F){
        return 0;
    } else if(v >= 1.0F){
        return 255;
    } else {
        return v*255;
    }
}


bool RGBImage::saveToDisk( const char* Filename)
{
    FILE* file;
    BITMAPFILEHEADER hfile ;
    hfile.bfType = 0x4d42;
    hfile.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + this->m_Height * this->m_Width * 3;
    hfile.bfReserved1 = 0;
    hfile.bfReserved2 = 0;
    hfile.bfOffBits = 0x36;
    
    BITMAPINFOHEADER hinfo;
    hinfo.biSize = sizeof(BITMAPINFOHEADER);
    hinfo.biWidth = this->m_Width;
    hinfo.biHeight = this->m_Height;
    hinfo.biPlanes = 1;
    hinfo.biBitCount = 24;
    hinfo.biCompression = 0;
    hinfo.biSizeImage = 0;
    hinfo.biXPelsPerMeter = 0;
    hinfo.biYPelsPerMeter = 0;
    hinfo.biClrUsed = 0;
    hinfo.biClrImportant = 0;
    
    file = fopen(Filename, "wb");
    
    fwrite(&hfile.bfType, sizeof(hfile.bfType), 1, file);
    fwrite(&hfile.bfSize, sizeof(hfile.bfSize), 1, file);
    fwrite(&hfile.bfReserved2, sizeof(hfile.bfReserved2), 1, file);
    fwrite(&hfile.bfReserved1, sizeof(hfile.bfReserved1), 1, file);
    fwrite(&hfile.bfOffBits, sizeof(hfile.bfOffBits), 1, file);
    
    fwrite(&hinfo, sizeof(hinfo), 1, file);
    
    for (int y = this->m_Height - 1; y >= 0; y--) {
        for (int x = 0; x < this->m_Width; x++) {
            Color c = getPixelColor(x, y);
            unsigned char b = convertColorChannel(c.B);
            unsigned char g = convertColorChannel(c.G);
            unsigned char r = convertColorChannel(c.R);
            fwrite((unsigned char*)&b, sizeof(unsigned char), 1, file);
            fwrite((unsigned char*)&g, sizeof(unsigned char), 1, file);
            fwrite((unsigned char*)&r, sizeof(unsigned char), 1, file);
        }
    }
    fclose(file);
    return true;
}
