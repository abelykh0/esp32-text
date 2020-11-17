#include "VgaText.h"
#include "fabutils.h"
#include "fonts/font_8x14.h"

#define BACK_COLOR 0x10
#define FORE_COLOR 0x3F

void VgaText::start(char const* modeline)
{
    this->_fontWidth = FONT_8x14.width;
    this->_fontHeight = FONT_8x14.height;
    this->_fontData = (uint8_t*)FONT_8x14.data;
    
/*
    int charDataSize = 256 * this->_fontHeight * ((this->_fontWidth + 7) / 8);
    this->_fontData = (uint8_t*)heap_caps_malloc(charDataSize, MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);
    memcpy(this->_fontData, FONT_8x14.data, charDataSize);
*/

    this->Attributes = (uint32_t**)heap_caps_malloc(SCREEN_WIDTH * SCREEN_HEIGHT * 4, MALLOC_CAP_8BIT);

    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
    {
        this->Characters[i] = ' ';
        this->Attributes[i] = nullptr;
    }

    this->begin();
    this->setResolution(modeline);

    // Create a "default" attribute "white on blue"
    this->_defaultAttribute = this->CreateAttribute(FORE_COLOR, BACK_COLOR);
}

void VgaText::setAttribute(uint8_t x, uint8_t y, uint8_t foreColor, uint8_t backColor)
{
    uint32_t* attribute;
    if (foreColor == 0xFF && backColor == 0xFF)
    {
        attribute = this->_defaultAttribute;
    }
    else
    {
        uint16_t colors = foreColor << 8 | backColor;

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
    //uint32_t* result = (uint32_t*)heap_caps_malloc(16 * 4, MALLOC_CAP_8BIT);
    uint32_t* result = (uint32_t*)heap_caps_malloc(16 * 4, MALLOC_CAP_32BIT);
    uint32_t* attribute = result;
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

    return result;
}

void IRAM_ATTR VgaText::drawScanline(uint8_t* dest, int scanLine)
{
    if (_defaultAttribute == nullptr)
    {
        return;
    }

    //uint8_t color = m_HVSync | 0x3F10;
    //memset(dest, color, 800);

    int y = scanLine / this->_fontHeight;
    int fontRow = scanLine % this->_fontHeight;
    uint32_t* dest32 = (uint32_t*)dest;
    int startCoord = y * SCREEN_WIDTH;

    for (int coord = startCoord; coord < startCoord + SCREEN_WIDTH; coord++)
    {
        uint8_t fontPixels = 
            this->_fontData[(uint8_t)this->Characters[coord] * this->_fontHeight + fontRow];
        
        uint32_t* attribute = this->Attributes[coord];
        if (attribute == nullptr)
        {
            attribute = this->_defaultAttribute;
        }

        *dest32 = attribute[fontPixels >> 4];
        dest32++;
        *dest32 = attribute[fontPixels & 0x0F];
        dest32++;
    }
}