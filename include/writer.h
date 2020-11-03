/* Created by Irwin on 02/11/2020. */
#include <stdio.h>
#include "quadtree.h"

#ifndef __WRITER_H
#define __WRITER_H

/**
 * Encodes and write down a QuadTreeRGBA in binary.
 * 
 * @param tree Tree to encode.
 * @param file File to write.
 */
void writeRGBA(QuadTreeRGBA tree, FILE *file);

/**
 * Encodes and write down a QuadTreeBin in binary.
 * 
 * @param tree Tree to encode.
 * @param file File to write.
 */
void writeBin(QuadTreeBin tree, FILE *file);

#endif /* __WRITER_H */
