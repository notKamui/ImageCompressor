/* Created by Jimmy on 2/11/2020. */
#include "quadtree.h"

#ifndef __COMPRESSOR_H
#define __COMPRESSOR_H

/**
 * Calculates the color distance between two QuadTreeRGBA nodes
 * 
 * @param node1 The first node to compare
 * @param node2 The second node to compare
 * 
 * @return The color distance between the two nodes
 */
int distRGBA(QuadTreeRGBA node1, QuadTreeRGBA node2);

/**
 * Calculates the color distance between two QuadTreeBin nodes
 * 
 * @param node1 The first node to compare
 * @param node2 The second node to compare
 * 
 * @return The color distance between the two nodes
 */
int distBin(QuadTreeBin node1, QuadTreeBin node2);

/**
 * Calculates an average of given QuadTreeRGBA nodes
 * 
 * @param nodes an array of QuadTreeRGBA nodes to average
 * @param size the number of nodes
 * 
 * @return the average of the given nodes
 */
QuadTreeRGBA avgRGBA(QuadTreeRGBA *nodes, int size);

/**
 * Calculates an average of given QuadTreeBin nodes
 * 
 * @param nodes an array of QuadTreeBin nodes to average
 * @param size the number of nodes
 * 
 * @return the average of the given nodes
 */
QuadTreeBin avgBin(QuadTreeBin *nodes, int size);

#endif /* __COMPRESSOR_H */
