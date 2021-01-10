/* Created by Jimmy on 2/11/2020. */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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

float distTreeRGBA(QuadTreeRGBA tree1, QuadTreeRGBA tree2) {
    if (tree1 == NULL || tree2 == NULL) return 99999999999;

    if (isLeafRGBA(tree1) && isLeafRGBA(tree2)) 
    {
        return distRGBA(tree1, tree2);
    } 
    else if (isLeafRGBA(tree1) && !isLeafRGBA(tree2)) 
    {
        return  distTreeRGBA(tree1, tree2->northWest) / 4 +
                distTreeRGBA(tree1, tree2->northEast) / 4 +
                distTreeRGBA(tree1, tree2->southWest) / 4 +
                distTreeRGBA(tree1, tree2->southEast) / 4;
    } 
    else if (!isLeafRGBA(tree1) && isLeafRGBA(tree2)) 
    {
        return  distTreeRGBA(tree2, tree1->northWest) / 4 +
                distTreeRGBA(tree2, tree1->northEast) / 4 +
                distTreeRGBA(tree2, tree1->southWest) / 4 +
                distTreeRGBA(tree2, tree1->southEast) / 4;
    } 
    else if (!isLeafRGBA(tree1) && !isLeafRGBA(tree2)) 
    {
        return  distTreeRGBA(tree1->northWest, tree2->northWest) / 4 +
                distTreeRGBA(tree1->northEast, tree2->northEast) / 4 +
                distTreeRGBA(tree1->southWest, tree2->southWest) / 4 +
                distTreeRGBA(tree1->southEast, tree2->southEast) / 4;
    }
    return 99999999999;
}

float distTreeBin(QuadTreeBin tree1, QuadTreeBin tree2)
{
    if (tree1 == NULL || tree2 == NULL) return 99999999999;

    if (isLeafBin(tree1) && isLeafBin(tree2))
    {
        return distBin(tree1, tree2);
    }
    else if (isLeafBin(tree1) && !isLeafBin(tree2)) 
    {
        return  distTreeBin(tree1, tree2->northWest) / 4 +
                distTreeBin(tree1, tree2->northEast) / 4 +
                distTreeBin(tree1, tree2->southWest) / 4 +
                distTreeBin(tree1, tree2->southEast) / 4;
    } 
    else if (!isLeafBin(tree1) && isLeafBin(tree2)) 
    {
        return  distTreeBin(tree2, tree1->northWest) / 4 +
                distTreeBin(tree2, tree1->northEast) / 4 +
                distTreeBin(tree2, tree1->southWest) / 4 +
                distTreeBin(tree2, tree1->southEast) / 4;
    } 
    else if (!isLeafBin(tree1) && !isLeafBin(tree2)) 
    {
        return  distTreeBin(tree1->northWest, tree2->northWest) / 4 +
                distTreeBin(tree1->northEast, tree2->northEast) / 4 +
                distTreeBin(tree1->southWest, tree2->southWest) / 4 +
                distTreeBin(tree1->southEast, tree2->southEast) / 4;
    }
    return 99999999999;
}

void takeRGBALeaves(QuadTreeRGBA *tree, QuadTreeRGBA *cache, int *size) {
    int isCached;
    int i;

    if (*tree != NULL && isLeafRGBA(*tree)) 
    {
        /* Checking if leaf is cached */
        isCached = 0;
        for (i = 0; i < *size; i++) 
        {
            if (
                cache[i]->r == (*tree)->r &&
                cache[i]->g == (*tree)->g &&
                cache[i]->b == (*tree)->b &&
                cache[i]->a == (*tree)->a
            ) 
            {
                isCached = 1;
                break;
            }
        }

        if (!isCached) 
        {
            /* Adding leaf to cache */
            *size += 1;
            cache = (QuadTreeRGBA*)realloc(cache, sizeof(QuadTreeRGBA)*(*size));
            cache[*size-1] = allocQuadTreeRGBA((*tree)->r, (*tree)->g, (*tree)->b, (*tree)->a);
        }

        /* Relinking */
        *tree = cache[i];
    } 
    else 
    {
        /* Recursive call to children */
        takeRGBALeaves(&(*tree)->northWest, cache, size);
        takeRGBALeaves(&(*tree)->northEast, cache, size);
        takeRGBALeaves(&(*tree)->southEast, cache, size);
        takeRGBALeaves(&(*tree)->southWest, cache, size);
    }
}

void minTreeRGBANoLoss(QuadTreeRGBA *tree) {
    QuadTreeRGBA *cache = (QuadTreeRGBA*)malloc(0);
    int size = 0;
    takeRGBALeaves(tree, cache, &size);
    free(cache);
}

void takeBinLeaves(QuadTreeBin *tree, QuadTreeBin *cache, int *size) {
    int isCached;
    int i;

    if (*tree != NULL && isLeafBin(*tree))
    {
        /* Checking if leaf is cached */
        isCached = 0;
        for (i = 0; i < *size; i++) {
            if (cache[i]->b == (*tree)->b) {
                isCached = 1;
                break;
            }
        }

        if (!isCached)  {
            /* Adding leaf to cache */
            *size += 1;
            cache = (QuadTreeBin*)realloc(cache, sizeof(QuadTreeBin)*(*size));
            cache[*size-1] = allocQuadTreeBin((*tree)->b);
        }

        /* Relinking */
        *tree = cache[i];
    } 
    else
    {
        /* Recursive call to children */
        takeBinLeaves(&(*tree)->northWest, cache, size);
        takeBinLeaves(&(*tree)->northEast, cache, size);
        takeBinLeaves(&(*tree)->southEast, cache, size);
        takeBinLeaves(&(*tree)->southWest, cache, size);
    }
}

void minTreeBinNoLoss(QuadTreeBin *tree) 
{
    QuadTreeBin *cache = (QuadTreeBin*)malloc(0);
    int size = 0;
    takeBinLeaves(tree, cache, &size);
    free(cache);
}

void takeRGBATrees(QuadTreeRGBA *tree, float distErr, QuadTreeRGBA *cache, int *size)
{
    int i, j, k;
    float dist;
    QuadTreeRGBA **children = (QuadTreeRGBA**)malloc(sizeof(QuadTreeRGBA*)*4);

    if (*tree == NULL) return;

    children[0] = &(*tree)->northWest;
    children[1] = &(*tree)->northEast;
    children[2] = &(*tree)->southEast;
    children[3] = &(*tree)->southWest;

    /* check for each pair of children */
    for (i = 0; i < 4; i++) 
    {
        for (j = i+1; j < 4; j++) 
        {
            dist = distTreeRGBA(*children[i], *children[j]);
            if (dist <= distErr) /* if exact (or error) same subtrees */
            { 
                *children[j] = *children[i];
                for (k = 0; k < *size; k++)
                {
                    dist = distTreeRGBA(*children[j], cache[k]);
                    if (dist <= distErr) /* if similar tree is cached */
                    {
                        /* Relinking */
                        *children[j] = cache[k];
                    } else { /* Adding tree to cache */
                        *size += 1;
                        cache = (QuadTreeRGBA*)realloc(cache, sizeof(QuadTreeRGBA)*(*size));
                        cache[*size-1] = *children[j];
                    }
                }
            }
        }
    }

    /* Recursive recall on non leaves */
    for (i = 0; i < 4; i++) 
    {
        if (!isLeafRGBA(*children[i])) 
        {
            takeRGBATrees(children[i], distErr, cache, size);
        }
    }

    free(children);
}

void minTreeRGBALoss(QuadTreeRGBA *tree, float distErr)
{
    QuadTreeRGBA *cache = (QuadTreeRGBA*)malloc(0);
    int size = 0;
    takeRGBATrees(tree, distErr, cache, &size);
    free(cache);
}

void takeBinTrees(QuadTreeBin *tree, float distErr, QuadTreeBin *cache, int *size) 
{
    int i, j, k;
    float dist;
    QuadTreeBin **children = (QuadTreeBin**)malloc(sizeof(QuadTreeBin*)*4);

    if (*tree == NULL) return;

    children[0] = &(*tree)->northWest;
    children[1] = &(*tree)->northEast;
    children[2] = &(*tree)->southEast;
    children[3] = &(*tree)->southWest;

    /* check for each pair of children */
    for (i = 0; i < 4; i++) 
    {
        for (j = i+1; j < 4; j++) 
        {
            dist = distTreeBin(*children[i], *children[j]);
            if (dist <= distErr) /* if exact (or error) same subtrees */
            { 
                *children[j] = *children[i];
                for (k = 0; k < *size; k++)
                {
                    dist = distTreeBin(*children[j], cache[k]);
                    if (dist <= distErr) /* if similar tree is cached */
                    {
                        /* Relinking */
                        *children[j] = cache[k];
                    } else { /* Adding tree to cache */
                        *size += 1;
                        cache = (QuadTreeBin*)realloc(cache, sizeof(QuadTreeBin)*(*size));
                        cache[*size-1] = *children[j];
                    }
                }
            }
        }
    }

    /* Recursive recall on non leaves */
    for (i = 0; i < 4; i++) 
    {
        if (!isLeafBin(*children[i])) 
        { 
            takeBinTrees(children[i], distErr, cache, size);
        }
    }

    free(children);
}

void minTreeBinLoss(QuadTreeBin *tree, float distErr)
{
    QuadTreeBin *cache = (QuadTreeBin*)malloc(0);
    int size = 0;
    takeBinTrees(tree, distErr, cache, &size);
    free(cache);
}