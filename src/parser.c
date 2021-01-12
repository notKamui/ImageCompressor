#include <stdio.h>
#include <string.h>
#include "../include/parser.h"

#define RGBA_LINE_INNER_NODE 0
#define RGBA_LINE_OUTER_NODE 1

#define BIN_LINE_INNER_NODE 4
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
    return token;
}

int parseMinimizedRGBA(FILE *file, int index, QuadTreeRGBA tree, QuadTreeRGBABuffer *buffer)
{
    int kind;

    int readIndex;
    int nw, ne, se, sw;
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
            nw = parseMinimizedRGBA(file, nw, tree->northWest, buffer);
        }
        else
        {
            tree->northWest = (*buffer)->buffer[nw];
            nw = 1;
        }

        if ((*buffer)->bufferSize <= ne || (*buffer)->buffer[ne] == NULL)
        {
            tree->northEast = allocQuadTreeRGBA(0, 0, 0, 0);
            ne = parseMinimizedRGBA(file, ne, tree->northEast, buffer);
        }
        else
        {
            tree->northEast = (*buffer)->buffer[ne];
            ne = 1;
        }

        if ((*buffer)->bufferSize <= se || (*buffer)->buffer[se] == NULL)
        {
            tree->southEast = allocQuadTreeRGBA(0, 0, 0, 0);
            se = parseMinimizedRGBA(file, se, tree->southEast, buffer);
        }
        else
        {
            tree->southEast = (*buffer)->buffer[se];
            se = 1;
        }

        if ((*buffer)->bufferSize <= sw || (*buffer)->buffer[sw] == NULL)
        {
            tree->southWest = allocQuadTreeRGBA(0, 0, 0, 0);
            sw = parseMinimizedRGBA(file, sw, tree->southWest, buffer);
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

int parseMinimizedBin(FILE *file, int index, QuadTreeBin tree, QuadTreeBinBuffer *buffer)
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
            nw = parseMinimizedBin(file, nw, tree->northWest, buffer);
        }
        else
        {
            tree->northWest = (*buffer)->buffer[nw];
            nw = 1;
        }

        if ((*buffer)->bufferSize <= ne || (*buffer)->buffer[ne] == NULL)
        {
            tree->northEast = allocQuadTreeBin(0);
            ne = parseMinimizedBin(file, ne, tree->northEast, buffer);
        }
        else
        {
            tree->northEast = (*buffer)->buffer[ne];
            ne = 1;
        }

        if ((*buffer)->bufferSize <= se || (*buffer)->buffer[se] == NULL)
        {
            tree->southEast = allocQuadTreeBin(0);
            se = parseMinimizedBin(file, se, tree->southEast, buffer);
        }
        else
        {
            tree->southEast = (*buffer)->buffer[se];
            se = 1;
        }

        if ((*buffer)->bufferSize <= sw || (*buffer)->buffer[sw] == NULL)
        {
            tree->southWest = allocQuadTreeBin(0);
            sw = parseMinimizedBin(file, sw, tree->southWest, buffer);
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
