/* Created by Irwin on 27/10/2020. */
#include <stdlib.h>
#include <stdio.h>
#include "../include/quadtree.h"

int isLeafRGBA(QuadTreeRGBA tree)
{
    return tree->northWest == NULL &&
           tree->southWest == NULL &&
           tree->northEast == NULL &&
           tree->southEast == NULL;
}

int isLeafBin(QuadTreeBin tree)
{
    return tree->northWest == NULL &&
           tree->southWest == NULL &&
           tree->northEast == NULL &&
           tree->southEast == NULL;
}

int equivalentRGBA(QuadTreeRGBA tree1, QuadTreeRGBA tree2)
{
    return tree1->r == tree2->r && tree1->g == tree2->g && tree1->b == tree2->b && tree1->a == tree2->a;
}

int equivalentBin(QuadTreeBin tree1, QuadTreeBin tree2)
{
    return tree1->b == tree2->b;
}

int max(int a, int b)
{
    return a > b ? a : b;
}

int treeHeightRGBA(QuadTreeRGBA tree)
{
    if (tree == NULL)
    {
        return 0;
    }
    else
    {
        int nwH, neH, swH, seH;

        nwH = treeHeightRGBA(tree->northWest);
        neH = treeHeightRGBA(tree->northEast);
        swH = treeHeightRGBA(tree->southWest);
        seH = treeHeightRGBA(tree->southEast);

        return max(max(nwH, neH), max(swH, seH)) + 1;
    }
    
}

int treeHeightBin(QuadTreeBin tree)
{
    if (tree == NULL)
    {
        return 0;
    }
    else
    {
        int nwH, neH, swH, seH;

        nwH = treeHeightBin(tree->northWest);
        neH = treeHeightBin(tree->northEast);
        swH = treeHeightBin(tree->southWest);
        seH = treeHeightBin(tree->southEast);

        return max(max(nwH, neH), max(swH, seH)) + 1;
    }
    
}

QuadTreeRGBA allocQuadTreeRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    QuadTreeRGBA tree;
    if ((tree = malloc(sizeof(struct s_quadtree_rgba))) == NULL)
    {
        fprintf(stderr, "FATAL: Allocation error.\n");
        exit(1);
    }

    tree->northWest = NULL;
    tree->northEast = NULL;
    tree->southWest = NULL;
    tree->southEast = NULL;
    tree->r = r;
    tree->g = g;
    tree->b = b;
    tree->a = a;

    return tree;
}

QuadTreeBin allocQuadTreeBin(unsigned char b)
{
    QuadTreeBin tree;
    if ((tree = malloc(sizeof(struct s_quadtree_bin))) == NULL)
    {
        fprintf(stderr, "FATAL: Allocation error.\n");
        exit(1);
    }

    tree->northWest = NULL;
    tree->northEast = NULL;
    tree->southWest = NULL;
    tree->southEast = NULL;
    tree->b = b;

    return tree;
}

QuadTreeRGBABuffer allocQuadTreeRGBABuffer()
{
    QuadTreeRGBABuffer buffer;
    if ((buffer = malloc(sizeof(struct s_quadtree_rgba_buffer))) == NULL)
    {
        fprintf(stderr, "FATAL: Allocation error.\n");
        exit(1);
    }

    buffer->buffer = NULL;
    buffer->bufferSize = 0;
    return buffer;
}

QuadTreeBinBuffer allocQuadTreeBinBuffer()
{
    QuadTreeBinBuffer buffer;
    if ((buffer = malloc(sizeof(struct s_quadtree_bin_buffer))) == NULL)
    {
        fprintf(stderr, "FATAL: Allocation error.\n");
        exit(1);
    }

    buffer->buffer = NULL;
    buffer->bufferSize = 0;
    return buffer;
}

void setRGBABuffer(QuadTreeRGBABuffer *buffer, QuadTreeRGBA tree, int index)
{
    int i;

    if ((*buffer)->buffer == NULL)
    {
        if (((*buffer)->buffer = calloc(sizeof(QuadTreeRGBA), index + 1)) == NULL)
        {
            fprintf(stderr, "FATAL: Allocation error.\n");
            exit(1);
        }
        for (i = 0; i < index; i++)
        {
            (*buffer)->buffer[i] = NULL;
        }
        (*buffer)->bufferSize = index + 1;
    }
    else if (index >= (*buffer)->bufferSize)
    {
        if (((*buffer)->buffer = realloc((*buffer)->buffer, (index + 1) * sizeof(QuadTreeRGBA))) == NULL)
        {
            fprintf(stderr, "FATAL: Allocation error.\n");
            exit(1);
        }
        for (i = (*buffer)->bufferSize; i < index; i++)
        {
            (*buffer)->buffer[i] = NULL;
        }
        (*buffer)->bufferSize = index + 1;
    }
    (*buffer)->buffer[index] = tree;
}

void setBinBuffer(QuadTreeBinBuffer *buffer, QuadTreeBin tree, int index)
{
    int i;

    if ((*buffer)->buffer == NULL)
    {
        if (((*buffer)->buffer = calloc(sizeof(QuadTreeBin), index + 1)) == NULL)
        {
            fprintf(stderr, "FATAL: Allocation error.\n");
            exit(1);
        }
        for (i = 0; i < index; i++)
        {
            (*buffer)->buffer[i] = NULL;
        }
        (*buffer)->bufferSize = index + 1;
    }
    else if (index >= (*buffer)->bufferSize)
    {
        if (((*buffer)->buffer = realloc((*buffer)->buffer, (index + 1) * sizeof(QuadTreeBin))) == NULL)
        {
            fprintf(stderr, "FATAL: Allocation error.\n");
            exit(1);
        }
        for (i = (*buffer)->bufferSize; i < index; i++)
        {
            (*buffer)->buffer[i] = NULL;
        }
        (*buffer)->bufferSize = index + 1;
    }
    (*buffer)->buffer[index] = tree;
}

void offerRGBABuffer(QuadTreeRGBABuffer *buffer, QuadTreeRGBA tree)
{
    setRGBABuffer(buffer, tree, (*buffer)->bufferSize);
}

void offerBinBuffer(QuadTreeBinBuffer *buffer, QuadTreeBin tree)
{
    setBinBuffer(buffer, tree, (*buffer)->bufferSize);
}

int isBufferedRGBA(QuadTreeRGBABuffer buffer, QuadTreeRGBA tree)
{
    int i;
    for (i = 0; i < buffer->bufferSize; i++)
    {
        if (buffer->buffer[i] == tree)
            return i;
    }
    return -1;
}

int isBufferedBin(QuadTreeBinBuffer buffer, QuadTreeBin tree)
{
    int i;
    for (i = 0; i < buffer->bufferSize; i++)
    {
        if (buffer->buffer[i] == tree)
            return i;
    }
    return -1;
}

void freeRGBABuffer(QuadTreeRGBABuffer buffer)
{
    free(buffer->buffer);
    free(buffer);
}

void hardFreeRGBABuffer(QuadTreeRGBABuffer buffer)
{
    int i;
    QuadTreeRGBABuffer freeBuffer = allocQuadTreeRGBABuffer();
    for (i = 0; i < buffer->bufferSize; i++)
    {
        freeQuadTreeRGBA(buffer->buffer[i], &freeBuffer);
    }
    freeRGBABuffer(buffer);
    freeRGBABuffer(freeBuffer);
}

void freeBinBuffer(QuadTreeBinBuffer buffer)
{
    free(buffer->buffer);
    free(buffer);
}

void hardFreeBinBuffer(QuadTreeBinBuffer buffer)
{
    int i;
    QuadTreeBinBuffer freeBuffer = allocQuadTreeBinBuffer();
    for (i = 0; i < buffer->bufferSize; i++)
    {
        freeQuadTreeBin(buffer->buffer[i], &freeBuffer);
    }
    freeBinBuffer(buffer);
    freeBinBuffer(freeBuffer);
}

void freeQuadTreeRGBA(QuadTreeRGBA tree, QuadTreeRGBABuffer *buffer)
{
    if (tree != NULL && isBufferedRGBA(*buffer, tree) == -1)
    {
        freeQuadTreeRGBA(tree->northWest, buffer);
        freeQuadTreeRGBA(tree->northEast, buffer);
        freeQuadTreeRGBA(tree->southEast, buffer);
        freeQuadTreeRGBA(tree->southWest, buffer);
        offerRGBABuffer(buffer, tree);
        free(tree);
    }
}

void freeQuadTreeBin(QuadTreeBin tree, QuadTreeBinBuffer *buffer)
{
    if (tree != NULL && isBufferedBin(*buffer, tree) == -1)
    {
        freeQuadTreeBin(tree->northWest, buffer);
        freeQuadTreeBin(tree->northEast, buffer);
        freeQuadTreeBin(tree->southEast, buffer);
        freeQuadTreeBin(tree->southWest, buffer);
        offerBinBuffer(buffer, tree);
        free(tree);
    }
}

int sizeQuadTreeRGBA(QuadTreeRGBA tree, QuadTreeRGBABuffer *buffer)
{
    if (tree != NULL && isBufferedRGBA(*buffer, tree) == -1)
    {
        offerRGBABuffer(buffer, tree);
        return sizeQuadTreeRGBA(tree->northWest, buffer) +
               sizeQuadTreeRGBA(tree->northEast, buffer) +
               sizeQuadTreeRGBA(tree->southEast, buffer) +
               sizeQuadTreeRGBA(tree->southWest, buffer) +
               1;
    }
    return 0;
}

int sizeQuadTreeBin(QuadTreeBin tree, QuadTreeBinBuffer *buffer)
{
    if (tree != NULL && isBufferedBin(*buffer, tree) == -1)
    {
        offerBinBuffer(buffer, tree);
        return sizeQuadTreeBin(tree->northWest, buffer) +
               sizeQuadTreeBin(tree->northEast, buffer) +
               sizeQuadTreeBin(tree->southEast, buffer) +
               sizeQuadTreeBin(tree->southWest, buffer) +
               1;
    }
    return 0;
}
