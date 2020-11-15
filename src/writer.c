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
    int i;

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
