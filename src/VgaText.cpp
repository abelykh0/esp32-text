#include "VgaText.h"
#include "fabutils.h"
#include "fonts/font_8x14.h"

#define BACK_COLOR 0x10
#define FORE_COLOR 0x3F

extern "C" void IRAM_ATTR drawScanline(void* arg, uint8_t* dest, int scanLine);

void VgaText::InitAttribute(uint32_t* attribute, uint8_t foreColor, uint8_t backColor)
{
	for (uint8_t i = 0; i < 16; i++)
	{
		uint8_t value = i;
        uint32_t attributeValue;
		for (uint8_t bit = 0; bit < 4; bit++)
		{
            VGA_PIXELINROW(((uint8_t*)&attributeValue), bit) = 
                (value & 0x08 ?  foreColor : backColor) | this->m_HVSync;
			value <<= 1;
		}
        
        *attribute = attributeValue;
        attribute++;
	}
}

void VgaText::start(char const* modeline)
{
    this->Characters = (uint32_t*)heap_caps_malloc(SCREEN_WIDTH * SCREEN_HEIGHT * 4, MALLOC_CAP_32BIT);

    FontInfo font = FONT_8x14;
    this->_fontWidth = font.width;
    this->_fontHeight = font.height;
    this->_fontData = (uint8_t*)font.data;
    
/*
    int charDataSize = 256 * this->_fontHeight * ((this->_fontWidth + 7) / 8);
    this->_fontData = (uint8_t*)heap_caps_malloc(charDataSize, MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);
    memcpy(this->_fontData, FONT_8x14.data, charDataSize);
*/

    // "default" attribute (white on blue)
    this->_defaultAttribute = (uint32_t*)heap_caps_malloc(16 * 4, MALLOC_CAP_32BIT);

    this->Attributes = (uint32_t**)heap_caps_malloc(SCREEN_WIDTH * SCREEN_HEIGHT * 4, MALLOC_CAP_32BIT);

    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
    {
        this->Characters[i] = ' ';
        this->Attributes[i] = this->_defaultAttribute;
    }

    this->setDrawScanlineCallback(drawScanline, this);

    this->begin();
    this->setResolution(modeline);

    this->InitAttribute(this->_defaultAttribute, FORE_COLOR, BACK_COLOR);
}

void VgaText::setAttribute(uint8_t x, uint8_t y, uint8_t foreColor, uint8_t backColor)
{
    uint32_t* attribute;
    uint16_t colors = foreColor << 8 | backColor;

    if (colors == 0xFFFF)
    {
        attribute = this->_defaultAttribute;
    }
    else
    {

        auto iterator = this->_attrToAddr.find(colors);
        if (iterator == this->_attrToAddr.end())
        {
            // Missing
            attribute = this->CreateAttribute(foreColor, backColor);
            this->_attrToAddr.insert(make_pair(colors, attribute));
        }
        else
        {
            attribute = iterator->second;
        }
    }

    this->Attributes[y * SCREEN_WIDTH + x] = attribute;
}

void VgaText::printChar(uint16_t x, uint16_t y, uint8_t ch)
{
	VgaText::printChar(x, y, ch, 0xFF, 0xFF);
}
void VgaText::printChar(uint16_t x, uint16_t y, uint8_t ch, uint8_t foreColor, uint8_t backColor)
{
	if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
	{
		// Invalid
		return;
	}

	int offset = y * SCREEN_WIDTH + x;
	this->Characters[offset] = ch;
    this->setAttribute(x, y, foreColor, backColor);
}

void VgaText::print(const char* str)
{
	print((char*)str);
}
void VgaText::print(const char* str, uint8_t foreColor, uint8_t backColor)
{
	print((char*)str, foreColor, backColor);
}

void VgaText::print(char* str)
{
	print(str, 0xFF, 0xFF);
}
void VgaText::print(char* str, uint8_t foreColor, uint8_t backColor)
{
    while (*str)
    {
    	printChar(cursor_x, cursor_y, *str++, foreColor, backColor);
    	cursorNext();
    }
}

void VgaText::cursorNext()
{
    uint8_t x = cursor_x;
    uint8_t y = cursor_y;
    if (x < SCREEN_WIDTH - 1)
    {
        x++;
    }
    else
    {
        if (y < SCREEN_HEIGHT - 1)
        {
            x = 0;
            y++;
        }
    }

    setCursorPosition(x, y);
}

void VgaText::setCursorPosition(uint8_t x, uint8_t y)
{
	cursor_x = x;
	cursor_y = y;
	if (cursor_x >= SCREEN_WIDTH)
	{
		cursor_x = SCREEN_WIDTH - 1;
	}
	if (cursor_y >= SCREEN_HEIGHT)
	{
		cursor_y = SCREEN_HEIGHT - 1;
	}
}

void VgaText::freeUnusedAttributes()
{
    for (auto it = this->_attrToAddr.begin(); it != this->_attrToAddr.end(); it++)
    {
        uint32_t* attribute = it->second;

        bool found = false;
        for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
        {
            if (this->Attributes[i] == attribute)
            {
                found = true;
                break;
            }
        }

        if (!found)
        {   
            free(attribute);
            this->_attrToAddr.erase(it->first);
        }
    }   
}

uint32_t* VgaText::CreateAttribute(uint8_t foreColor, uint8_t backColor)
{
    uint32_t* attribute = (uint32_t*)heap_caps_malloc(16 * 4, MALLOC_CAP_32BIT);
    this->InitAttribute(attribute, foreColor, backColor);
    return attribute;
}

void IRAM_ATTR drawScanline(void* arg, uint8_t* dest, int scanLine)
{
    auto controller = static_cast<VgaText*>(arg);

    int fontHeight = controller->_fontHeight;
    int y = scanLine / fontHeight;
    int fontRow = scanLine % fontHeight;
    int startCoord = y * SCREEN_WIDTH;

    uint32_t* characters = (uint32_t*)(controller->Characters + startCoord);
    uint32_t** attributes = controller->Attributes + startCoord;
    uint32_t* dest32 = (uint32_t*)dest;
    uint32_t** lastAttribute = attributes + SCREEN_WIDTH - 1;
    uint8_t* fontData = controller->_fontData + fontRow;
    uint32_t character;
    uint32_t* attribute;
    uint8_t fontPixels;

    do
    {
        character = *characters;
        fontPixels = fontData[character * fontHeight];
        attribute = *attributes;
        dest32[0] = attribute[fontPixels >> 4];
        dest32[1] = attribute[fontPixels & 0x0F];

        dest32 += 2;
        attributes++;
        characters++;
    } while (attributes <= lastAttribute);
}
