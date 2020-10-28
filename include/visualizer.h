/* Created by Irwin on 28/10/2020. */
#include <stdio.h>
#include "quadtree.h"

#ifndef __VISUALIZER_H
#define __VISUALIZER_H

void writeFileHeader(FILE *file);

void writeFileFooter(FILE *file);

void writeQuadTreeRGBA(FILE *file, QuadTreeRGBA tree);

void writeQuadTreeBin(FILE *file, QuadTreeBin tree);

void generatePDFQuadTreeRGBA(char *dotFileName, char *pdfFileName, QuadTreeRGBA tree);

void generatePDFQuadTreeBin(char *dotFileName, char *pdfFileName, QuadTreeBin tree);

#endif /* __VISUALIZER_H */
