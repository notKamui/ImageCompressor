#include <stdio.h>
#include "../include/quadtree.h"
#include "../include/writer.h"

void writeRGBA(QuadTreeRGBA tree, FILE *file)
{
    int i;

    if (tree == NULL)
    {
        return;
    }

    if (tree->northWest || tree->northEast || tree->southEast || tree->southWest)
    {
        fprintf(file, "0");
    }
    else
    {
        fprintf(file, "1");
        for (i = 0; i < 8; i++)
        {
            fprintf(file, "%d", !!((tree->r << i) & 0x80));
        }
        for (i = 0; i < 8; i++)
        {
            fprintf(file, "%d", !!((tree->g << i) & 0x80));
        }
        for (i = 0; i < 8; i++)
        {
            fprintf(file, "%d", !!((tree->b << i) & 0x80));
        }
        for (i = 0; i < 8; i++)
        {
            fprintf(file, "%d", !!((tree->a << i) & 0x80));
        }
    }

    writeRGBA(tree->northWest, file);
    writeRGBA(tree->northEast, file);
    writeRGBA(tree->southEast, file);
    writeRGBA(tree->southWest, file);
}

void writeBin(QuadTreeBin tree, FILE *file)
{
    if (tree == NULL)
    {
        return;
    }

    if (tree->northWest || tree->northEast || tree->southEast || tree->southWest)
    {
        fprintf(file, "0");
    }
    else
    {
        fprintf(file, "1%d", (tree->b > 0));
    }
    
    writeBin(tree->northWest, file);
    writeBin(tree->northEast, file);
    writeBin(tree->southEast, file);
    writeBin(tree->southWest, file);
}
