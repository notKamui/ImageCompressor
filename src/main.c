#include <stdio.h>
#include "../include/quadtree.h"
#include "../include/visualizer.h"
#include "../include/writer.h"
#include "../include/gui.h"
#include "../include/image.h"
#include "../include/compressor.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)       \
        (byte & 0x80 ? '1' : '0'), \
        (byte & 0x40 ? '1' : '0'), \
        (byte & 0x20 ? '1' : '0'), \
        (byte & 0x10 ? '1' : '0'), \
        (byte & 0x08 ? '1' : '0'), \
        (byte & 0x04 ? '1' : '0'), \
        (byte & 0x02 ? '1' : '0'), \
        (byte & 0x01 ? '1' : '0')

int main(void)
{
    MLV_create_window("Super image compressor 2000", "", WND_WIDTH, WND_HEIGHT);

    launchApp();

    MLV_free_window();
    return 0;
}
