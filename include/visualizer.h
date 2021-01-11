/* Created by Irwin on 28/10/2020. */
#include <stdio.h>
#include "quadtree.h"

#ifndef __VISUALIZER_H
#define __VISUALIZER_H

/**
 * Generates a PDF file of a QuadTreeRGBA.
 * 
 * @param dotFileName Name of the .dot file.
 * @param pdfFileName Name of the .pdf file.
 * @param tree Tree to export.
 */
void generatePDFQuadTreeRGBA(char *dotFileName, char *pdfFileName, QuadTreeRGBA tree);

/**
 * Generates a PDF file of a QuadTreeBin.
 * 
 * @param dotFileName Name of the .dot file.
 * @param pdfFileName Name of the .pdf file.
 * @param tree Tree to export.
 */
void generatePDFQuadTreeBin(char *dotFileName, char *pdfFileName, QuadTreeBin tree);

#endif /* __VISUALIZER_H */
