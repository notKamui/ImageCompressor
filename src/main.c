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
    /*
    QuadTreeRGBA tree = allocQuadTreeRGBA(0, 0, 0, 0);

    const int width = 1024;
    const int height = 1024;

    MLV_create_window("Test", NULL, width, height);

    MLV_Image *image = MLV_load_image("terre.jpg");
    MLV_resize_image_with_proportions(image, width, height);

    MLV_draw_image(image, 0, 0);
    MLV_actualise_window();
    MLV_free_window();

    buildRGBATree(tree, image, 0, 0, width, height, 50000);
    generatePDFQuadTreeRGBA("test.dot", "test.pdf", tree);
    */

    /*
    QuadTreeBin tree = allocQuadTreeBin(0);
    QuadTreeBin node2 = allocQuadTreeBin(0);
    QuadTreeBin node3 = allocQuadTreeBin(1);
    QuadTreeBin node4 = allocQuadTreeBin(0);
    QuadTreeBin node5 = allocQuadTreeBin(0);
    QuadTreeBin node6 = allocQuadTreeBin(0);
    QuadTreeBin node7 = allocQuadTreeBin(1);
    QuadTreeBin node8 = allocQuadTreeBin(0);
    QuadTreeBin node9 = allocQuadTreeBin(0);
    QuadTreeBin node10 = allocQuadTreeBin(1);
    QuadTreeBin node11 = allocQuadTreeBin(0);
    QuadTreeBin node12 = allocQuadTreeBin(1);
    QuadTreeBin node13 = allocQuadTreeBin(0);
    QuadTreeBin node14 = allocQuadTreeBin(0);
    QuadTreeBin node15 = allocQuadTreeBin(0);
    QuadTreeBin node16 = allocQuadTreeBin(0);
    QuadTreeBin node17 = allocQuadTreeBin(0);
    QuadTreeBin node18 = allocQuadTreeBin(1);
    QuadTreeBin node19 = allocQuadTreeBin(1);
    QuadTreeBin node20 = allocQuadTreeBin(0);
    QuadTreeBin node21 = allocQuadTreeBin(1);

    tree->northWest = node2;
    tree->northEast = node3;
    tree->southEast = node4;
    tree->southWest = node5;

    node2->northWest = node6;
    node2->northEast = node7;
    node2->southEast = node8;
    node2->southWest = node9;

    node4->northWest = node10;
    node4->northEast = node11;
    node4->southEast = node12;
    node4->southWest = node13;

    node5->northWest = node14;
    node5->northEast = node15;
    node5->southEast = node16;
    node5->southWest = node17;

    node16->northWest = node18;
    node16->northEast = node19;
    node16->southEast = node20;
    node16->southWest = node21;
    */

    int width = 64;
    int height = 64;

    QuadTreeRGBA tree = allocQuadTreeRGBA(0, 0, 0, 0);
    MLV_create_window("Test", "", width, height);

    MLV_Image *image = MLV_load_image("white.png");
    MLV_resize_image(image, width, height);

    buildRGBATree(tree, image, 0, 0, width, height, 1000000);

    MLV_clear_window(MLV_COLOR_BLACK);
    drawQuadTreeRGBA(tree, 0, 0, width, height);
    MLV_update_window();
    MLV_wait_seconds(3);
    MLV_free_window();

    generatePDFQuadTreeRGBA("test.dot", "test.pdf", tree);

    FILE *file = fopen("test.txt", "w");

    int buffer = 0;
    size_t size = 0;

    writeRGBA(tree, file, &buffer, &size);
    flush(file, &buffer, &size);
    fclose(file);

    printf("%c%c%c%c%c\n", 46, 167, 114, 159, 112);

    /*
    QuadTreeBin node6 = allocQuadTreeBin(1);
    QuadTreeBin node7 = allocQuadTreeBin(1);
    QuadTreeBin node8 = allocQuadTreeBin(0);
    QuadTreeBin node9 = allocQuadTreeBin(1);

    QuadTreeBin node10 = allocQuadTreeBin(1);
    QuadTreeBin node11 = allocQuadTreeBin(1);
    QuadTreeBin node12 = allocQuadTreeBin(0);
    QuadTreeBin node13 = allocQuadTreeBin(1);

    node1->northWest = node2;
    node1->northEast = node3;
    node1->southEast = node4;
    node1->southWest = node5;

    node2->northWest = node10;
    node2->northEast = node11;
    node2->southEast = node12;
    node2->southWest = node13;

    node4->northWest = node6;
    node4->northEast = node7;
    node4->southEast = node8;
    node4->southWest = node9;

    generatePDFQuadTreeBin("bin.dot", "bin.pdf", node1);

    minTreeBinLoss(&node1, 0);
    minTreeBinNoLoss(&node1);

    generatePDFQuadTreeBin("bin2.dot", "bin2.pdf", node1);
    */

    return 0;
}
