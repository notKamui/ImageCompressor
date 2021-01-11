#include <stdio.h>
#include <stdlib.h>
#include "../include/quadtree.h"
#include "../include/writer.h"

void writeBit(FILE *file, unsigned char bit, unsigned char *buffer, size_t *bufferSize)
{
    *buffer = (*buffer << 1) | bit;
    (*bufferSize)++;

    if (*bufferSize == 8)
    {
        fprintf(file, "%c", *buffer);
        *bufferSize = 0;
        *buffer = 0;
    }
}

void writeByte(FILE *file, unsigned char byte, unsigned char *buffer, size_t *bufferSize)
{
    int i;
    for (i = 7; i >= 0; i--)
    {
        writeBit(file, (byte >> i) & 1, buffer, bufferSize);
    }
}

void flush(FILE *file, unsigned char *buffer, size_t *bufferSize)
{
    int i;
    if (*bufferSize > 0)
    {
        for (i = *bufferSize; i < 8; i++)
        {
            writeBit(file, 0, buffer, bufferSize);
        }
    }
}

void writeRGBA(QuadTreeRGBA tree, FILE *file, unsigned char *buffer, size_t *bufferSize)
{
    if (tree == NULL)
    {
        return;
    }

    if (tree->northWest || tree->northEast || tree->southEast || tree->southWest)
    {
        writeBit(file, 0, buffer, bufferSize);
    }
    else
    {
        writeBit(file, 1, buffer, bufferSize);
        writeByte(file, tree->r, buffer, bufferSize);
        writeByte(file, tree->g, buffer, bufferSize);
        writeByte(file, tree->b, buffer, bufferSize);
        writeByte(file, tree->a, buffer, bufferSize);
    }

    writeRGBA(tree->northWest, file, buffer, bufferSize);
    writeRGBA(tree->northEast, file, buffer, bufferSize);
    writeRGBA(tree->southEast, file, buffer, bufferSize);
    writeRGBA(tree->southWest, file, buffer, bufferSize);
}

void writeBin(QuadTreeBin tree, FILE *file, unsigned char *buffer, size_t *bufferSize)
{
    if (tree == NULL)
    {
        return;
    }

    if (tree->northWest || tree->northEast || tree->southEast || tree->southWest)
    {
        writeBit(file, 0, buffer, bufferSize);
    }
    else
    {
        writeBit(file, 1, buffer, bufferSize);
        writeBit(file, (tree->b > 0), buffer, bufferSize);
    }

    writeBin(tree->northWest, file, buffer, bufferSize);
    writeBin(tree->northEast, file, buffer, bufferSize);
    writeBin(tree->southEast, file, buffer, bufferSize);
    writeBin(tree->southWest, file, buffer, bufferSize);
}

void writeMinimizedRGBA(QuadTreeRGBA tree, FILE *file, QuadTreeRGBABuffer *treeBuffer, int index, size_t *treeSize)
{
    int i, j;

    QuadTreeRGBA children[4];
    int childrenIdentifiers[4] = {0, 0, 0, 0};

    if (tree == NULL)
    {
        return;
    }

    children[0] = tree->northWest;
    children[1] = tree->northEast;
    children[2] = tree->southEast;
    children[3] = tree->southWest;

    if (isBufferedRGBA(*treeBuffer, tree) >= 0)
        return;

    if (!isLeafRGBA(tree))
    {
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < i; j++)
            {
                if (children[j] == children[i])
                {
                    childrenIdentifiers[i] = childrenIdentifiers[j];
                    break;
                }
            }
            if (i == 0 || i == j)
            {
                childrenIdentifiers[i] = ++(*treeSize);
            }
        }
        fprintf(file, "%d %d %d %d %d\n", index, childrenIdentifiers[0], childrenIdentifiers[1], childrenIdentifiers[2], childrenIdentifiers[3]);
    }
    else
    {
        fprintf(file, "%df %d %d %d %d\n", index, tree->r, tree->g, tree->b, tree->a);
    }

    offerRGBABuffer(treeBuffer, tree);

    writeMinimizedRGBA(tree->northWest, file, treeBuffer, childrenIdentifiers[0], treeSize);
    writeMinimizedRGBA(tree->northEast, file, treeBuffer, childrenIdentifiers[1], treeSize);
    writeMinimizedRGBA(tree->southEast, file, treeBuffer, childrenIdentifiers[2], treeSize);
    writeMinimizedRGBA(tree->southWest, file, treeBuffer, childrenIdentifiers[3], treeSize);

    if (index == 0)
    {
        free((*treeBuffer)->buffer);
        free(*treeBuffer);
    }
}

void writeMinimizedBin(QuadTreeBin tree, FILE *file, QuadTreeBinBuffer *treeBuffer, int index, size_t *treeSize)
{
    int i, j;

    QuadTreeBin children[4];
    int childrenIdentifiers[4] = {0, 0, 0, 0};

    if (tree == NULL)
    {
        return;
    }

    children[0] = tree->northWest;
    children[1] = tree->northEast;
    children[2] = tree->southEast;
    children[3] = tree->southWest;

    if (isBufferedBin(*treeBuffer, tree) >= 0)
        return;

    if (!isLeafBin(tree))
    {
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < i; j++)
            {
                if (children[j] == children[i])
                {
                    childrenIdentifiers[i] = childrenIdentifiers[j];
                    break;
                }
            }
            if (i == 0 || i == j)
            {
                childrenIdentifiers[i] = ++(*treeSize);
            }
        }
        fprintf(file, "%d %d %d %d %d\n", index, childrenIdentifiers[0], childrenIdentifiers[1], childrenIdentifiers[2], childrenIdentifiers[3]);
    }
    else
    {
        fprintf(file, "%d %d\n", index, tree->b);
    }

    offerBinBuffer(treeBuffer, tree);

    writeMinimizedBin(tree->northWest, file, treeBuffer, childrenIdentifiers[0], treeSize);
    writeMinimizedBin(tree->northEast, file, treeBuffer, childrenIdentifiers[1], treeSize);
    writeMinimizedBin(tree->southEast, file, treeBuffer, childrenIdentifiers[2], treeSize);
    writeMinimizedBin(tree->southWest, file, treeBuffer, childrenIdentifiers[3], treeSize);

    if (index == 0)
    {
        free((*treeBuffer)->buffer);
        free(*treeBuffer);
    }
}
