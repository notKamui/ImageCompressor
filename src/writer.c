#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>
#include "../include/quadtree.h"
#include "../include/writer.h"
#include "../include/visualizer.h"

#define MAXIMUM_IDENTIFIERS_LENGTH sizeof(long) * 8 /* Node ids should not excess 64 bits (long type) */

void writeBit(FILE *file, unsigned char bit, unsigned char *buffer, size_t *bufferSize)
{
    *buffer = (*buffer << 1) | bit;
    (*bufferSize)++;

    if (*bufferSize == 8)
    {
        putc(*buffer, file);
        *bufferSize = 0;
        *buffer = 0;
    }
}

void writeNumber(FILE *file, long number, unsigned char len, unsigned char *buffer, size_t *bufferSize)
{
    int i;
    assert(len <= MAXIMUM_IDENTIFIERS_LENGTH);

    for (i = len - 1; i >= 0; i--)
    {
        writeBit(file, (number >> i) & 1, buffer, bufferSize);
    }
}

void writeByte(FILE *file, unsigned char byte, unsigned char *buffer, size_t *bufferSize)
{
    writeNumber(file, byte, 8, buffer, bufferSize);
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

void encodeRGBA(QuadTreeRGBA tree, FILE *file, unsigned char *buffer, size_t *bufferSize)
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

    encodeRGBA(tree->northWest, file, buffer, bufferSize);
    encodeRGBA(tree->northEast, file, buffer, bufferSize);
    encodeRGBA(tree->southEast, file, buffer, bufferSize);
    encodeRGBA(tree->southWest, file, buffer, bufferSize);
}

void encodeBin(QuadTreeBin tree, FILE *file, unsigned char *buffer, size_t *bufferSize)
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

    encodeBin(tree->northWest, file, buffer, bufferSize);
    encodeBin(tree->northEast, file, buffer, bufferSize);
    encodeBin(tree->southEast, file, buffer, bufferSize);
    encodeBin(tree->southWest, file, buffer, bufferSize);
}

void writeRGBA(QuadTreeRGBA tree, FILE *file)
{
    unsigned char buffer = 0;
    size_t bufferSize = 0;
    encodeRGBA(tree, file, &buffer, &bufferSize);
}

void writeBin(QuadTreeBin tree, FILE *file)
{
    unsigned char buffer = 0;
    size_t bufferSize = 0;
    encodeBin(tree, file, &buffer, &bufferSize);
}

void encodeMinimizedRGBA(QuadTreeRGBA tree, FILE *file, QuadTreeRGBABuffer *treeBuffer, int index, size_t *treeSize)
{
    int i, j, k;

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

    k = isBufferedRGBA(*treeBuffer, tree);
    /* if node is buffered but its index is lower than current one, must continue */
    if (k >= 0 && k <= index)
    {
        return;
    }

    setRGBABuffer(treeBuffer, tree, index);

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
            if ((k = isBufferedRGBA(*treeBuffer, children[i])) >= 0)
            {
                childrenIdentifiers[i] = k;
            }
            else if (i == j)
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

    encodeMinimizedRGBA(tree->northWest, file, treeBuffer, childrenIdentifiers[0], treeSize);
    encodeMinimizedRGBA(tree->northEast, file, treeBuffer, childrenIdentifiers[1], treeSize);
    encodeMinimizedRGBA(tree->southEast, file, treeBuffer, childrenIdentifiers[2], treeSize);
    encodeMinimizedRGBA(tree->southWest, file, treeBuffer, childrenIdentifiers[3], treeSize);
}

void encodeMinimizedBin(QuadTreeBin tree, FILE *file, QuadTreeBinBuffer *treeBuffer, int index, size_t *treeSize)
{
    int i, j, k;

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

    k = isBufferedBin(*treeBuffer, tree);
    /* if node is buffered but its index is lower than current one, must continue */
    if (k >= 0 && k <= index)
    {
        return;
    }

    setBinBuffer(treeBuffer, tree, index);

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
            if ((k = isBufferedBin(*treeBuffer, children[i])) >= 0)
            {
                childrenIdentifiers[i] = k;
            }
            else if (i == j)
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

    encodeMinimizedBin(tree->northWest, file, treeBuffer, childrenIdentifiers[0], treeSize);
    encodeMinimizedBin(tree->northEast, file, treeBuffer, childrenIdentifiers[1], treeSize);
    encodeMinimizedBin(tree->southEast, file, treeBuffer, childrenIdentifiers[2], treeSize);
    encodeMinimizedBin(tree->southWest, file, treeBuffer, childrenIdentifiers[3], treeSize);
}

void writeMinimizedRGBA(QuadTreeRGBA tree, FILE *file)
{
    size_t treesize = 0;
    QuadTreeRGBABuffer buffer = allocQuadTreeRGBABuffer();
    encodeMinimizedRGBA(tree, file, &buffer, 0, &treesize);
}

void writeMinimizedBin(QuadTreeBin tree, FILE *file)
{
    size_t treesize = 0;
    QuadTreeRGBABuffer buffer = allocQuadTreeBinBuffer();
    encodeMinimizedBin(tree, file, &buffer, 0, &treesize);
}

void encodeMinimizedRGBA2(QuadTreeRGBA tree, FILE *file, QuadTreeRGBABuffer *treeBuffer, long index, unsigned char id_len, size_t *treeSize, unsigned char *buffer, size_t *bufferSize)
{
    int i, j, k;

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

    k = isBufferedRGBA(*treeBuffer, tree);
    /* if node is buffered but its index is lower than current one, must continue */
    if (k >= 0 && k <= index)
    {
        return;
    }

    setRGBABuffer(treeBuffer, tree, index);

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
            if ((k = isBufferedRGBA(*treeBuffer, children[i])) >= 0)
            {
                childrenIdentifiers[i] = k;
            }
            else if (i == j)
            {
                childrenIdentifiers[i] = ++(*treeSize);
            }
        }
        writeNumber(file, index, id_len, buffer, bufferSize);
        writeBit(file, 0, buffer, bufferSize);
        for (i = 0; i < 4; i++)
            writeNumber(file, childrenIdentifiers[i], id_len, buffer, bufferSize);
    }
    else
    {
        writeNumber(file, index, id_len, buffer, bufferSize);
        writeBit(file, 1, buffer, bufferSize);
        writeByte(file, tree->r, buffer, bufferSize);
        writeByte(file, tree->g, buffer, bufferSize);
        writeByte(file, tree->b, buffer, bufferSize);
        writeByte(file, tree->a, buffer, bufferSize);
    }

    encodeMinimizedRGBA2(tree->northWest, file, treeBuffer, childrenIdentifiers[0], id_len, treeSize, buffer, bufferSize);
    encodeMinimizedRGBA2(tree->northEast, file, treeBuffer, childrenIdentifiers[1], id_len, treeSize, buffer, bufferSize);
    encodeMinimizedRGBA2(tree->southEast, file, treeBuffer, childrenIdentifiers[2], id_len, treeSize, buffer, bufferSize);
    encodeMinimizedRGBA2(tree->southWest, file, treeBuffer, childrenIdentifiers[3], id_len, treeSize, buffer, bufferSize);
}

void encodeMinimizedBin2(QuadTreeBin tree, FILE *file, QuadTreeBinBuffer *treeBuffer, long index, unsigned char id_len, size_t *treeSize, unsigned char *buffer, size_t *bufferSize)
{
    int i, j, k;

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

    k = isBufferedBin(*treeBuffer, tree);
    /* if node is buffered but its index is lower than current one, must continue */
    if (k >= 0 && k <= index)
    {
        return;
    }

    setBinBuffer(treeBuffer, tree, index);

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
            if ((k = isBufferedBin(*treeBuffer, children[i])) >= 0)
            {
                childrenIdentifiers[i] = k;
            }
            else if (i == j)
            {
                childrenIdentifiers[i] = ++(*treeSize);
            }
        }
        writeNumber(file, index, id_len, buffer, bufferSize);
        writeBit(file, 0, buffer, bufferSize);
        for (i = 0; i < 4; i++)
            writeNumber(file, childrenIdentifiers[i], id_len, buffer, bufferSize);
    }
    else
    {
        writeNumber(file, index, id_len, buffer, bufferSize);
        writeBit(file, 1, buffer, bufferSize);
        writeBit(file, tree->b, buffer, bufferSize);
    }

    encodeMinimizedBin2(tree->northWest, file, treeBuffer, childrenIdentifiers[0], id_len, treeSize, buffer, bufferSize);
    encodeMinimizedBin2(tree->northEast, file, treeBuffer, childrenIdentifiers[1], id_len, treeSize, buffer, bufferSize);
    encodeMinimizedBin2(tree->southEast, file, treeBuffer, childrenIdentifiers[2], id_len, treeSize, buffer, bufferSize);
    encodeMinimizedBin2(tree->southWest, file, treeBuffer, childrenIdentifiers[3], id_len, treeSize, buffer, bufferSize);
}

void writeMinimizedRGBA2(QuadTreeRGBA tree, FILE *file)
{
    unsigned char buffer = 0;
    unsigned char ids_len = 0;
    size_t bufferSize = 0;
    size_t treeSize;
    QuadTreeRGBABuffer treebuffer = allocQuadTreeRGBABuffer();

    treeSize = sizeQuadTreeRGBA(tree, &treebuffer);
    free(treebuffer->buffer);
    free(treebuffer);
    ids_len = ceil(log(treeSize) / log(2));
    treeSize = 0;

    if (ids_len > MAXIMUM_IDENTIFIERS_LENGTH)
    {
        fprintf(stderr, "Tree exceeds maximal node identifiers size. Cannot continue.\n");
        return;
    }

    treebuffer = allocQuadTreeRGBABuffer();
    writeByte(file, ids_len, &buffer, &bufferSize);
    encodeMinimizedRGBA2(tree, file, &treebuffer, 0, ids_len, &treeSize, &buffer, &bufferSize);
    free(treebuffer->buffer);
    free(treebuffer);
    flush(file, &buffer, &bufferSize);
}

void writeMinimizedBin2(QuadTreeBin tree, FILE *file)
{
    unsigned char buffer = 0;
    unsigned char ids_len = 0;
    size_t bufferSize = 0;
    size_t treeSize;
    QuadTreeBinBuffer treebuffer = allocQuadTreeBinBuffer();

    treeSize = sizeQuadTreeBin(tree, &treebuffer);
    free(treebuffer->buffer);
    free(treebuffer);
    ids_len = ceil(log(treeSize) / log(2));
    treeSize = 0;

    if (ids_len > MAXIMUM_IDENTIFIERS_LENGTH)
    {
        fprintf(stderr, "Tree exceeds maximal node identifiers size. Cannot continue.\n");
        return;
    }

    treebuffer = allocQuadTreeBinBuffer();
    writeByte(file, ids_len, &buffer, &bufferSize);
    encodeMinimizedBin2(tree, file, &treebuffer, 0, ids_len, &treeSize, &buffer, &bufferSize);
    free(treebuffer->buffer);
    free(treebuffer);
    flush(file, &buffer, &bufferSize);
}
