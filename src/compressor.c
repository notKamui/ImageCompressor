/* Created by Jimmy on 2/11/2020. */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "../include/compressor.h"
#include "../include/quadtree.h"
#include "../include/visualizer.h"

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
        return 99999999999;

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
    return 99999999999;
}

float distTreeBin(QuadTreeBin tree1, QuadTreeBin tree2)
{
    if (tree1 == NULL || tree2 == NULL)
        return 99999999999;

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
    return 99999999999;
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

void simplifyTreesRGBA(QuadTreeRGBA *tree, float distErr, QuadTreeRGBABuffer *buffer)
{
    int i, j, k;
    float dist;
    QuadTreeRGBA *children[4];
    QuadTreeRGBA toFree;
    QuadTreeRGBABuffer freeBuffer;

    if (*tree == NULL)
        return;

    children[0] = &(*tree)->northWest;
    children[1] = &(*tree)->northEast;
    children[2] = &(*tree)->southEast;
    children[3] = &(*tree)->southWest;

    /* check for each pair of children */
    for (i = 0; i < 4; i++)
    {
        for (j = i + 1; j < 4; j++)
        {
            if (*children[i] == *children[j])
                continue;

            dist = distTreeRGBA(*children[i], *children[j]);
            if (dist <= distErr) /* if exact (or error) same subtrees */
            {
                toFree = *children[j];
                *children[j] = *children[i];
                freeBuffer = allocQuadTreeRGBABuffer();
                freeQuadTreeRGBA(toFree, &freeBuffer);
                free(freeBuffer->buffer);
                free(freeBuffer);

                for (k = 0; k < (*buffer)->bufferSize; k++)
                {
                    if ((*buffer)->buffer[k] == *children[j])
                        continue;

                    dist = distTreeRGBA(*children[j], (*buffer)->buffer[k]);
                    if (dist <= distErr) /* if similar tree is cached */
                    {
                        /* Relinking */
                        printf("Replace level 2\n");
                        toFree = *children[j];
                        *children[j] = (*buffer)->buffer[k];
                        freeBuffer = allocQuadTreeRGBABuffer();
                        freeQuadTreeRGBA(toFree, &freeBuffer);
                        free(freeBuffer->buffer);
                        free(freeBuffer);
                    }
                    else
                    {
                        /* Adding tree to cache */
                        offerRGBABuffer(buffer, *children[j]);
                    }
                }
            }
        }
    }

    /* Recursive recall on children */
    for (i = 0; i < 4; i++)
    {
        simplifyTreesRGBA(children[i], distErr, buffer);
    }
}

void simplifyTreesBin(QuadTreeBin *tree, float distErr, QuadTreeBinBuffer *buffer)
{
    int i, j, k;
    float dist;
    QuadTreeBin *children[4];
    QuadTreeBin toFree;
    QuadTreeBinBuffer freeBuffer;

    if (*tree == NULL)
        return;

    children[0] = &(*tree)->northWest;
    children[1] = &(*tree)->northEast;
    children[2] = &(*tree)->southEast;
    children[3] = &(*tree)->southWest;

    /* check for each pair of children */
    for (i = 0; i < 4; i++)
    {
        for (j = i + 1; j < 4; j++)
        {
            if (*children[i] == *children[j])
                continue;

            dist = distTreeBin(*children[i], *children[j]);
            if (dist <= distErr) /* if exact (or error) same subtrees */
            {
                toFree = *children[j];
                *children[j] = *children[i];
                freeBuffer = allocQuadTreeBinBuffer();
                freeQuadTreeBin(toFree, &freeBuffer);
                free(freeBuffer->buffer);
                free(freeBuffer);

                for (k = 0; k < (*buffer)->bufferSize; k++)
                {
                    if ((*buffer)->buffer[k] == *children[j])
                        continue;

                    dist = distTreeBin(*children[j], (*buffer)->buffer[k]);
                    if (dist <= distErr) /* if similar tree is cached */
                    {
                        /* Relinking */
                        printf("Replace level 2\n");
                        toFree = *children[j];
                        *children[j] = (*buffer)->buffer[k];
                        freeBuffer = allocQuadTreeBinBuffer();
                        freeQuadTreeBin(toFree, &freeBuffer);
                        free(freeBuffer->buffer);
                        free(freeBuffer);
                    }
                    else
                    {
                        /* Adding tree to cache */
                        offerBinBuffer(buffer, *children[j]);
                    }
                }
            }
        }
    }

    /* Recursive recall on children */
    for (i = 0; i < 4; i++)
    {
        simplifyTreesBin(children[i], distErr, buffer);
    }
}

void minimizeQuadTreeRGBA(QuadTreeRGBA *tree, float distErr)
{
    QuadTreeRGBABuffer buffer = allocQuadTreeRGBABuffer();
    QuadTreeRGBABuffer trash = allocQuadTreeRGBABuffer();
    int i;

    simplifyTreesRGBA(tree, distErr, &buffer);
    removeDuplicateLeavesRGBA(tree, &buffer, &trash);
    free(buffer->buffer);
    free(buffer);

    for (i = 0; i < trash->bufferSize; i++)
    {
        free(trash->buffer[i]);
    }
    free(trash->buffer);
    free(trash);
}

void minimizeQuadTreeBin(QuadTreeBin *tree, float distErr)
{
    QuadTreeBinBuffer buffer = allocQuadTreeBinBuffer();
    QuadTreeBinBuffer trash = allocQuadTreeBinBuffer();
    int i;

    simplifyTreesBin(tree, distErr, &buffer);
    removeDuplicateLeavesBin(tree, &buffer, &trash);
    free(buffer->buffer);
    free(buffer);

    for (i = 0; i < trash->bufferSize; i++)
    {
        free(trash->buffer[i]);
    }
    free(trash->buffer);
    free(trash);
}