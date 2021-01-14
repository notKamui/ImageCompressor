#include <stdio.h>
#include <string.h>
#include "../include/parser.h"

#define RGBA_LINE_INNER_NODE 0
#define RGBA_LINE_OUTER_NODE 1

#define BIN_LINE_INNER_NODE 0
#define BIN_LINE_OUTER_NODE 1

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

int readNumber(FILE *file, long *number, unsigned char len, unsigned char *buffer, size_t *bufferSize)
{
    unsigned char bit;
    int i, canContinue = 1;

    for (i = 0; i < len; i++)
    {
        if (readBit(file, &bit, buffer, bufferSize) == 0)
        {
            canContinue = 0;
        }
        *number = (*number << 1) | bit;
    }
    return canContinue;
}

int readByte(FILE *file, unsigned char *byte, unsigned char *buffer, size_t *bufferSize)
{
    long read = 0;
    int res = readNumber(file, &read, 8, buffer, bufferSize);
    *byte = (unsigned char)read;
    return res;
}

char *readLine(FILE *file)
{
    char *line = malloc(0), c;
    size_t len = 0;

    while ((c = getc(file)) != '\n')
    {
        line = realloc(line, len + 1);
        line[len++] = c;
    }
    line = realloc(line, len + 1);
    line[len++] = '\0';
    return line;
}

int decodeRGBA(FILE *file, QuadTreeRGBA tree, unsigned char *buffer, size_t *bufferSize)
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
        a = decodeRGBA(file, tree->northWest, buffer, bufferSize);

        tree->northEast = allocQuadTreeRGBA(0, 0, 0, 0);
        b = decodeRGBA(file, tree->northEast, buffer, bufferSize);

        tree->southEast = allocQuadTreeRGBA(0, 0, 0, 0);
        c = decodeRGBA(file, tree->southEast, buffer, bufferSize);

        tree->southWest = allocQuadTreeRGBA(0, 0, 0, 0);
        d = decodeRGBA(file, tree->southWest, buffer, bufferSize);

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

int decodeBin(FILE *file, QuadTreeBin tree, unsigned char *buffer, size_t *bufferSize)
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
        nw = decodeBin(file, tree->northWest, buffer, bufferSize);

        tree->northEast = allocQuadTreeBin(0);
        ne = decodeBin(file, tree->northEast, buffer, bufferSize);

        tree->southEast = allocQuadTreeBin(0);
        se = decodeBin(file, tree->southEast, buffer, bufferSize);

        tree->southWest = allocQuadTreeBin(0);
        sw = decodeBin(file, tree->southWest, buffer, bufferSize);

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

int parseRGBA(FILE *file, QuadTreeRGBA tree)
{
    unsigned char buffer = 0;
    size_t bufferSize = 0;
    return decodeRGBA(file, tree, &buffer, &bufferSize);
}

int parseBin(FILE *file, QuadTreeBin tree)
{
    unsigned char buffer = 0;
    size_t bufferSize = 0;
    return decodeBin(file, tree, &buffer, &bufferSize);
}

int getLineKindRGBA(const char *line, size_t length)
{
    int i, token = 0;
    for (i = 0; i < length; i++)
    {
        if (line[i] == 'f')
            token++;
    }
    return token;
}

int getLineKindBin(const char *line, size_t length)
{
    int i, token = 0;
    for (i = 0; i < length; i++)
    {
        if (line[i] == ' ')
            token++;
    }
    return token > 1 ? 0 : 1;
}

int decodeMinimizedRGBA(FILE *file, int index, QuadTreeRGBA tree, QuadTreeRGBABuffer *buffer)
{
    int kind;

    int readIndex;
    int nw = 0, ne = 0, se = 0, sw = 0;
    int r, g, b, a;

    char *line;

    if (isBufferedRGBA(*buffer, tree) >= 0)
        return 0;

    setRGBABuffer(buffer, tree, index);

    line = readLine(file);
    kind = getLineKindRGBA(line, strlen(line));

    if (kind == RGBA_LINE_INNER_NODE)
    {
        sscanf(line, "%d %d %d %d %d", &readIndex, &nw, &ne, &se, &sw);
        free(line);

        if ((*buffer)->bufferSize <= nw || (*buffer)->buffer[nw] == NULL)
        {
            tree->northWest = allocQuadTreeRGBA(0, 0, 0, 0);
            nw = decodeMinimizedRGBA(file, nw, tree->northWest, buffer);
        }
        else
        {
            tree->northWest = (*buffer)->buffer[nw];
            nw = 1;
        }

        if ((*buffer)->bufferSize <= ne || (*buffer)->buffer[ne] == NULL)
        {
            tree->northEast = allocQuadTreeRGBA(0, 0, 0, 0);
            ne = decodeMinimizedRGBA(file, ne, tree->northEast, buffer);
        }
        else
        {
            tree->northEast = (*buffer)->buffer[ne];
            ne = 1;
        }

        if ((*buffer)->bufferSize <= se || (*buffer)->buffer[se] == NULL)
        {
            tree->southEast = allocQuadTreeRGBA(0, 0, 0, 0);
            se = decodeMinimizedRGBA(file, se, tree->southEast, buffer);
        }
        else
        {
            tree->southEast = (*buffer)->buffer[se];
            se = 1;
        }

        if ((*buffer)->bufferSize <= sw || (*buffer)->buffer[sw] == NULL)
        {
            tree->southWest = allocQuadTreeRGBA(0, 0, 0, 0);
            sw = decodeMinimizedRGBA(file, sw, tree->southWest, buffer);
        }
        else
        {
            tree->southWest = (*buffer)->buffer[sw];
            sw = 1;
        }
        return nw && ne && se && sw;
    }
    else if (kind == RGBA_LINE_OUTER_NODE)
    {
        sscanf(line, "%df %d %d %d %d", &readIndex, &r, &g, &b, &a);
        free(line);
        tree->r = r;
        tree->g = g;
        tree->b = b;
        tree->a = a;
        return 1;
    }

    return 0;
}

int decodeMinimizedBin(FILE *file, int index, QuadTreeBin tree, QuadTreeBinBuffer *buffer)
{
    int kind;

    int readIndex;
    int nw, ne, se, sw;
    int b;

    char *line;

    if (isBufferedBin(*buffer, tree) >= 0)
        return 0;

    setBinBuffer(buffer, tree, index);

    line = readLine(file);
    kind = getLineKindBin(line, strlen(line));

    if (kind == BIN_LINE_INNER_NODE)
    {
        sscanf(line, "%d %d %d %d %d", &readIndex, &nw, &ne, &se, &sw);
        free(line);

        if ((*buffer)->bufferSize <= nw || (*buffer)->buffer[nw] == NULL)
        {
            tree->northWest = allocQuadTreeBin(0);
            nw = decodeMinimizedBin(file, nw, tree->northWest, buffer);
        }
        else
        {
            tree->northWest = (*buffer)->buffer[nw];
            nw = 1;
        }

        if ((*buffer)->bufferSize <= ne || (*buffer)->buffer[ne] == NULL)
        {
            tree->northEast = allocQuadTreeBin(0);
            ne = decodeMinimizedBin(file, ne, tree->northEast, buffer);
        }
        else
        {
            tree->northEast = (*buffer)->buffer[ne];
            ne = 1;
        }

        if ((*buffer)->bufferSize <= se || (*buffer)->buffer[se] == NULL)
        {
            tree->southEast = allocQuadTreeBin(0);
            se = decodeMinimizedBin(file, se, tree->southEast, buffer);
        }
        else
        {
            tree->southEast = (*buffer)->buffer[se];
            se = 1;
        }

        if ((*buffer)->bufferSize <= sw || (*buffer)->buffer[sw] == NULL)
        {
            tree->southWest = allocQuadTreeBin(0);
            sw = decodeMinimizedBin(file, sw, tree->southWest, buffer);
        }
        else
        {
            tree->southWest = (*buffer)->buffer[sw];
            sw = 1;
        }
        return nw && ne && se && sw;
    }
    else if (kind == BIN_LINE_OUTER_NODE)
    {
        sscanf(line, "%d %d", &readIndex, &b);
        free(line);
        tree->b = b;
        return 1;
    }

    return 0;
}

int parseMinimizedRGBA(FILE *file, QuadTreeRGBA tree)
{
    QuadTreeRGBABuffer buffer = allocQuadTreeRGBABuffer();
    int res = decodeMinimizedRGBA(file, 0, tree, &buffer);
    freeRGBABuffer(buffer);
    return res;
}

int parseMinimizedBin(FILE *file, QuadTreeBin tree)
{
    QuadTreeBinBuffer buffer = allocQuadTreeBinBuffer();
    int res = decodeMinimizedBin(file, 0, tree, &buffer);
    freeBinBuffer(buffer);
    return res;
}

int decodeMinimizedRGBA2(FILE *file, int index, unsigned char id_len, QuadTreeRGBA tree, QuadTreeRGBABuffer *treeBuffer, unsigned char *buffer, size_t *bufferSize)
{
    long readIndex = 0;
    unsigned char kind = 0;
    long nw = 0, ne = 0, se = 0, sw = 0;

    if (isBufferedRGBA(*treeBuffer, tree) >= 0)
        return 0;

    setRGBABuffer(treeBuffer, tree, index);

    readNumber(file, &readIndex, id_len, buffer, bufferSize);
    readBit(file, &kind, buffer, bufferSize);

    if (kind == RGBA_LINE_INNER_NODE)
    {
        readNumber(file, &nw, id_len, buffer, bufferSize);
        readNumber(file, &ne, id_len, buffer, bufferSize);
        readNumber(file, &se, id_len, buffer, bufferSize);
        readNumber(file, &sw, id_len, buffer, bufferSize);

        if ((*treeBuffer)->bufferSize <= nw || (*treeBuffer)->buffer[nw] == NULL)
        {
            tree->northWest = allocQuadTreeRGBA(0, 0, 0, 0);
            nw = decodeMinimizedRGBA2(file, nw, id_len, tree->northWest, treeBuffer, buffer, bufferSize);
        }
        else
        {
            tree->northWest = (*treeBuffer)->buffer[nw];
            nw = 1;
        }

        if ((*treeBuffer)->bufferSize <= ne || (*treeBuffer)->buffer[ne] == NULL)
        {
            tree->northEast = allocQuadTreeRGBA(0, 0, 0, 0);
            ne = decodeMinimizedRGBA2(file, ne, id_len, tree->northEast, treeBuffer, buffer, bufferSize);
        }
        else
        {
            tree->northEast = (*treeBuffer)->buffer[ne];
            ne = 1;
        }

        if ((*treeBuffer)->bufferSize <= se || (*treeBuffer)->buffer[se] == NULL)
        {
            tree->southEast = allocQuadTreeRGBA(0, 0, 0, 0);
            se = decodeMinimizedRGBA2(file, se, id_len, tree->southEast, treeBuffer, buffer, bufferSize);
        }
        else
        {
            tree->southEast = (*treeBuffer)->buffer[se];
            se = 1;
        }

        if ((*treeBuffer)->bufferSize <= sw || (*treeBuffer)->buffer[sw] == NULL)
        {
            tree->southWest = allocQuadTreeRGBA(0, 0, 0, 0);
            sw = decodeMinimizedRGBA2(file, sw, id_len, tree->southWest, treeBuffer, buffer, bufferSize);
        }
        else
        {
            tree->southWest = (*treeBuffer)->buffer[sw];
            sw = 1;
        }
        return nw && ne && se && sw;
    }
    else if (kind == RGBA_LINE_OUTER_NODE)
    {
        readByte(file, &tree->r, buffer, bufferSize);
        readByte(file, &tree->g, buffer, bufferSize);
        readByte(file, &tree->b, buffer, bufferSize);
        readByte(file, &tree->a, buffer, bufferSize);
        return 1;
    }

    return 0;
}

int decodeMinimizedBin2(FILE *file, int index, unsigned char id_len, QuadTreeBin tree, QuadTreeBinBuffer *treeBuffer, unsigned char *buffer, size_t *bufferSize)
{
    long readIndex = 0;
    unsigned char kind = 0;
    long nw = 0, ne = 0, se = 0, sw = 0;

    if (isBufferedBin(*treeBuffer, tree) >= 0)
        return 0;

    setBinBuffer(treeBuffer, tree, index);

    readNumber(file, &readIndex, id_len, buffer, bufferSize);
    readBit(file, &kind, buffer, bufferSize);

    if (kind == BIN_LINE_INNER_NODE)
    {
        readNumber(file, &nw, id_len, buffer, bufferSize);
        readNumber(file, &ne, id_len, buffer, bufferSize);
        readNumber(file, &se, id_len, buffer, bufferSize);
        readNumber(file, &sw, id_len, buffer, bufferSize);

        if ((*treeBuffer)->bufferSize <= nw || (*treeBuffer)->buffer[nw] == NULL)
        {
            tree->northWest = allocQuadTreeBin(0);
            nw = decodeMinimizedBin2(file, nw, id_len, tree->northWest, treeBuffer, buffer, bufferSize);
        }
        else
        {
            tree->northWest = (*treeBuffer)->buffer[nw];
            nw = 1;
        }

        if ((*treeBuffer)->bufferSize <= ne || (*treeBuffer)->buffer[ne] == NULL)
        {
            tree->northEast = allocQuadTreeBin(0);
            ne = decodeMinimizedBin2(file, ne, id_len, tree->northEast, treeBuffer, buffer, bufferSize);
        }
        else
        {
            tree->northEast = (*treeBuffer)->buffer[ne];
            ne = 1;
        }

        if ((*treeBuffer)->bufferSize <= se || (*treeBuffer)->buffer[se] == NULL)
        {
            tree->southEast = allocQuadTreeBin(0);
            se = decodeMinimizedBin2(file, se, id_len, tree->southEast, treeBuffer, buffer, bufferSize);
        }
        else
        {
            tree->southEast = (*treeBuffer)->buffer[se];
            se = 1;
        }

        if ((*treeBuffer)->bufferSize <= sw || (*treeBuffer)->buffer[sw] == NULL)
        {
            tree->southWest = allocQuadTreeBin(0);
            sw = decodeMinimizedBin2(file, sw, id_len, tree->southWest, treeBuffer, buffer, bufferSize);
        }
        else
        {
            tree->southWest = (*treeBuffer)->buffer[sw];
            sw = 1;
        }
        return nw && ne && se && sw;
    }
    else if (kind == BIN_LINE_OUTER_NODE)
    {
        readBit(file, &tree->b, buffer, bufferSize);
        return 1;
    }

    return 0;
}

int parseMinimizedRGBA2(FILE *file, QuadTreeRGBA tree)
{
    unsigned char id_len = 0, buffer = 0, res;
    size_t bufferSize = 0;
    QuadTreeRGBABuffer treeBuffer = allocQuadTreeRGBABuffer();

    readByte(file, &id_len, &buffer, &bufferSize);
    res = decodeMinimizedRGBA2(file, 0, id_len, tree, &treeBuffer, &buffer, &bufferSize);
    free(treeBuffer->buffer);
    free(treeBuffer);
    return res;
}

int parseMinimizedBin2(FILE *file, QuadTreeBin tree)
{
    unsigned char id_len = 0, buffer = 0, res;
    size_t bufferSize = 0;
    QuadTreeBinBuffer treeBuffer = allocQuadTreeBinBuffer();

    readByte(file, &id_len, &buffer, &bufferSize);
    res = decodeMinimizedBin2(file, 0, id_len, tree, &treeBuffer, &buffer, &bufferSize);
    free(treeBuffer->buffer);
    free(treeBuffer);
    return res;
}
