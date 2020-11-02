/* Created by Irwin on 28/10/2020. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/quadtree.h"
#include "../include/visualizer.h"

void writeFileHeader(FILE *file) {
    fprintf(file, "digraph quadtree {\n");
    fprintf(file, "    node [shape = record, height = .1]\n");
    fprintf(file, "    edge [tailclip = false];\n\n");
}

void writeFileFooter(FILE *file) {
    fprintf(file, "}\n");
}

void writeQuadTreeRGBA(FILE *file, QuadTreeRGBA tree) {
    if (tree == NULL) return;

    fprintf(file, "    n%p [label = \"<NW> NW | <SW> SW | <r> r:%d | <g> g:%d | <b> b:%d | <a> a:%f | <NE> NE | <SE> SE\"];\n", (void *) tree, tree->r, tree->g, tree->b, tree->a);

    if (tree->northWest)
        fprintf(file, "    n%p:NW:s -> n%p;\n", (void *) tree, (void *) tree->northWest);
    if (tree->southWest)
        fprintf(file, "    n%p:SW:s -> n%p;\n", (void *) tree, (void *) tree->southWest);
    if (tree->northEast)
        fprintf(file, "    n%p:NE:s -> n%p;\n", (void *) tree, (void *) tree->northEast);
    if (tree->southEast)
        fprintf(file, "    n%p:SE:s -> n%p;\n", (void *) tree, (void *) tree->southEast);

    writeQuadTreeRGBA(file, tree->northWest);

    if (tree->southWest != tree->northWest)
        writeQuadTreeRGBA(file, tree->southWest);

    if (tree->northEast != tree->northWest && tree->northEast != tree->southWest)
        writeQuadTreeRGBA(file, tree->northEast);

    if (tree->southEast != tree->northWest && tree->southEast != tree->southWest && tree->southEast != tree->northEast)
        writeQuadTreeRGBA(file, tree->southEast);
}

void writeQuadTreeBin(FILE *file, QuadTreeBin tree) {
    if (tree == NULL) return;

    fprintf(file, "    n%p [label = \"<NW> NW | <SW> SW | <b> %d | <NE> NE | <SE> SE\"];\n", (void *) tree, tree->b);

    if (tree->northWest)
        fprintf(file, "    n%p:NW:s -> n%p;\n", (void *) tree, (void *) tree->northWest);
    if (tree->southWest)
        fprintf(file, "    n%p:SW:s -> n%p;\n", (void *) tree, (void *) tree->southWest);
    if (tree->northEast)
        fprintf(file, "    n%p:NE:s -> n%p;\n", (void *) tree, (void *) tree->northEast);
    if (tree->southEast)
        fprintf(file, "    n%p:SE:s -> n%p;\n", (void *) tree, (void *) tree->southEast);

    writeQuadTreeBin(file, tree->northWest);

    if (tree->southWest != tree->northWest)
        writeQuadTreeBin(file, tree->southWest);

    if (tree->northEast != tree->northWest && tree->northEast != tree->southWest)
        writeQuadTreeBin(file, tree->northEast);

    if (tree->southEast != tree->northWest && tree->southEast != tree->southWest && tree->southEast != tree->northEast)
        writeQuadTreeBin(file, tree->southEast);
}

void generatePDFQuadTreeRGBA(char *dotFileName, char *pdfFileName, QuadTreeRGBA tree) {
    FILE *out = fopen(dotFileName, "w");
    int len = strlen(dotFileName) + strlen(pdfFileName) + 15;
    char *cmd = malloc(sizeof(char) * len);

    if (cmd == NULL) return;

    writeFileHeader(out);
    writeQuadTreeRGBA(out, tree);
    writeFileFooter(out);
    fclose(out);
    
    strcpy(cmd, "dot -Tpdf ");
    strcat(cmd, dotFileName);
    strcat(cmd, " -o ");
    strcat(cmd, pdfFileName);
    system(cmd);
}

void generatePDFQuadTreeBin(char *dotFileName, char *pdfFileName, QuadTreeBin tree) {
    FILE *out = fopen(dotFileName, "w");
    int len = strlen(dotFileName) + strlen(pdfFileName) + 15;
    char *cmd = malloc(sizeof(char) * len);

    if (cmd == NULL) return;

    writeFileHeader(out);
    writeQuadTreeBin(out, tree);
    writeFileFooter(out);
    fclose(out);
    
    strcpy(cmd, "dot -Tpdf ");
    strcat(cmd, dotFileName);
    strcat(cmd, " -o ");
    strcat(cmd, pdfFileName);
    system(cmd);
}
