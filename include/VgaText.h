#ifndef __VGATEXT__
#define __VGATEXT__

#include <memory>
#include <map>
#include "fabgl.h"

using namespace fabgl;
using namespace std;

#define RESOLUTION SVGA_800x600_60Hz
#define SCREEN_WIDTH  100
#define SCREEN_HEIGHT 43

class VgaText : public VGADirectController
{
public:
    char Characters[SCREEN_WIDTH * SCREEN_HEIGHT];
    uint32_t** Attributes;

    void start(char const * modeline);

    void setCursorPosition(uint8_t x, uint8_t y);
    void print(char* str);
    void print(char* str, uint8_t foreColor, uint8_t backColor);
    void print(const char* str);
    void print(const char* str, uint8_t foreColor, uint8_t backColor);

    void setAttribute(uint8_t x, uint8_t y, uint8_t foreColor, uint8_t backColor);
    void freeUnusedAttributes();

    void IRAM_ATTR drawScanline(uint8_t* dest, int scanLine);

private:
    int _fontHeight;
    int _fontWidth;
    uint8_t* _fontData;

    uint32_t* _defaultAttribute = nullptr;
    std::map<uint16_t, uint32_t*> _attrToAddr;
    std::map<uint32_t*, uint16_t> _addrToAttr;

    uint16_t cursor_x = 0;
    uint16_t cursor_y = 0;

    uint32_t* CreateAttribute(uint8_t foreColor, uint8_t backColor);
    void printChar(uint16_t x, uint16_t y, uint8_t ch);
    void printChar(uint16_t x, uint16_t y, uint8_t ch, uint8_t foreColor, uint8_t backColor);
    void cursorNext();
};

#endif