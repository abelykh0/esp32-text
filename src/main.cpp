#include "wifi.h"
#include "arduino.h"
#include "dispdrivers/vgabasecontroller.h"
#include "VgaText.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

static VgaText _vgaText;

void setup()
{
    Serial.begin(115200); 
    Serial.write("in setup()\r\n");

    char buf[50];

    uint32_t freeHeapBeforeVga = heap_caps_get_free_size(MALLOC_CAP_8BIT);

    _vgaText.start(RESOLUTION);

    _vgaText.setCursorPosition(60, 10);
    sprintf(buf, "Free heap before VGA: %d", freeHeapBeforeVga);
    _vgaText.print(buf);

    _vgaText.setCursorPosition(60, 11);
    sprintf(buf, "Free heap after VGA: %d", heap_caps_get_free_size(MALLOC_CAP_8BIT));
    _vgaText.print(buf);

/*
    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            _vgaText.Characters[y * SCREEN_WIDTH + x] = '0' + x % 10;
        }
    }

    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        _vgaText.setAttribute(x, 2, 0x3F, 0x00);
    }
*/
    // Display frame
    _vgaText.printChar(0, 0, '\xC9'); // ╔
    _vgaText.printChar(SCREEN_WIDTH - 1, 0, '\xBB'); // ╗
    _vgaText.printChar(0, SCREEN_HEIGHT - 1, '\xC8'); // ╚
    _vgaText.printChar(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, '\xBC'); // ╝
    for (int i = 1; i < SCREEN_WIDTH - 1; i++)
    {
    	_vgaText.printChar(i, 0, '\x0CD'); // ═
    	_vgaText.printChar(i, SCREEN_HEIGHT - 1, '\x0CD'); // ═
    }
    for (int i = 1; i < SCREEN_HEIGHT - 1; i++)
    {
    	_vgaText.printChar(0, i, '\x0BA'); // ║
    	_vgaText.printChar(SCREEN_WIDTH - 1, i, '\x0BA'); // ║
    }

    for (int i = 0; i < 64; i++)
    {
    	sprintf(buf, BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(i));

    	_vgaText.setCursorPosition(4 + (i % 4) * 7, 4 + (i / 4) * 2);
    	_vgaText.print("      ", i, i);

    	_vgaText.setCursorPosition(4 + (i % 4) * 7, 3 + (i / 4) * 2);
    	_vgaText.print(buf);
    }

    _vgaText.setCursorPosition(60, 12);
    sprintf(buf, "Free heap after 64 colors: %d", heap_caps_get_free_size(MALLOC_CAP_8BIT));
    _vgaText.print(buf);
}
 
void loop()
{
}
