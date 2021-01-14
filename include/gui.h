/* Created by Irwin on 14/11/2020 */
#ifndef __GUI_H
#define __GUI_H

#include <MLV/MLV_all.h>
#include "quadtree.h"

<<<<<<< Updated upstream
#define PIC_WIDTH 512
#define PIC_HEIGHT PIC_WIDTH

#define MARGIN 40
#define BTN_WIDTH 350
#define BTN_HEIGHT 60

#define WND_WIDTH 3 * MARGIN + BTN_WIDTH + PIC_WIDTH
#define BTN_WIDTH_THIRD (BTN_WIDTH - 20) / 3
#define WND_HEIGHT 2 * MARGIN + PIC_HEIGHT
=======
#define FONT_SIZE 42

    typedef struct s_button_info
{
    int x;
    int y;
    int width;
    int height;
    void(*f);
} ButtonInfo;
>>>>>>> Stashed changes

    /**
 * @brief Draw a RGBAQuadTree recursively at the given coordinates.
 * 
 * @param tree RGBAQuadTree to draw.
 * @param width Width of the draw.
 * @param height Height of the draw.
 */
    void
    drawQuadTreeRGBA(QuadTreeRGBA tree, int x, int y, int width, int height);

void drawQuadTreeCircleRGBA(QuadTreeRGBA tree, int x, int y, int width, int height);

/**
 * @brief Draw a BinQuadTree recursively at the given coordinates.
 * 
 * @param tree BinQuadTree to draw.
 * @param width Width of the draw.
 * @param height Height of the draw.
 */
void drawQuadTreeBin(QuadTreeBin tree, int x, int y, int width, int height);

/**
 * @brief Gets the menu choice on click
 * 
 * @return the clicked menu choice
 */
int getMenuChoice();

/**
 * @brief Draws the main menu to open an image
 */
void drawMainMenu();

/**
 * @brief Draws the menu for manipulating the image
 * 
 * @param binIsMinimized is not 0 if the image has been minimized to B&W
 * @param RGBAIsMinimized is not 0 if the image has been minimized to RGBA
 */
void drawImgMenu(int binIsMinimized, int RGBAIsMinimized);

#endif /* __GUI_H */
