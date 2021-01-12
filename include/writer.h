/* Created by Irwin on 02/11/2020. */
#ifndef __WRITER_H
#define __WRITER_H

#include <stdio.h>
#include "quadtree.h"

/**
 * @brief Write zeros to complete the last byte of the file.
 * 
 * @param file Current file.
 * @param buffer Current buffer.
 * @param bufferSize Buffer size.
 */
void flush(FILE *file, unsigned char *buffer, size_t *bufferSize);

/**
 * @brief Encodes and write down a QuadTreeRGBA in binary.
 * 
 * @param tree Tree to encode.
 * @param file File to write.
 * @param buffer Current buffer.
 * @param bufferSize Buffer size.
 */
void writeRGBA(QuadTreeRGBA tree, FILE *file, unsigned char *buffer, size_t *bufferSize);

/**
 * @brief Encodes and write down a QuadTreeBin in binary.
 * 
 * @param tree Tree to encode.
 * @param file File to write.
 * @param buffer Current buffer.
 * @param bufferSize Buffer size.
 */
void writeBin(QuadTreeBin tree, FILE *file, unsigned char *buffer, size_t *bufferSize);

/**
 * @brief Encodes and write down a minimized QuadTreeRGBA in ASCII.
 * 
 * @param tree Tree to encode.
 * @param file File to write.
 * @param treeBuffer QuadTreeRGBA buffer.
 * @param index Index of current node. Should be 0 when function is called.
 * @param treeSize Size of the tree. Should be 0 when function is called.
 */
void writeMinimizedRGBA(QuadTreeRGBA tree, FILE *file, QuadTreeRGBABuffer *treeBuffer, int index, size_t *treeSize);

/**
 * @brief Encodes and write down a minimized QuadTreeBin in ASCII.
 * 
 * @param tree Tree to encode.
 * @param file File to write.
 * @param treeBuffer QuadTreeBin buffer.
 * @param index Index of current node. Should be 0 when function is called.
 * @param treeSize Size of the tree. Should be 0 when function is called.
 */
void writeMinimizedBin(QuadTreeBin tree, FILE *file, QuadTreeBinBuffer *treeBuffer, int index, size_t *treeSize);

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
