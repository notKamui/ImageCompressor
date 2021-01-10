/* Created by Irwin on 18/11/2020. */
#ifndef __PARSER_H
#define __PARSER_H

#include <stdio.h>
#include "quadtree.h"

/**
 * @brief Read a single bit from a file. Reads 0 if the bit doesn't exists.
 * 
 * @param file File to read.
 * @param bit Read bit.
 * @param buffer Current buffer.
 * @param bufferSize Buffer size.
 * 
 * @return Whether the reading succeeded.
 */
int readBit(FILE *file, unsigned char *bit, unsigned char *buffer, size_t *bufferSize);

/**
 * @brief Read an entire byte from a file. This take into account the current buffer state.
 * 
 * @param file File to read.
 * @param byte Read byte.
 * @param buffer Current buffer.
 * @param bufferSize Buffer size.
 * 
 * @return Whether the reading succeeded.
 */
int readByte(FILE *file, unsigned char *byte, unsigned char *buffer, size_t *bufferSize);

/**
 * @brief Build a non-compressed QuadTreeRGBA from a given file.
 * 
 * @param file File to read.
 * @param tree Built tree.
 * @param buffer Current buffer.
 * @param bufferSize Buffer size.
 * 
 * @return Whether the build succeeded. 
 */
int parseRGBA(FILE *file, QuadTreeRGBA tree, unsigned char *buffer, size_t *bufferSize);

/**
 * @brief Build a non-compressed QuadTreeBin from a given file.
 * 
 * @param file File to read.
 * @param tree Built tree.
 * @param buffer Current buffer.
 * @param bufferSize Buffer size.
 * 
 * @return Whether the build succeeded. 
 */
int parseBin(FILE *file, QuadTreeBin tree, unsigned char *buffer, size_t *bufferSize);

/**
 * @brief Build a minimized QuadTreeRGBA from a given file.
 * 
 * @param file File to read.
 * @param index Index of the current node. Should be 0 when function is called.
 * @param tree Built tree.
 * @param buffer Tree buffer.
 * 
 * @return Whether the build succeeded.
 */
int parseMinimizedRGBA(FILE *file, int index, QuadTreeRGBA tree, QuadTreeRGBABuffer *buffer);

/**
 * @brief Build a minimized QuadTreeBin from a given file.
 * 
 * @param file File to read.
 * @param index Index of the current node. Should be 0 when function is called.
 * @param tree Built tree.
 * @param buffer Tree buffer.
 * 
 * @return Whether the build succeeded.
 */
int parseMinimizedBin(FILE *file, int index, QuadTreeBin tree, QuadTreeBinBuffer *buffer);

#endif /* __PARSER_H */
