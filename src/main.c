#include <stdio.h>
#include "../include/quadtree.h"
#include "../include/visualizer.h"
#include "../include/writer.h"
#include "../include/gui.h"
#include "../include/image.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)       \
    (byte & 0x80 ? '1' : '0'),     \
        (byte & 0x40 ? '1' : '0'), \
        (byte & 0x20 ? '1' : '0'), \
        (byte & 0x10 ? '1' : '0'), \
        (byte & 0x08 ? '1' : '0'), \
        (byte & 0x04 ? '1' : '0'), \
        (byte & 0x02 ? '1' : '0'), \
        (byte & 0x01 ? '1' : '0')

int main(void)
{
    QuadTreeBin node1 = allocQuadTreeBin(0);
    QuadTreeBin node2 = allocQuadTreeBin(0);
    QuadTreeBin node3 = allocQuadTreeBin(0);
    QuadTreeBin node4 = allocQuadTreeBin(0);
    QuadTreeBin node5 = allocQuadTreeBin(1);

    node1->northWest = node2;
    node1->northEast = node2;
    node1->southEast = node2;
    node1->southWest = node2;

    node2->northWest = node3;
    node2->northEast = node3;
    node2->southEast = node3;
    node2->southWest = node3;

    node3->northWest = node4;
    node3->northEast = node5;
    node3->southEast = node4;
    node3->southWest = node5;

    /*FILE *file = fopen("test.qtn", "wb");*/

    const int width = 750;
    const int height = 750;

    MLV_create_window("Test", NULL, width, height);

    MLV_Image *image = MLV_load_image("terre.jpg");
    MLV_resize_image_with_proportions(image, width, height);
    MLV_Color average = averageColor(image, 0, 0, width, height);

    MLV_draw_image(image, 0, 0);
    MLV_actualise_window();
    MLV_wait_seconds(3);

    MLV_clear_window(average);
    MLV_actualise_window();

    printf("error=%d\n", colorError(image, average, 0, 0, width, height));

    MLV_wait_seconds(5);

    MLV_free_window();
    /*writeBin(node1, file);
    fclose(file);*/
    return 0;
}
