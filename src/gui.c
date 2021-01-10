#include "../include/gui.h"
#include <stdio.h>

void drawQuadTreeRGBA(QuadTreeRGBA tree, int x, int y, int width, int height)
{
    if (isLeafRGBA(tree))
    {
        MLV_draw_filled_rectangle(x, y, width, height, MLV_rgba(tree->r, tree->g, tree->b, tree->a));
        return;
    }

    drawQuadTreeRGBA(tree->northWest, x, y, width / 2, height / 2);
    drawQuadTreeRGBA(tree->northEast, x + width / 2, y, width / 2, height / 2);
    drawQuadTreeRGBA(tree->southEast, x + width / 2, y + height / 2, width / 2, height / 2);
    drawQuadTreeRGBA(tree->southWest, x, y + height / 2, width / 2, height / 2);
}

void drawQuadTreeBin(QuadTreeBin tree, int x, int y, int width, int height)
{
    if (isLeafBin(tree))
    {
        MLV_draw_filled_rectangle(x, y, width, height, tree->b ? MLV_COLOR_BLACK : MLV_COLOR_WHITE);
        return;
    }

    drawQuadTreeBin(tree->northWest, x, y, width / 2, height / 2);
    drawQuadTreeBin(tree->northEast, x + width / 2, y, width / 2, height / 2);
    drawQuadTreeBin(tree->southEast, x + width / 2, y + height / 2, width / 2, height / 2);
    drawQuadTreeBin(tree->southWest, x, y + height / 2, width / 2, height / 2);
}
