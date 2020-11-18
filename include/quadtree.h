/* Created by Irwin on 27/10/2020. */

#ifndef __QUADTREE_H
#define __QUADTREE_H

#define INNER_NODE 0
#define OUTER_NODE 1

typedef struct s_quadtree_rgba {
    struct s_quadtree_rgba *northWest;
    struct s_quadtree_rgba *northEast;
    struct s_quadtree_rgba *southWest;
    struct s_quadtree_rgba *southEast;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
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
 
/** Allocates a QuadTreeRGBA node.
 * 
 * @param r Red value between 0 and 255.
 * @param g Green value between 0 and 255.
 * @param b Blue value between 0 and 255.
 * @param a Alpha value between 0 and 255.
 * 
 * @return The new node.
 */
QuadTreeRGBA allocQuadTreeRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

/**
 * Allocates a QuadTreeBin node.
 * 
 * @param b Binary value among 0 and 1.
 * 
 * @return The new node.
 */
QuadTreeBin allocQuadTreeBin(unsigned char b);

/**
 * @brief Free memory allocated by the given QuadTreeRGBA.
 */
void freeQuadTreeRGBA(QuadTreeRGBA tree);

/**
 * @brief Free memory allocated by the given QuadTreeBin.
 */
void freeQuadTreeBin(QuadTreeBin tree);

#endif /* __QUADTREE_H */
