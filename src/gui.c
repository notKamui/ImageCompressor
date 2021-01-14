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

int getMenuChoice()
{
    int mousex, mousey;
    int choice;

    choice = 0;
    while (choice == 0)
    {
        MLV_wait_mouse(&mousex, &mousey);

        if (mousex >= 40 && mousex <= 160)
        {
            if (mousey >= 40 && mousey <= 80)
            {
                choice = 1;
            }
            else if (mousey >= 120 && mousey <= 160)
            {
                choice = 2;
            }
            else if (mousey >= 200 && mousey <= 240)
            {
                choice = 3;
            }
            else if (mousey >= 280 && mousey <= 320)
            {
                choice = 4;
            }
            else if (mousey >= 360 && mousey <= 400)
            {
                choice = 5;
            }
        }
    }

    return choice;
}

void drawMainMenu()
{
    MLV_clear_window(MLV_COLOR_BLACK);

    MLV_draw_rectangle(40, 40, 120, 40, MLV_COLOR_WHITE);
    MLV_draw_text(50, 50, "Open Image", MLV_COLOR_WHITE);

    MLV_draw_rectangle(40, 120, 120, 40, MLV_COLOR_WHITE);
    MLV_draw_text(70, 130, "Quit", MLV_COLOR_WHITE);

    MLV_actualise_window();
}

void drawImgMenu(int binIsMinimized, int RGBAIsMinimized)
{
    MLV_clear_window(MLV_COLOR_BLACK);

    if (!binIsMinimized)
    {
        MLV_draw_text_box(40, 40, 130, 40, "Save B&W", 20, MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
        /*MLV_draw_rectangle(40, 40, 120, 40, MLV_COLOR_WHITE);
        MLV_draw_text(50, 50, "Save B&W", MLV_COLOR_WHITE);*/

        MLV_draw_rectangle(40, 120, 120, 40, MLV_COLOR_WHITE);
        MLV_draw_text(50, 130, "Minimize B&W", MLV_COLOR_WHITE);
    }
    else
    {
        MLV_draw_rectangle(40, 40, 120, 40, MLV_COLOR_WHITE);
        MLV_draw_text(50, 50, "Save MinB&W", MLV_COLOR_WHITE);
    }

    if (!RGBAIsMinimized)
    {
        MLV_draw_rectangle(40, 200, 120, 40, MLV_COLOR_WHITE);
        MLV_draw_text(50, 210, "Save RGBA", MLV_COLOR_WHITE);

        MLV_draw_rectangle(40, 280, 120, 40, MLV_COLOR_WHITE);
        MLV_draw_text(50, 290, "Minimize RGBA", MLV_COLOR_WHITE);
    }
    else
    {
        MLV_draw_rectangle(40, 200, 120, 40, MLV_COLOR_WHITE);
        MLV_draw_text(50, 210, "Save MinRGBA", MLV_COLOR_WHITE);
    }

    MLV_draw_rectangle(40, 360, 120, 40, MLV_COLOR_WHITE);
    MLV_draw_text(70, 370, "Quit", MLV_COLOR_WHITE);

    MLV_actualise_window();
}
