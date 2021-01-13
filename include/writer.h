/* Created by Irwin on 02/11/2020. */
#ifndef __WRITER_H
#define __WRITER_H

#include <stdio.h>
#include "quadtree.h"

/**
 * @brief Encodes and write down a QuadTreeRGBA in binary.
 * 
 * @param tree Tree to encode.
 * @param file File to write.
 */
void writeRGBA(QuadTreeRGBA tree, FILE *file);

/**
 * @brief Encodes and write down a QuadTreeBin in binary.
 * 
 * @param tree Tree to encode.
 * @param file File to write.
 */
void writeBin(QuadTreeBin tree, FILE *file);

/**
 * @brief Encodes and write down a minimized QuadTreeRGBA in ASCII.
 * 
 * @param tree Tree to encode.
 * @param file File to write.
 */
void writeMinimizedRGBA(QuadTreeRGBA tree, FILE *file);

/**
 * @brief Encodes and write down a minimized QuadTreeBin in ASCII.
 * 
 * @param tree Tree to encode.
 * @param file File to write.
 */
void writeMinimizedBin(QuadTreeBin tree, FILE *file);

/**
 * @brief Encodes and write down a minimized QuadTreeRGBA in binary.
 * 
 * @param tree Tree to encode.
 * @param file File to write.
 */
void writeMinimizedRGBA2(QuadTreeRGBA tree, FILE *file);

/**
 * @brief Encodes and write down a minimized QuadTreeBin in binary.
 * 
 * @param tree Tree to encode.
 * @param file File to write.
 */
void writeMinimizedBin2(QuadTreeBin tree, FILE *file);

#endif /* __WRITER_H */
