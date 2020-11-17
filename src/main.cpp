#include "wifi.h"
#include "arduino.h"
#include "dispdrivers/vgabasecontroller.h"
#include "VgaText.h"

static VgaText _vgaText;

void setup()
{
    Serial.begin(115200); 
    Serial.write("in setup()\r\n");

    _vgaText.start(RESOLUTION);
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
    char buf[200];
    sprintf(buf, "%d", heap_caps_get_free_size(MALLOC_CAP_8BIT));
    _vgaText.setCursorPosition(10, 10);
    _vgaText.print(buf);
}
 
void loop()
{
}