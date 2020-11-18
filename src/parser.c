#include <stdio.h>
#include "../include/parser.h"

int readBit(FILE *file, unsigned char *bit, unsigned char *buffer, size_t *bufferSize)
{
    if (*bufferSize == 0)
    {
        if ((fscanf(file, "%c", buffer)) < 0)
        {
            *bit = 0;
            return 0;
        }
    }
    *bit = (*buffer >> (7 - *bufferSize)) & 1;
    (*bufferSize) = ((*bufferSize) + 1) % 8;
    return 1;
}

int readByte(FILE *file, unsigned char *byte, unsigned char *buffer, size_t *bufferSize)
{
    unsigned char bit;
    int i, canContinue = 1;

    for (i = 0; i < 8; i++)
    {
        if (readBit(file, &bit, buffer, bufferSize) == 0)
        {
            canContinue = 0;
        }
        *byte = (*byte << 1) | bit;
    }
    return canContinue;
}

int parseRGBA(FILE *file, QuadTreeRGBA tree, unsigned char *buffer, size_t *bufferSize)
{
    unsigned char nodeType = -1;
    int a, b, c, d;

    if ((readBit(file, &nodeType, buffer, bufferSize)) == 0)
    {
        return 0;
    }

    if (nodeType == INNER_NODE)
    {
        tree->northWest = allocQuadTreeRGBA(0, 0, 0, 0);
        a = parseRGBA(file, tree->northWest, buffer, bufferSize);

        tree->northEast = allocQuadTreeRGBA(0, 0, 0, 0);
        b = parseRGBA(file, tree->northEast, buffer, bufferSize);

        tree->southEast = allocQuadTreeRGBA(0, 0, 0, 0);
        c = parseRGBA(file, tree->southEast, buffer, bufferSize);

        tree->southWest = allocQuadTreeRGBA(0, 0, 0, 0);
        d = parseRGBA(file, tree->southWest, buffer, bufferSize);

        return a && b && c && d;
    }
    else if (nodeType == OUTER_NODE)
    {   
        a = readByte(file, &(tree->r), buffer, bufferSize);
        b = readByte(file, &(tree->g), buffer, bufferSize);
        c = readByte(file, &(tree->b), buffer, bufferSize);
        d = readByte(file, &(tree->a), buffer, bufferSize);
        
        return a && b && c && d;
    }
    else
    {
        return 0;
    }
}

int parseBin(FILE *file, QuadTreeBin tree, unsigned char *buffer, size_t *bufferSize)
{
    unsigned char nodeType = -1;
    int nw, ne, se, sw;

    if ((readBit(file, &nodeType, buffer, bufferSize)) == 0)
    {
        return 0;
    }

    if (nodeType == INNER_NODE)
    {
        tree->northWest = allocQuadTreeBin(0);
        nw = parseBin(file, tree->northWest, buffer, bufferSize);

        tree->northEast = allocQuadTreeBin(0);
        ne = parseBin(file, tree->northEast, buffer, bufferSize);

        tree->southEast = allocQuadTreeBin(0);
        se = parseBin(file, tree->southEast, buffer, bufferSize);

        tree->southWest = allocQuadTreeBin(0);
        sw = parseBin(file, tree->southWest, buffer, bufferSize);

        return nw && ne && se && sw;
    }
    else if (nodeType == OUTER_NODE)
    {   
        return readBit(file, &(tree->b), buffer, bufferSize);
    }
    else
    {
        return 0;
    }
}
