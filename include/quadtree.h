/* Created by Irwin on 27/10/2020. */

#ifndef __QUADTREE_H
#define __QUADTREE_H

typedef struct s_quadtree_rgba {
    struct s_quadtree_rgba *northWest;
    struct s_quadtree_rgba *northEast;
    struct s_quadtree_rgba *southWest;
    struct s_quadtree_rgba *southEast;
    char r;
    char g;
    char b;
    char a;
} *QuadTreeRGBA;

typedef struct s_quadtree_bin {
    struct s_quadtree_bin *northWest;
    struct s_quadtree_bin *northEast;
    struct s_quadtree_bin *southWest;
    struct s_quadtree_bin *southEast;
    char b;
} *QuadTreeBin;

QuadTreeRGBA allocQuadTreeRGBA(char r, char g, char b, char a);

QuadTreeBin allocQuadTreeBin(char b);

#endif /* __QUADTREE_H */
