#include "../include/image.h"
#include "../include/quadtree.h"
#include <MLV/MLV_all.h>
#include <math.h>
#include <stdio.h>

MLV_Color averageColor(MLV_Image *image, int x, int y, int width, int height)
{
    int i, j;
    int r = 0, g = 0, b = 0, a = 0;
    int tmpR = 0, tmpG = 0, tmpB = 0, tmpA = 0;
    const int pixels = width * height;

    for (i = x; i < x + width; i++)
    {
        for (j = y; j < y + height; j++)
        {
            MLV_get_pixel_on_image(image, i, j, &tmpR, &tmpG, &tmpB, &tmpA);
            r += tmpR;
            g += tmpG;
            b += tmpB;
            a += tmpA;
        }
    }

    return MLV_rgba(r / pixels, g / pixels, b / pixels, a / pixels);
}

int distColor(MLV_Color color1, MLV_Color color2)
{
    Uint8 r1, g1, b1, a1, r2, g2, b2, a2;
    MLV_convert_color_to_rgba(color1, &r1, &g1, &b1, &a1);
    MLV_convert_color_to_rgba(color2, &r2, &g2, &b2, &a2);
    return sqrt(
        (r1 - r2) * (r1 - r2) +
        (g1 - g2) * (g1 - g2) +
        (b1 - b2) * (b1 - b2) +
        (a1 - a2) * (a1 - a2));
}

int colorError(MLV_Image *image, MLV_Color average, int x, int y, int width, int height)
{
    int i, j, r, g, b, a, error = 0;
    for (i = x; i < x + width; i++)
    {
        for (j = y; j < y + height; j++)
        {
            MLV_get_pixel_on_image(image, i, j, &r, &g, &b, &a);
            error += distColor(average, MLV_rgba(r, g, b, a));
        }
    }
    return error;
}

void buildRGBATree(QuadTreeRGBA tree, MLV_Image *image, int x, int y, int width, int height, int maxError)
{
    MLV_Color color = averageColor(image, x, y, width, height);
    if (colorError(image, color, x, y, width, height) > maxError)
    {
        tree->northWest = allocQuadTreeRGBA(0, 0, 0, 0);
        buildRGBATree(tree->northWest, image, x, y, width / 2, height / 2, maxError);

        tree->northEast = allocQuadTreeRGBA(0, 0, 0, 0);
        buildRGBATree(tree->northEast, image, x + width / 2, y, width / 2, height / 2, maxError);

        tree->southEast = allocQuadTreeRGBA(0, 0, 0, 0);
        buildRGBATree(tree->southEast, image, x + width / 2, y + height / 2, width / 2, height / 2, maxError);

        tree->southWest = allocQuadTreeRGBA(0, 0, 0, 0);
        buildRGBATree(tree->southWest, image, x, y + height / 2, width / 2, height / 2, maxError);
    }
    else
    {
        MLV_convert_color_to_rgba(color, &(tree->r), &(tree->g), &(tree->b), &(tree->a));
    }
}

void buildBinTree(QuadTreeBin tree, MLV_Image *image, int x, int y, int width, int height, int maxError)
{
    MLV_Color color = averageColor(image, x, y, width, height);
    Uint8 r, g, b, a;
    if (colorError(image, color, x, y, width, height) > maxError)
    {
        tree->northWest = allocQuadTreeBin(0);
        buildBinTree(tree->northWest, image, x, y, width / 2, height / 2, maxError);

        tree->northEast = allocQuadTreeBin(0);
        buildBinTree(tree->northEast, image, x + width / 2, y, width / 2, height / 2, maxError);

        tree->southEast = allocQuadTreeBin(0);
        buildBinTree(tree->southEast, image, x + width / 2, y + height / 2, width / 2, height / 2, maxError);

        tree->southWest = allocQuadTreeBin(0);
        buildBinTree(tree->southWest, image, x, y + height / 2, width / 2, height / 2, maxError);
    }
    else
    {
        MLV_convert_color_to_rgba(color, &r, &g, &b, &a);
        tree->b = (r + g + b + a) / 4 <= 126 ? 0 : 1;
    }
}
