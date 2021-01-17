/* Created by Jimmy on 2/11/2020. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <MLV/MLV_color.h>

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

MLV_Color avgColorRGBA(QuadTreeRGBA tree, int index)
{
    MLV_Color color;
    Uint8 tmpR, tmpG, tmpB, tmpA;
    double r = 0, g = 0, b = 0, a = 0;
    int i;
    QuadTreeRGBA children[4];

    children[0] = tree->northWest;
    children[1] = tree->northEast;
    children[2] = tree->southEast;
    children[3] = tree->southWest;

    if (tree == NULL)
    {
        return MLV_rgba(0, 0, 0, 0);
    }

    if (isLeafRGBA(tree))
    {
        return MLV_rgba(tree->r, tree->g, tree->b, tree->a);
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            color = avgColorRGBA(children[i], index + 1);
            MLV_convert_color_to_rgba(color, &tmpR, &tmpG, &tmpB, &tmpA);
            r += tmpR;
            g += tmpG;
            b += tmpB;
            a += tmpA;
        }
        return MLV_rgba(floor(r / 4), floor(g / 4), floor(b / 4), floor(a / 4));
    }
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
        return (distTreeRGBA(tree1, tree2->northWest) +
               distTreeRGBA(tree1, tree2->northEast) +
               distTreeRGBA(tree1, tree2->southWest) +
               distTreeRGBA(tree1, tree2->southEast)) * .25; /* (1/4) * is way faster than / */
    }
    else if (!isLeafRGBA(tree1) && isLeafRGBA(tree2))
    {
        return (distTreeRGBA(tree2, tree1->northWest) +
               distTreeRGBA(tree2, tree1->northEast) +
               distTreeRGBA(tree2, tree1->southWest) +
               distTreeRGBA(tree2, tree1->southEast)) * .25;
    }
    else if (!isLeafRGBA(tree1) && !isLeafRGBA(tree2))
    {
        return (distTreeRGBA(tree1->northWest, tree2->northWest) +
               distTreeRGBA(tree1->northEast, tree2->northEast) +
               distTreeRGBA(tree1->southWest, tree2->southWest) +
               distTreeRGBA(tree1->southEast, tree2->southEast)) * .25;
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
        return (distTreeBin(tree1, tree2->northWest) +
               distTreeBin(tree1, tree2->northEast) +
               distTreeBin(tree1, tree2->southWest) +
               distTreeBin(tree1, tree2->southEast)) * .25;
    }
    else if (!isLeafBin(tree1) && isLeafBin(tree2))
    {
        return (distTreeBin(tree2, tree1->northWest) +
               distTreeBin(tree2, tree1->northEast) +
               distTreeBin(tree2, tree1->southWest) +
               distTreeBin(tree2, tree1->southEast)) * .25;
    }
    else if (!isLeafBin(tree1) && !isLeafBin(tree2))
    {
        return (distTreeBin(tree1->northWest, tree2->northWest) +
               distTreeBin(tree1->northEast, tree2->northEast) +
               distTreeBin(tree1->southWest, tree2->southWest) +
               distTreeBin(tree1->southEast, tree2->southEast)) * .25;
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
    int i;

    if (*tree != NULL && isLeafBin(*tree))
    {
        for (i = 0; i < (*buffer)->bufferSize; i++) {
            if (equivalentBin((*buffer)->buffer[i], *tree)) {
                /* Relinking */
                offerBinBuffer(trash, *tree);
                *tree = (*buffer)->buffer[i];
            }
        }        
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

void naiveTreeReductionRGBA(QuadTreeRGBA *tree, float distErr, QuadTreeRGBABuffer *trash)
{
    float dist;
    MLV_Color avg;
    unsigned char r, g, b, a;
    QuadTreeRGBA tmp;

    if (*tree == NULL)
        return;

    /* Creating the average leaf color of the current tree */
    avg = avgColorRGBA(*tree, 0);
    MLV_convert_color_to_rgba(avg, &r, &g, &b, &a);
    tmp = allocQuadTreeRGBA(r, g, b, a);

    dist = distTreeRGBA(tmp, *tree);
    if (dist <= distErr)
    {
        /* Discarding the previous tree and replacing it by its average */
        offerRGBABuffer(trash, *tree);
        *tree = tmp;
    }
    else
    {
        /* Discarding the average and recursive call on children */
        offerRGBABuffer(trash, tmp);

        naiveTreeReductionRGBA(&(*tree)->northWest, distErr, trash);
        naiveTreeReductionRGBA(&(*tree)->northEast, distErr, trash);
        naiveTreeReductionRGBA(&(*tree)->southWest, distErr, trash);
        naiveTreeReductionRGBA(&(*tree)->southEast, distErr, trash);
    }
}

void simplifyTreesRGBA(QuadTreeRGBA *tree, float distErr, QuadTreeRGBABuffer *buffer, QuadTreeRGBABuffer *trash)
{
    int k, heightOfCurrent;
    float dist;

    if (isLeafRGBA(*tree))
        return;

    /* Recursive recall on children to go down the tree first */
    simplifyTreesRGBA(&(*tree)->northWest, distErr, buffer, trash);
    simplifyTreesRGBA(&(*tree)->northEast, distErr, buffer, trash);
    simplifyTreesRGBA(&(*tree)->southWest, distErr, buffer, trash);
    simplifyTreesRGBA(&(*tree)->southEast, distErr, buffer, trash);

    heightOfCurrent = treeHeightRGBA(*tree);

    if ((*buffer)->bufferSize == 0)
        offerRGBABuffer(buffer, *tree);

    /* check if similar tree is cached in the buffer */
    for (k = 0; k < (*buffer)->bufferSize; k++)
    {
        if ((*buffer)->buffer[k] == *tree) {
            continue;
        }

        if (treeHeightRGBA((*buffer)->buffer[k]) <= heightOfCurrent)
        {
            dist = distTreeRGBA(*tree, (*buffer)->buffer[k]);
            if (dist != -1 && dist <= distErr) /* if similar tree is cached */
            {
                /* Relinking */
                offerRGBABuffer(trash, *tree);
                *tree = (*buffer)->buffer[k];
                return;
            }
            else
            {
                /* Adding tree to cache */
                if (!isBufferedRGBA(*buffer, *tree)) {
                    offerRGBABuffer(buffer, *tree);
                }
            }
        }
        else 
        {
            /* Adding tree to cache */
            offerRGBABuffer(buffer, *tree);
        }  
    }
}

void simplifyTreesBin(QuadTreeBin *tree, float distErr, QuadTreeBinBuffer *buffer, QuadTreeBinBuffer *trash)
{
    int k, heightOfCurrent;
    float dist;

    if (isLeafBin(*tree))
        return;

    /* Recursive recall on children to go down the tree first */
    simplifyTreesBin(&(*tree)->northWest, distErr, buffer, trash);
    simplifyTreesBin(&(*tree)->northEast, distErr, buffer, trash);
    simplifyTreesBin(&(*tree)->southWest, distErr, buffer, trash);
    simplifyTreesBin(&(*tree)->southEast, distErr, buffer, trash);

    heightOfCurrent = treeHeightBin(*tree);

    if ((*buffer)->bufferSize == 0)
        offerBinBuffer(buffer, *tree);

    /* check if similar tree is cached in the buffer */
    for (k = 0; k < (*buffer)->bufferSize; k++)
    {
        if ((*buffer)->buffer[k] == *tree) {
            continue;
        }

        if (treeHeightBin((*buffer)->buffer[k]) <= heightOfCurrent)
        {
            dist = distTreeBin(*tree, (*buffer)->buffer[k]);
            if (dist != -1 && dist <= distErr) /* if similar tree is cached */
            {
                /* Relinking */
                offerBinBuffer(trash, *tree);
                *tree = (*buffer)->buffer[k];
                return;
            }
            else
            {
                /* Adding tree to cache */
                if (!isBufferedBin(*buffer, *tree)) {
                    offerBinBuffer(buffer, *tree);
                }
            }
        }
        else 
        {
            /* Adding tree to cache */
            offerBinBuffer(buffer, *tree);
        }  
    }
}

void minimizeQuadTreeRGBA(QuadTreeRGBA *tree, float distErr)
{
    int i;
    QuadTreeRGBABuffer buffer1 = allocQuadTreeRGBABuffer();
    QuadTreeRGBABuffer buffer2 = allocQuadTreeRGBABuffer();
    QuadTreeRGBABuffer trash1 = allocQuadTreeRGBABuffer();
    QuadTreeRGBABuffer trash2 = allocQuadTreeRGBABuffer();
    QuadTreeRGBABuffer trash3 = allocQuadTreeRGBABuffer();
    QuadTreeRGBABuffer trueTrash = allocQuadTreeRGBABuffer();

    printf("Starting minimization...\n");
    if (distErr > 0) { /* no point in doing that if the allowed error is 0 */
        naiveTreeReductionRGBA(tree, distErr, &trash1);
    }
    removeDuplicateLeavesRGBA(tree, &buffer1, &trash2);
    simplifyTreesRGBA(tree, distErr, &buffer2, &trash3);

    printf("Minimization completed !\nFreeing buffers...\n");
    freeRGBABuffer(buffer1);
    hardFreeRGBABuffer(trash1);
    hardFreeRGBABuffer(trash2);
    /* checking for truly unused nodes */
    for (i = 0; i < trash3->bufferSize; i++) {
        if (!isBufferedRGBA(buffer2, trash3->buffer[i])) {
            offerRGBABuffer(&trueTrash, trash3->buffer[i]);
        }
    }
    freeRGBABuffer(buffer2);
    freeRGBABuffer(trash3);
    hardFreeRGBABuffer(trueTrash);
    printf("Completed !\n");
}

void minimizeQuadTreeBin(QuadTreeBin *tree, float distErr)
{
    int i;
    QuadTreeBinBuffer buffer1 = allocQuadTreeBinBuffer();
    QuadTreeBinBuffer buffer2 = allocQuadTreeBinBuffer();
    QuadTreeBinBuffer trash1 = allocQuadTreeBinBuffer();
    QuadTreeBinBuffer trash2 = allocQuadTreeBinBuffer();
    QuadTreeBinBuffer trueTrash = allocQuadTreeBinBuffer();

    /* since there will be no other leaves, might as well make them in advance */
    QuadTreeBin black = allocQuadTreeBin(0);
    QuadTreeBin white = allocQuadTreeBin(1);
    offerBinBuffer(&buffer1, black);
    offerBinBuffer(&buffer1, white);

    printf("Starting minimization...\n");
    removeDuplicateLeavesBin(tree, &buffer1, &trash1);
    simplifyTreesBin(tree, distErr, &buffer2, &trash2);

    printf("Minimization completed !\nFreeing buffers...\n");
    freeBinBuffer(buffer1);
    hardFreeBinBuffer(trash1);
    /* checking for truly unused nodes */
    for (i = 0; i < trash2->bufferSize; i++) {
        if (!isBufferedBin(buffer2, trash2->buffer[i])) {
            offerBinBuffer(&trueTrash, trash2->buffer[i]);
        }
    }
    freeBinBuffer(buffer2);
    freeBinBuffer(trash2);
    hardFreeBinBuffer(trueTrash);
    printf("Completed !\n");
}
