/* Created by Irwin on 02/11/2020. */
#include <stdio.h>
#include "quadtree.h"

#ifndef __WRITER_H
#define __WRITER_H

/**
 * @brief Write zeros to complete the last byte of the file.
 * 
 * @param file Current file.
 * @param buffer Current buffer.
 * @param bufferSize Buffer size.
 */
void flush(FILE *file, unsigned char *buffer, size_t *bufferSize);

/**
 * Encodes and write down a QuadTreeRGBA in binary.
 * 
 * @param tree Tree to encode.
 * @param file File to write.
 * @param buffer Current buffer.
 * @param bufferSize Buffer size.
 */
void writeRGBA(QuadTreeRGBA tree, FILE *file, unsigned char *buffer, size_t *bufferSize);

/**
 * Encodes and write down a QuadTreeBin in binary.
 * 
 * @param tree Tree to encode.
 * @param file File to write.
 * @param buffer Current buffer.
 * @param bufferSize Buffer size.
 */
void writeBin(QuadTreeBin tree, FILE *file, unsigned char *buffer, size_t *bufferSize);

#endif /* __WRITER_H */
