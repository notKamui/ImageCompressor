/* Created by Jimmy on 2/11/2020. */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "../include/compressor.h"
#include "../include/quadtree.h"
#include "../include/visualizer.h"

float distRGBA(QuadTreeRGBA node1, QuadTreeRGBA node2)
{
    return sqrt(
        (node1->r - node2->r) * (node1->r - node2->r) +
        (node1->g - node2->g) * (node1->g - node2->g) +
        (node1->b - node2->b) * (node1->b - node2->b) +
        (node1->a - node2->a) * (node1->a - node2->a));
}

float distBin(QuadTreeBin node1, QuadTreeBin node2)
{
    return sqrt((node1->b - node2->b) * (node1->b - node2->b));
}

QuadTreeRGBA avgRGBA(QuadTreeRGBA nodes[], int size)
{
    int rz, bz, gz;
    float az;
    int i;

    rz = bz = gz = 0;
    az = 0;

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

float distTreeRGBA(QuadTreeRGBA tree1, QuadTreeRGBA tree2)
{
    if (tree1 == NULL || tree2 == NULL)
        return -1;

    if (isLeafRGBA(tree1) && isLeafRGBA(tree2))
    {
        return distRGBA(tree1, tree2);
    }
    else if (isLeafRGBA(tree1) && !isLeafRGBA(tree2))
    {
        return distTreeRGBA(tree1, tree2->northWest) / 4 +
               distTreeRGBA(tree1, tree2->northEast) / 4 +
               distTreeRGBA(tree1, tree2->southWest) / 4 +
               distTreeRGBA(tree1, tree2->southEast) / 4;
    }
    else if (!isLeafRGBA(tree1) && isLeafRGBA(tree2))
    {
        return distTreeRGBA(tree2, tree1->northWest) / 4 +
               distTreeRGBA(tree2, tree1->northEast) / 4 +
               distTreeRGBA(tree2, tree1->southWest) / 4 +
               distTreeRGBA(tree2, tree1->southEast) / 4;
    }
    else if (!isLeafRGBA(tree1) && !isLeafRGBA(tree2))
    {
        return distTreeRGBA(tree1->northWest, tree2->northWest) / 4 +
               distTreeRGBA(tree1->northEast, tree2->northEast) / 4 +
               distTreeRGBA(tree1->southWest, tree2->southWest) / 4 +
               distTreeRGBA(tree1->southEast, tree2->southEast) / 4;
    }
    return -1;
}

float distTreeBin(QuadTreeBin tree1, QuadTreeBin tree2)
{
    if (tree1 == NULL || tree2 == NULL)
        return -1;

    if (isLeafBin(tree1) && isLeafBin(tree2))
    {
        return distBin(tree1, tree2);
    }
    else if (isLeafBin(tree1) && !isLeafBin(tree2))
    {
        return distTreeBin(tree1, tree2->northWest) / 4 +
               distTreeBin(tree1, tree2->northEast) / 4 +
               distTreeBin(tree1, tree2->southWest) / 4 +
               distTreeBin(tree1, tree2->southEast) / 4;
    }
    else if (!isLeafBin(tree1) && isLeafBin(tree2))
    {
        return distTreeBin(tree2, tree1->northWest) / 4 +
               distTreeBin(tree2, tree1->northEast) / 4 +
               distTreeBin(tree2, tree1->southWest) / 4 +
               distTreeBin(tree2, tree1->southEast) / 4;
    }
    else if (!isLeafBin(tree1) && !isLeafBin(tree2))
    {
        return distTreeBin(tree1->northWest, tree2->northWest) / 4 +
               distTreeBin(tree1->northEast, tree2->northEast) / 4 +
               distTreeBin(tree1->southWest, tree2->southWest) / 4 +
               distTreeBin(tree1->southEast, tree2->southEast) / 4;
    }
    return -1;
}

void removeDuplicateLeavesRGBA(QuadTreeRGBA *tree, QuadTreeRGBABuffer *buffer, QuadTreeRGBABuffer *trash)
{
    int buffered;
    int i;

    if (*tree != NULL && isLeafRGBA(*tree))
    {
        /* Checking if leaf is cached */
        buffered = 0;
        for (i = 0; i < (*buffer)->bufferSize; i++)
        {
            if (equivalentRGBA((*buffer)->buffer[i], *tree))
            {
                buffered = 1;
                break;
            }
        }

        if (!buffered)
        {
            offerRGBABuffer(buffer, allocQuadTreeRGBA((*tree)->r, (*tree)->g, (*tree)->b, (*tree)->a));
        }

        if (isBufferedRGBA(*buffer, *tree) == -1 && isBufferedRGBA(*trash, *tree) == -1)
        {
            offerRGBABuffer(trash, *tree);
        }

        /* Relinking */
        *tree = (*buffer)->buffer[i];
    }
    else
    {
        /* Recursive call to children */
        removeDuplicateLeavesRGBA(&(*tree)->northWest, buffer, trash);
        removeDuplicateLeavesRGBA(&(*tree)->northEast, buffer, trash);
        removeDuplicateLeavesRGBA(&(*tree)->southEast, buffer, trash);
        removeDuplicateLeavesRGBA(&(*tree)->southWest, buffer, trash);
    }
}

void removeDuplicateLeavesBin(QuadTreeBin *tree, QuadTreeBinBuffer *buffer, QuadTreeBinBuffer *trash)
{
    int buffered;
    int i;

    if (*tree != NULL && isLeafBin(*tree))
    {
        /* Checking if leaf is cached */
        buffered = 0;
        for (i = 0; i < (*buffer)->bufferSize; i++)
        {
            if (equivalentBin((*buffer)->buffer[i], *tree))
            {
                buffered = 1;
                break;
            }
        }

        if (!buffered)
        {
            offerBinBuffer(buffer, allocQuadTreeBin((*tree)->b));
        }

        if (isBufferedBin(*buffer, *tree) == -1 && isBufferedBin(*trash, *tree) == -1)
        {
            offerBinBuffer(trash, *tree);
        }

        /* Relinking */
        *tree = (*buffer)->buffer[i];
    }
    else
    {
        /* Recursive call to children */
        removeDuplicateLeavesBin(&(*tree)->northWest, buffer, trash);
        removeDuplicateLeavesBin(&(*tree)->northEast, buffer, trash);
        removeDuplicateLeavesBin(&(*tree)->southEast, buffer, trash);
        removeDuplicateLeavesBin(&(*tree)->southWest, buffer, trash);
    }
}

void simplifyTreesRGBA(QuadTreeRGBA *tree, float distErr, QuadTreeRGBABuffer *buffer, QuadTreeRGBABuffer *trash)
{
    int i, k, isCached;
    float dist;
    QuadTreeRGBA *children[4];

    if (*tree == NULL)
        return;

    children[0] = &(*tree)->northWest;
    children[1] = &(*tree)->northEast;
    children[2] = &(*tree)->southEast;
    children[3] = &(*tree)->southWest;

    /* check for each children */
    for (i = 0; i < 4; i++)
    {
        isCached = 0;
        for (k = 0; k < (*buffer)->bufferSize; k++)
        {
            if ((*buffer)->buffer[k] == *children[i])
                continue;
            
            dist = distTreeRGBA(*children[i], (*buffer)->buffer[k]);
            if (dist != -1 && dist <= distErr) /* if similar tree is cached */
            {
                isCached = 1;
                break;
            }
        }

        if (isCached)
        {
            if (treeHeightRGBA((*buffer)->buffer[k]) > treeHeightRGBA(*children[i]))
            {
                offerRGBABuffer(trash, (*buffer)->buffer[k]);
                (*buffer)->buffer[k] = *children[i];
            }
            else
            {
                /* Relinking */
                if (isBufferedRGBA(*buffer, *children[i]) == -1 && isBufferedRGBA(*trash, *children[i]) == -1)
                {
                    offerRGBABuffer(trash, *children[i]);
                }
                *children[i] = (*buffer)->buffer[k];
            }
        }
        else
        {
            /* Adding tree to cache */
            offerRGBABuffer(buffer, *children[i]);
        }
    }

    /* Recursive recall on children */
    for (i = 0; i < 4; i++)
    {
        simplifyTreesRGBA(children[i], distErr, buffer, trash);
    }
}

void simplifyTreesBin(QuadTreeBin *tree, float distErr, QuadTreeBinBuffer *buffer, QuadTreeBinBuffer *trash)
{
    int i, k, isCached;
    float dist;
    QuadTreeBin *children[4];

    if (*tree == NULL)
        return;

    children[0] = &(*tree)->northWest;
    children[1] = &(*tree)->northEast;
    children[2] = &(*tree)->southEast;
    children[3] = &(*tree)->southWest;

    /* check for each children */
    for (i = 0; i < 4; i++)
    {
        isCached = 0;
        for (k = 0; k < (*buffer)->bufferSize; k++)
        {
            if ((*buffer)->buffer[k] == *children[i])
                continue;

            dist = distTreeBin(*children[i], (*buffer)->buffer[k]);
            if (dist != -1 && dist <= distErr) /* if similar tree is cached */
            {
                isCached = 1;
                break;
            }
        }

        if (isCached)
        {
            if (treeHeightBin((*buffer)->buffer[k]) > treeHeightBin(*children[i]))
            {
                offerBinBuffer(trash, (*buffer)->buffer[k]);
                (*buffer)->buffer[k] = *children[i];
            }
            else
            {
                /* Relinking */
                if (isBufferedBin(*buffer, *children[i]) == -1 && isBufferedBin(*trash, *children[i]) == -1)
                {
                    offerBinBuffer(trash, *children[i]);
                }
                *children[i] = (*buffer)->buffer[k];
            }
        }
        else
        {
            /* Adding tree to cache */
            offerBinBuffer(buffer, *children[i]);
        }
    }

    /* Recursive recall on children */
    for (i = 0; i < 4; i++)
    {
        simplifyTreesBin(children[i], distErr, buffer, trash);
    }
}

void minimizeQuadTreeRGBA(QuadTreeRGBA *tree, float distErr)
{
    QuadTreeRGBABuffer buffer = allocQuadTreeRGBABuffer();
    QuadTreeRGBABuffer trash = allocQuadTreeRGBABuffer();

    simplifyTreesRGBA(tree, distErr, &buffer, &trash);
    /*removeDuplicateLeavesRGBA(tree, &buffer, &trash);*/
    freeRGBABuffer(buffer);
    hardFreeRGBABuffer(trash);
}

void minimizeQuadTreeBin(QuadTreeBin *tree, float distErr)
{
    QuadTreeBinBuffer buffer = allocQuadTreeBinBuffer();
    QuadTreeBinBuffer trash = allocQuadTreeBinBuffer();

    simplifyTreesBin(tree, distErr, &buffer, &trash);
    /*removeDuplicateLeavesBin(tree, &buffer, &trash);*/
    freeBinBuffer(buffer);
    hardFreeBinBuffer(trash);
}
