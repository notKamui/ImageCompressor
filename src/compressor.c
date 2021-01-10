/* Created by Jimmy on 2/11/2020. */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../include/compressor.h"
#include "../include/quadtree.h"

int distRGBA(QuadTreeRGBA node1, QuadTreeRGBA node2)
{
    return sqrt(
        (node1->r - node2->r) * (node1->r - node2->r) +
        (node1->g - node2->g) * (node1->g - node2->g) +
        (node1->b - node2->b) * (node1->b - node2->b) +
        (node1->a - node2->a) * (node1->a - node2->a));
}

int distBin(QuadTreeBin node1, QuadTreeBin node2)
{
    return sqrt((node1->b - node2->b) * (node1->b - node2->b));
}

QuadTreeRGBA avgRGBA(QuadTreeRGBA nodes[], int size)
{
    int rz, bz, gz;
    float az;
    int i;

    rz = bz = gz = 0;
    az = .0;

    for (i = 0; i < size; i++)
    {
        rz += nodes[i]->r;
        bz += nodes[i]->g;
        gz += nodes[i]->b;
        az += nodes[i]->a;
    }

    return allocQuadTreeRGBA(rz / size, bz / size, gz / size, az / size);
}

QuadTreeBin avgBin(QuadTreeBin nodes[], int size)
{
    int bz;
    int i;

    bz = 0;

    for (i = 0; i < size; i++)
    {
        bz += nodes[i]->b;
    }

    return allocQuadTreeBin(bz / size);
}

int distTreeRGBA(QuadTreeRGBA tree1, QuadTreeRGBA tree2)
{
    if (isLeafRGBA(tree1) && isLeafRGBA(tree2))
    {
        return distRGBA(tree1, tree2);
    }
    if (isLeafRGBA(tree1) && !isLeafRGBA(tree2))
    {
        return distTreeRGBA(tree1, tree2->northWest) / 4 +
               distTreeRGBA(tree1, tree2->northEast) / 4 +
               distTreeRGBA(tree1, tree2->southWest) / 4 +
               distTreeRGBA(tree1, tree2->southEast) / 4;
    }
    if (!isLeafRGBA(tree1) && isLeafRGBA(tree2))
    {
        return distTreeRGBA(tree2, tree1->northWest) / 4 +
               distTreeRGBA(tree2, tree1->northEast) / 4 +
               distTreeRGBA(tree2, tree1->southWest) / 4 +
               distTreeRGBA(tree2, tree1->southEast) / 4;
    }
    if (isLeafRGBA(tree1) && !isLeafRGBA(tree2))
    {
        return distTreeRGBA(tree1->northWest, tree2->northWest) / 4 +
               distTreeRGBA(tree1->northEast, tree2->northEast) / 4 +
               distTreeRGBA(tree1->southWest, tree2->southWest) / 4 +
               distTreeRGBA(tree1->southEast, tree2->southEast) / 4;
    }
    return 0;
}

int distTreeBin(QuadTreeBin tree1, QuadTreeBin tree2)
{
    if (isLeafBin(tree1) && isLeafBin(tree2))
    {
        return distBin(tree1, tree2);
    }
    if (isLeafBin(tree1) && !isLeafBin(tree2))
    {
        return distTreeBin(tree1, tree2->northWest) / 4 +
               distTreeBin(tree1, tree2->northEast) / 4 +
               distTreeBin(tree1, tree2->southWest) / 4 +
               distTreeBin(tree1, tree2->southEast) / 4;
    }
    if (!isLeafBin(tree1) && isLeafBin(tree2))
    {
        return distTreeBin(tree2, tree1->northWest) / 4 +
               distTreeBin(tree2, tree1->northEast) / 4 +
               distTreeBin(tree2, tree1->southWest) / 4 +
               distTreeBin(tree2, tree1->southEast) / 4;
    }
    if (isLeafBin(tree1) && !isLeafBin(tree2))
    {
        return distTreeBin(tree1->northWest, tree2->northWest) / 4 +
               distTreeBin(tree1->northEast, tree2->northEast) / 4 +
               distTreeBin(tree1->southWest, tree2->southWest) / 4 +
               distTreeBin(tree1->southEast, tree2->southEast) / 4;
    }
    return 0;
}