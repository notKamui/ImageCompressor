/* Created by Irwin on 27/10/2020. */

#ifndef __QUADTREE_H
#define __QUADTREE_H

typedef struct s_quadtree_rgba {
    struct s_quadtree_rgba *northWest;
    struct s_quadtree_rgba *northEast;
    struct s_quadtree_rgba *southWest;
    struct s_quadtree_rgba *southEast;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    float a;
} *QuadTreeRGBA;

typedef struct s_quadtree_bin {
    struct s_quadtree_bin *northWest;
    struct s_quadtree_bin *northEast;
    struct s_quadtree_bin *southWest;
    struct s_quadtree_bin *southEast;
    unsigned char b;
} *QuadTreeBin;

/**
 * Checks if a QuadTreeRGBA tree is actually a leaf
 * 
 * @param tree the tree to be checked
 * 
 * @return true if the tree is a leaf
 */
int RGBAIsLeaf(QuadTreeRGBA tree);

/**
 * Checks if a QuadTreeBin tree is actually a leaf
 * 
 * @param tree the tree to be checked
 * 
 * @return true if the tree is a leaf
 */
int BinIsLeaf(QuadTreeBin tree);

QuadTreeRGBA allocQuadTreeRGBA(unsigned char r, unsigned char g, unsigned char b, float a);

QuadTreeBin allocQuadTreeBin(unsigned char b);

#endif /* __QUADTREE_H */
