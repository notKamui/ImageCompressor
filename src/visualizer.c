/* Created by Irwin on 28/10/2020. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/quadtree.h"
#include "../include/visualizer.h"

void writeFileHeader(FILE *file)
{
    fprintf(file, "digraph quadtree {\n");
    fprintf(file, "    node [style=filled label=\"\"];\n");
    fprintf(file, "    edge [arrowhead=none];\n\n");
}

void writeFileFooter(FILE *file)
{
    fprintf(file, "}\n");
}

void writeQuadTreeRGBA(FILE *file, QuadTreeRGBA tree)
{
    if (tree == NULL)
    {
        return;
    }

    if (tree->northWest || tree->northEast || tree->southEast || tree->southWest)
    {
        fprintf(file, "    n%p [shape=point];\n", (void *)tree);
    }
    else
    {
        fprintf(file, "    n%p [fixedsize=true width=.15 height=.15 shape=circle fillcolor=\"#%.2x%.2x%.2x%.2x\"];\n", (void *)tree, tree->r, tree->g, tree->b, tree->a);
    }

    if (tree->northWest)
    {
        fprintf(file, "    n%p:c -> n%p:c;\n", (void *)tree, (void *)tree->northWest);
    }
    if (tree->northEast)
    {
        fprintf(file, "    n%p:c -> n%p:c;\n", (void *)tree, (void *)tree->northEast);
    }
    if (tree->southEast)
    {
        fprintf(file, "    n%p:c -> n%p:c;\n", (void *)tree, (void *)tree->southEast);
    }
    if (tree->southWest)
    {
        fprintf(file, "    n%p:c -> n%p:c;\n", (void *)tree, (void *)tree->southWest);
    }

    writeQuadTreeRGBA(file, tree->northWest);

    if (tree->northEast != tree->northWest)
    {
        writeQuadTreeRGBA(file, tree->northEast);
    }
    if (tree->southEast != tree->northWest && tree->northEast != tree->northWest)
    {
        writeQuadTreeRGBA(file, tree->southEast);
    }
    if (tree->southWest != tree->northWest && tree->southEast != tree->northWest && tree->northEast != tree->northWest)
    {
        writeQuadTreeRGBA(file, tree->southWest);
    }
}

void writeQuadTreeBin(FILE *file, QuadTreeBin tree)
{
    if (tree == NULL)
    {
        return;
    }

    if (tree->northWest || tree->northEast || tree->southEast || tree->southWest)
    {
        fprintf(file, "    n%p [shape=point];\n", (void *)tree);
    }
    else if (tree->b == 0)
    {
        fprintf(file, "    n%p [fixedsize=true width=.15 height=.15 shape=circle fillcolor=white];\n", (void *)tree);
    }
    else
    {
        fprintf(file, "    n%p [fixedsize=true width=.15 height=.15 shape=circle fillcolor=black];\n", (void *)tree);
    }

    if (tree->northWest)
    {
        fprintf(file, "    n%p:c -> n%p:c;\n", (void *)tree, (void *)tree->northWest);
    }
    if (tree->northEast)
    {
        fprintf(file, "    n%p:c -> n%p:c;\n", (void *)tree, (void *)tree->northEast);
    }
    if (tree->southEast)
    {
        fprintf(file, "    n%p:c -> n%p:c;\n", (void *)tree, (void *)tree->southEast);
    }
    if (tree->southWest)
    {
        fprintf(file, "    n%p:c -> n%p:c;\n", (void *)tree, (void *)tree->southWest);
    }

    writeQuadTreeBin(file, tree->northWest);

    if (tree->northEast != tree->northWest)
    {
        writeQuadTreeBin(file, tree->northEast);
    }
    if (tree->southEast != tree->northWest && tree->northEast != tree->northWest)
    {
        writeQuadTreeBin(file, tree->southEast);
    }
    if (tree->southWest != tree->northWest && tree->southEast != tree->northWest && tree->northEast != tree->northWest)
    {
        writeQuadTreeBin(file, tree->southWest);
    }
}

void generatePDFQuadTreeRGBA(char *dotFileName, char *pdfFileName, QuadTreeRGBA tree)
{
    FILE *out = fopen(dotFileName, "w");
    int len = strlen(dotFileName) + strlen(pdfFileName) + 15;
    char *cmd = malloc(sizeof(char) * len);

    if (cmd == NULL)
    {
        return;
    }

    writeFileHeader(out);
    writeQuadTreeRGBA(out, tree);
    writeFileFooter(out);
    fclose(out);

    strcpy(cmd, "dot -Tpdf ");
    strcat(cmd, dotFileName);
    strcat(cmd, " -o ");
    strcat(cmd, pdfFileName);
    system(cmd);
    free(cmd);
}

void generatePDFQuadTreeBin(char *dotFileName, char *pdfFileName, QuadTreeBin tree)
{
    FILE *out = fopen(dotFileName, "w");
    int len = strlen(dotFileName) + strlen(pdfFileName) + 15;
    char *cmd = malloc(sizeof(char) * len);

    if (cmd == NULL)
    {
        return;
    }

    writeFileHeader(out);
    writeQuadTreeBin(out, tree);
    writeFileFooter(out);
    fclose(out);

    strcpy(cmd, "dot -Tpdf ");
    strcat(cmd, dotFileName);
    strcat(cmd, " -o ");
    strcat(cmd, pdfFileName);
    system(cmd);
    free(cmd);
}
