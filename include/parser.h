/* Created by Irwin on 18/11/2020. */
#ifndef __PARSER_H
#define __PARSER_H

#include <stdio.h>
#include "quadtree.h"

/**
 * @brief Reads and decodes a QuadTreeRGBA from a given file.
 * 
 * @param file File to read.
 * @param tree Built tree. (Method souldn't be called with NULL tree)
 * 
 * @return Whether the build succeeded. 
 */
int parseRGBA(FILE *file, QuadTreeRGBA tree);

/**
 * @brief Reads and decodes a QuadTreeBin from a given file.
 * 
 * @param file File to read.
 * @param tree Built tree. (Method souldn't be called with NULL tree)
 * 
 * @return Whether the build succeeded. 
 */
int parseBin(FILE *file, QuadTreeBin tree);

/**
 * @brief Reads and decodes a minimized QuadTreeRGBA from a given file.
 * 
 * @param file File to read.
 * @param tree Built tree. (Method souldn't be called with NULL tree)
 * 
 * @return Whether the build succeeded.
 */
int parseMinimizedRGBA(FILE *file, QuadTreeRGBA tree);

/**
 * @brief Reads and decodes a minimized QuadTreeBin from a given file.
 * 
 * @param file File to read.
 * @param tree Built tree. (Method souldn't be called with NULL tree)
 * 
 * @return Whether the build succeeded.
 */
int parseMinimizedBin(FILE *file, QuadTreeBin tree);

/**
 * @brief Reads and decodes a minimized QuadTreeRGBA from a given binary file.
 * 
 * @param file File to read.
 * @param tree Built tree. (Method souldn't be called with NULL tree)
 * 
 * @return Whether the build succeeded. 
 */
int parseMinimizedRGBA2(FILE *file, QuadTreeRGBA tree);

/**
 * @brief Reads and decodes a minimized QuadTreeBin from a given binary file.
 * 
 * @param file File to read.
 * @param tree Built tree. (Method souldn't be called with NULL tree)
 * 
 * @return Whether the build succeeded. 
 */
int parseMinimizedBin2(FILE *file, QuadTreeBin tree);

#endif /* __PARSER_H */
