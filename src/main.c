#include <stdio.h>

#include "../include/controler.h"
#include "../include/compressor.h"
#include "../include/visualizer.h"

int main(void)
{
        QuadTreeBin node1 = allocQuadTreeBin(0);

    QuadTreeBin node2 = allocQuadTreeBin(0);

    QuadTreeBin node3 = allocQuadTreeBin(1);

    QuadTreeBin node4 = allocQuadTreeBin(0);

    QuadTreeBin node5 = allocQuadTreeBin(0);

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

    minimizeQuadTreeBin(&node1, 0);

    generatePDFQuadTreeBin("bin2.dot", "bin2.pdf", node1);

    return 0;
}
