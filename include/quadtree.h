/* Created by Irwin on 27/10/2020. */
#include <stdlib.h>

#ifndef __QUADTREE_H
#define __QUADTREE_H

#define INNER_NODE 0
#define OUTER_NODE 1

typedef struct s_quadtree_rgba
{
    struct s_quadtree_rgba *northWest;
    struct s_quadtree_rgba *northEast;
    struct s_quadtree_rgba *southWest;
    struct s_quadtree_rgba *southEast;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} * QuadTreeRGBA;

typedef struct s_quadtree_bin
{
    struct s_quadtree_bin *northWest;
    struct s_quadtree_bin *northEast;
    struct s_quadtree_bin *southWest;
    struct s_quadtree_bin *southEast;
    unsigned char b;
} * QuadTreeBin;

typedef struct s_quadtree_rgba_buffer
{
    QuadTreeRGBA *buffer;
    size_t bufferSize;
} * QuadTreeRGBABuffer;

typedef struct s_quadtree_bin_buffer
{
    QuadTreeBin *buffer;
    size_t bufferSize;
} * QuadTreeBinBuffer;

/**
 * @brief Checks if a QuadTreeRGBA tree is actually a leaf
 * 
 * @param tree the tree to be checked
 * 
 * @return true if the tree is a leaf
 */
int isLeafRGBA(QuadTreeRGBA tree);

/**
 * @brief Checks if a QuadTreeBin tree is actually a leaf
 * 
 * @param tree the tree to be checked
 * 
 * @return true if the tree is a leaf
 */
int isLeafBin(QuadTreeBin tree);

/** 
 * @brief Allocates a QuadTreeRGBA node.
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
 * @brief Allocates a QuadTreeBin node.
 * 
 * @param b Binary value among 0 and 1.
 * 
 * @return The new node.
 */
QuadTreeBin allocQuadTreeBin(unsigned char b);

/**
 * @brief Allocates a QuadTreeRGBA buffer structure made of an array of QuadTreeRGBA and this array's size.
 * 
 * @return QuadTreeRGBABuffer
 */
QuadTreeRGBABuffer allocQuadTreeRGBABuffer();

/**
 * @brief Allocates a QuadTreeBin buffer structure made of an array of QuadTreeBin and this array's size.
 * 
 * @return QuadTreeRGBABuffer
 */
QuadTreeBinBuffer allocQuadTreeBinBuffer();

/**
 * @brief Registers a node in the given QuadTreeRGBA buffer.
 * 
 * @param buffer Buffer to write.
 * @param tree Node to register.
 */
void offerRGBABuffer(QuadTreeRGBABuffer *buffer, QuadTreeRGBA tree);

/**
 * @brief Registers a node in the given QuadTreeBin buffer.
 * 
 * @param buffer Buffer to write.
 * @param tree Node to register.
 */
void offerBinBuffer(QuadTreeBinBuffer *buffer, QuadTreeBin tree);

/**
 * @brief Checks is the given RGBA node is in the given buffer.
 * 
 * @param buffer Buffer to search.
 * @param tree Node to check.
 * @return int Whether the node is buffered or not.
 */
int isBufferedRGBA(QuadTreeRGBABuffer buffer, QuadTreeRGBA tree);

/**
 * @brief Checks is the given bin node is in the given buffer.
 * 
 * @param buffer Buffer to search.
 * @param tree Node to check.
 * @return int Whether the node is buffered or not.
 */
int isBufferedBin(QuadTreeBinBuffer buffer, QuadTreeBin tree);

/**
 * @brief Free memory allocated by the given QuadTreeRGBA.
 * 
 * @param tree Tree to free.
 * @param buffer Tree buffer.
 */
void freeQuadTreeRGBA(QuadTreeRGBA tree, QuadTreeRGBABuffer *buffer);

/**
 * @brief Free memory allocated by the given QuadTreeBin.
 * 
 * @param tree Tree to free.
 * @param buffer Tree buffer.
 */
void freeQuadTreeBin(QuadTreeBin tree, QuadTreeBinBuffer *buffer);

#endif /* __QUADTREE_H */
