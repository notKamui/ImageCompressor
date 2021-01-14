#include <stdio.h>
<<<<<<< Updated upstream

#include "../include/controler.h"

int main(void)
{
    launchApp();
=======
#include <stdlib.h>
#include <time.h>
#include "../include/quadtree.h"
#include "../include/visualizer.h"
#include "../include/writer.h"
#include "../include/gui.h"
#include "../include/image.h"
#include "../include/parser.h"
#include "../include/compressor.h"

int main(int argc, char *argv[])
{
    /*
    FILE *file;
    int i;

    unsigned char buffer = 0;
    size_t bufferSize = 0;
    QuadTreeBinBuffer treeBuffer = allocQuadTreeBinBuffer();
    size_t treeSize = 0;

    QuadTreeBin tree = allocQuadTreeBin(0);
    QuadTreeBin node2 = allocQuadTreeBin(0);
    QuadTreeBin node3 = allocQuadTreeBin(1);
    QuadTreeBin node4 = allocQuadTreeBin(0);
    QuadTreeBin node5 = allocQuadTreeBin(1);

    tree->northEast = tree->northWest = tree->southEast = tree->southWest = node2;
    node2->northEast = node2->northWest = node2->southEast = node2->southWest = node3;
    node3->northEast = node3->southWest = node5;
    node3->northWest = node3->southEast = node4;


    MLV_create_window("Test", NULL, 1000, 1000);
    drawQuadTreeBin(tree, 0, 0, 1000, 1000);
    MLV_update_window();
    MLV_wait_mouse(NULL, NULL);
    MLV_free_window();

    generatePDFQuadTreeBin("t.dot", "t.pdf", tree);


    treeBuffer = allocQuadTreeBinBuffer();
    freeQuadTreeBin(tree, &treeBuffer);
    free(treeBuffer->buffer);
    free(treeBuffer);
    */

    /*
    QuadTreeRGBABuffer treeBuffer = allocQuadTreeRGBABuffer();
    QuadTreeRGBA tree = allocQuadTreeRGBA(0, 0, 0, 0);

    FILE *file = fopen("test.qtc", "r");

    parseMinimizedRGBA(file, 0, tree, &treeBuffer);

    generatePDFQuadTreeRGBA("parse.dot", "parse.pdf", tree);
    fclose(file);

*/

    QuadTreeRGBABuffer buffer;
    QuadTreeRGBA tree = allocQuadTreeRGBA(0, 0, 0, 0);
    time_t end, start;
    FILE *file = fopen("earth.gmc", "r");

    MLV_create_window("Visionneuse d'arbre quartique", NULL, 512, 512);

    /*
    buffer = allocQuadTreeRGBABuffer();
    freeQuadTreeRGBA(tree, &buffer);
    free(buffer->buffer);
    free(buffer);*/

    printf("Parsing... ");
    fflush(stdout);
    start = time(NULL);
    parseMinimizedRGBA(file, tree);
    end = time(NULL);
    fclose(file);
    printf("OK! (%ld seconds elapsed)\n", end - start);

    drawQuadTreeRGBA(tree, 0, 0, 512, 512);
    MLV_update_window();
    MLV_wait_mouse(NULL, NULL);

    file = fopen("earth.gmc2", "w");
    printf("Writing... ");
    fflush(stdout);
    start = time(NULL);
    writeMinimizedRGBA2(tree, file);
    end = time(NULL);
    fclose(file);
    printf("OK! (%ld seconds elapsed)\n", end - start);
    MLV_free_window();

    buffer = allocQuadTreeRGBABuffer();
    freeQuadTreeRGBA(tree, &buffer);
    freeRGBABuffer(buffer);

>>>>>>> Stashed changes
    return 0;
}
