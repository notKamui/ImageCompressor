/* Created by Irwin on 27/10/2020. */
#include <stdlib.h>
#include "../include/quadtree.h"

QuadTreeRGBA allocQuadTreeRGBA(char r, char g, char b, char a) {
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

QuadTreeBin allocQuadTreeBin(char b) {
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

