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
    uint32_t* Attributes[SCREEN_WIDTH * SCREEN_HEIGHT];

    void start(char const * modeline);

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

    uint32_t* CreateAttribute(uint8_t foreColor, uint8_t backColor);
};

#endif