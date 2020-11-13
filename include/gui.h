/* Created by Irwin on 14/11/2020 */
#ifndef __GUI_H
#define __GUI_H

#include <MLV/MLV_all.h>
#include "quadtree.h"

/**
 * @brief Draw a RGBAQuadTree recursively at the given coordinates.
 * 
 * @param tree RGBAQuadTree to draw.
 * @param width Width of the draw.
 * @param height Height of the draw.
 */
void drawQuadTreeRGBA(QuadTreeRGBA tree, int x, int y, int width, int height);

/**
 * @brief Draw a BinQuadTree recursively at the given coordinates.
 * 
 * @param tree BinQuadTree to draw.
 * @param width Width of the draw.
 * @param height Height of the draw.
 */
void drawQuadTreeBin(QuadTreeBin tree, int x, int y, int width, int height);

#endif /* __GUI_H */
