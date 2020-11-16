/* Created by Irwin on 27/10/2020. */
#include <stdlib.h>
#include "../include/quadtree.h"

int RGBAIsLeaf(QuadTreeRGBA tree) {
    return  tree->northWest == NULL && 
            tree->southWest == NULL &&
            tree->northEast == NULL &&
            tree->southEast == NULL;
}

int BinIsLeaf(QuadTreeBin tree) {
    return  tree->northWest == NULL && 
            tree->southWest == NULL &&
            tree->northEast == NULL &&
            tree->southEast == NULL;
}

QuadTreeRGBA allocQuadTreeRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    QuadTreeRGBA tree = malloc(sizeof(struct s_quadtree_rgba));

    if (tree) {
        tree->northWest = NULL;
        tree->northEast = NULL;
        tree->southWest = NULL;
        tree->southEast = NULL;
        tree->r = r;
        tree->g = g;
        tree->b = b;
        tree->a = a;
    }

    return tree;
}

QuadTreeBin allocQuadTreeBin(unsigned char b) {
    QuadTreeBin tree = malloc(sizeof(struct s_quadtree_bin));

    if (tree) {
        tree->northWest = NULL;
        tree->northEast = NULL;
        tree->southWest = NULL;
        tree->southEast = NULL;
        tree->b = b;
    }

    return tree;
}

void freeQuadTreeRGBA(QuadTreeRGBA tree)
{
    if (tree != NULL)
    {
        freeQuadTreeRGBA(tree->northWest);
        freeQuadTreeRGBA(tree->northEast);
        freeQuadTreeRGBA(tree->southEast);
        freeQuadTreeRGBA(tree->southWest);
        free(tree);
    }
}

void freeQuadTreeBin(QuadTreeBin tree)
{
    if (tree != NULL)
    {
        freeQuadTreeBin(tree->northWest);
        freeQuadTreeBin(tree->northEast);
        freeQuadTreeBin(tree->southEast);
        freeQuadTreeBin(tree->southWest);
        free(tree);
    }
}
