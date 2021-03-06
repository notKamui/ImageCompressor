/* Created by Irwin on 27/10/2020. */
#include <stdlib.h>

#ifndef __QUADTREE_H
#define __QUADTREE_H

#define INNER_NODE 0
#define OUTER_NODE 1

/**
 * @brief A quadtree node with rgba channels.
 * 
 * If all children are NULL then it is a leaf.
 */
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

/**
 * @brief A quadtree node with binary channel.
 * 
 * If all children are NULL then it is a leaf.
 */
typedef struct s_quadtree_bin
{
    struct s_quadtree_bin *northWest;
    struct s_quadtree_bin *northEast;
    struct s_quadtree_bin *southWest;
    struct s_quadtree_bin *southEast;
    unsigned char b;
} * QuadTreeBin;

/**
 * @brief A buffer to contain RGBA quadtree nodes.
 * Knows its size.
 */
typedef struct s_quadtree_rgba_buffer
{
    QuadTreeRGBA *buffer;
    size_t bufferSize;
} * QuadTreeRGBABuffer;

/**
 * @brief A buffer to contain binary quadtree nodes.
 * Knows its size.
 */
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
 * @brief Calculates the height of a QuadTreeRGBA
 * 
 * @param tree the tree to calculate the height
 * @return the height of the tree
 */
int treeHeightRGBA(QuadTreeRGBA tree);

/**
 * @brief Calculates the height of a QuadTreeBin
 * 
 * @param tree the tree to calculate the height
 * @return the height of the tree
 */
int treeHeightBin(QuadTreeBin tree);

/**
 * @brief Compares two QuadTreeRGBA data regardless of children.
 * 
 * @return Whether nodes are equivalent 
 */
int equivalentRGBA(QuadTreeRGBA tree1, QuadTreeRGBA tree2);

/**
 * @brief Compares two QuadTreeBin data regardless of children.
 * 
 * @return Whether nodes are equivalent 
 */
int equivalentBin(QuadTreeBin tree1, QuadTreeBin tree2);

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
 * @brief Registers a node in the given QuadTreeRGBA buffer at the given index.
 * 
 * @param buffer Buffer to write.
 * @param tree Node to register.
 * @param index Index of the node in the buffer.
 */
void setRGBABuffer(QuadTreeRGBABuffer *buffer, QuadTreeRGBA tree, int index);

/**
 * @brief Registers a node in the given QuadTreeBin buffer at the given index.
 * 
 * @param buffer Buffer to write.
 * @param tree Node to register.
 * @param index Index of the node in the buffer.
 */
void setBinBuffer(QuadTreeBinBuffer *buffer, QuadTreeBin tree, int index);

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
 * @return Index of the node in the buffer is found, -1 otherwise.
 */
int isBufferedRGBA(QuadTreeRGBABuffer buffer, QuadTreeRGBA tree);

/**
 * @brief Checks is the given bin node is in the given buffer.
 * 
 * @param buffer Buffer to search.
 * @param tree Node to check.
 * @return Index of the node in the buffer is found, -1 otherwise.
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

/**
 * @brief Computes the size of the given QuadTreeRGBA.
 * 
 * @param tree Tree to measure.
 * @param buffer Tree buffer.
 */
int sizeQuadTreeRGBA(QuadTreeRGBA tree, QuadTreeRGBABuffer *buffer);

/**
 * @brief Computes the size of the given QuadTreeBin.
 * 
 * @param tree Tree to measure.
 * @param buffer Tree buffer.
 */
int sizeQuadTreeBin(QuadTreeBin tree, QuadTreeBinBuffer *buffer);

/**
 * @brief Frees a given RGBA buffer.
 * 
 * @param buffer The buffer to be freed.
 */
void freeRGBABuffer(QuadTreeRGBABuffer buffer);

/**
 * @brief Frees a given Bin buffer.
 * 
 * @param buffer The buffer to be freed.
 */
void freeBinBuffer(QuadTreeBinBuffer buffer);

/**
 * @brief Frees the given RGBA buffer along with its content.
 * 
 * @param buffer The buffer to be freed.
 */
void hardFreeRGBABuffer(QuadTreeRGBABuffer buffer);

/**
 * @brief Frees the given Bin buffer along with its content.
 * 
 * @param buffer The buffer to be freed.
 */
void hardFreeBinBuffer(QuadTreeBinBuffer buffer);

#endif /* __QUADTREE_H */
