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

void offerRGBABuffer(QuadTreeRGBABuffer *buffer, QuadTreeRGBA tree)
{
    if ((*buffer)->buffer == NULL)
    {
        if (((*buffer)->buffer = malloc(sizeof(QuadTreeRGBA))) == NULL)
        {
            fprintf(stderr, "FATAL: Allocation error.\n");
            exit(1);
        }
    }
    else
    {
        if (((*buffer)->buffer = realloc((*buffer)->buffer, ((*buffer)->bufferSize + 1) * sizeof(QuadTreeRGBA))) == NULL)
        {
            fprintf(stderr, "FATAL: Allocation error.\n");
            exit(1);
        }
    }
    (*buffer)->buffer[(*buffer)->bufferSize] = tree;
    (*buffer)->bufferSize++;
}

void offerBinBuffer(QuadTreeBinBuffer *buffer, QuadTreeBin tree)
{
    if ((*buffer)->buffer == NULL)
    {
        if (((*buffer)->buffer = malloc(sizeof(QuadTreeBin))) == NULL)
        {
            fprintf(stderr, "FATAL: Allocation error.\n");
            exit(1);
        }
    }
    else
    {
        if (((*buffer)->buffer = realloc((*buffer)->buffer, ((*buffer)->bufferSize + 1) * sizeof(QuadTreeBin))) == NULL)
        {
            fprintf(stderr, "FATAL: Allocation error.\n");
            exit(1);
        }
    }
    (*buffer)->buffer[(*buffer)->bufferSize] = tree;
    (*buffer)->bufferSize++;
}

int isBufferedRGBA(QuadTreeRGBABuffer buffer, QuadTreeRGBA tree)
{
    int i;
    for (i = 0; i < buffer->bufferSize; i++)
    {
        if (buffer->buffer[i] == tree)
            return 1;
    }
    return 0;
}

int isBufferedBin(QuadTreeBinBuffer buffer, QuadTreeBin tree)
{
    int i;
    for (i = 0; i < buffer->bufferSize; i++)
    {
        if (buffer->buffer[i] == tree)
            return 1;
    }
    return 0;
}

void freeQuadTreeRGBA(QuadTreeRGBA tree, QuadTreeRGBABuffer *buffer)
{
    if (!isBufferedRGBA(*buffer, tree))
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
    if (tree != NULL && !isBufferedBin(*buffer, tree))
    {
        freeQuadTreeBin(tree->northWest, buffer);
        freeQuadTreeBin(tree->northEast, buffer);
        freeQuadTreeBin(tree->southEast, buffer);
        freeQuadTreeBin(tree->southWest, buffer);
        offerBinBuffer(buffer, tree);
        free(tree);
    }
}
