#include "../include/gui.h"
#include "../include/controler.h"
#include <stdio.h>

extern MLV_Input_box *fileInput;
extern MLV_Image *image;
extern DrawTarget target;
extern QuadTreeBin qtBin;
extern QuadTreeRGBA qtRGBA;

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

void drawQuadTreeWireframeRGBA(QuadTreeRGBA tree, int x, int y, int width, int height)
{
    if (isLeafRGBA(tree))
    {
        MLV_draw_rectangle(x, y, width, height, MLV_rgba(tree->r, tree->g, tree->b, tree->a));
        return;
    }

    drawQuadTreeWireframeRGBA(tree->northWest, x, y, width / 2, height / 2);
    drawQuadTreeWireframeRGBA(tree->northEast, x + width / 2, y, width / 2, height / 2);
    drawQuadTreeWireframeRGBA(tree->southEast, x + width / 2, y + height / 2, width / 2, height / 2);
    drawQuadTreeWireframeRGBA(tree->southWest, x, y + height / 2, width / 2, height / 2);
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

void drawQuadTreeWireframeBin(QuadTreeBin tree, int x, int y, int width, int height)
{
    if (isLeafBin(tree))
    {
        MLV_draw_rectangle(x, y, width, height, tree->b ? MLV_COLOR_BLACK : MLV_COLOR_WHITE);
        return;
    }

    drawQuadTreeWireframeBin(tree->northWest, x, y, width / 2, height / 2);
    drawQuadTreeWireframeBin(tree->northEast, x + width / 2, y, width / 2, height / 2);
    drawQuadTreeWireframeBin(tree->southEast, x + width / 2, y + height / 2, width / 2, height / 2);
    drawQuadTreeWireframeBin(tree->southWest, x, y + height / 2, width / 2, height / 2);
}

void drawMainMenu()
{
    MLV_clear_window(MLV_COLOR_BLACK);

    MLV_draw_rectangle(40, 40, BTN_WIDTH, BTN_HEIGHT, MLV_COLOR_GRAY);
    if (!fileInput)
    {
        fileInput = MLV_create_input_box(50, 41, BTN_WIDTH - 20, BTN_HEIGHT - 2, MLV_COLOR_BLACK, MLV_COLOR_WHITE, MLV_COLOR_BLACK, "Path : ");
    }
    /*MLV_draw_text_box(40, 110, BTN_WIDTH, BTN_HEIGHT, "Open image", 20, MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);*/
    MLV_draw_text_box(40, WND_HEIGHT - MARGIN - BTN_HEIGHT, BTN_WIDTH, BTN_HEIGHT, "Quit", 20, MLV_COLOR_WHITE, MLV_COLOR_RED, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);

    MLV_draw_filled_rectangle(WND_WIDTH - 40 - PIC_WIDTH, 40, PIC_WIDTH, PIC_HEIGHT, MLV_COLOR_GRAY);

    MLV_draw_all_input_boxes();
    MLV_actualise_window();
}

void drawImgMenu(int binIsMinimized, int RGBAIsMinimized)
{
    if (fileInput)
    {
        MLV_free_input_box(fileInput);
        fileInput = NULL;
    }

    MLV_clear_window(MLV_COLOR_BLACK);
    MLV_draw_filled_rectangle(WND_WIDTH - 40 - PIC_WIDTH, 40, PIC_WIDTH, PIC_HEIGHT, MLV_COLOR_GRAY);

    switch (target)
    {
    case QT_BIN:
        if (qtBin)
        {
            drawQuadTreeBin(qtBin, WND_WIDTH - 40 - PIC_WIDTH, 40, PIC_WIDTH, PIC_HEIGHT);
        }
        break;
    case QT_RGBA:
        if (qtRGBA)
        {
            drawQuadTreeRGBA(qtRGBA, WND_WIDTH - 40 - PIC_WIDTH, 40, PIC_WIDTH, PIC_HEIGHT);
        }
        break;
    case WIREFRAME_QT_BIN:
        if (qtBin)
        {
            drawQuadTreeWireframeBin(qtBin, WND_WIDTH - 40 - PIC_WIDTH, 40, PIC_WIDTH, PIC_HEIGHT);
        }
        break;
    case WIREFRAME_QT_RGBA:
        if (qtRGBA)
        {
            drawQuadTreeWireframeRGBA(qtRGBA, WND_WIDTH - 40 - PIC_WIDTH, 40, PIC_WIDTH, PIC_HEIGHT);
        }
        break;
    default:
        MLV_draw_image(image, WND_WIDTH - 40 - PIC_WIDTH, 40);
        break;
    }

    if (qtBin)
    {
        if (!binIsMinimized)
        {
            MLV_draw_text_box(40, 40, BTN_WIDTH, BTN_HEIGHT, "Save B&W", 20, MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
            MLV_draw_text_box(40, 110, BTN_WIDTH, BTN_HEIGHT, "Minimize B&W", 20, MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
            MLV_draw_text_box(40, 180, BTN_WIDTH, BTN_HEIGHT, "Save minimized B&W 2", 20, MLV_COLOR_GRAY30, MLV_COLOR_GRAY30, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
        }
        else
        {
            MLV_draw_text_box(40, 40, BTN_WIDTH, BTN_HEIGHT, "Save B&W", 20, MLV_COLOR_GRAY30, MLV_COLOR_GRAY30, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
            MLV_draw_text_box(40, 110, BTN_WIDTH, BTN_HEIGHT, "Save minimized B&W", 20, MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
            MLV_draw_text_box(40, 180, BTN_WIDTH, BTN_HEIGHT, "Save minimized B&W 2", 20, MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
        }
        MLV_draw_text_box(40, WND_HEIGHT - MARGIN - BTN_HEIGHT, BTN_WIDTH_THIRD, BTN_HEIGHT, "B&W", 20, MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    }
    else
    {
        if (image)
        {
            MLV_draw_text_box(40, 40, BTN_WIDTH, BTN_HEIGHT, "Build B&W", 20, MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
        }
        else
        {
            MLV_draw_text_box(40, 40, BTN_WIDTH, BTN_HEIGHT, "Build B&W", 20, MLV_COLOR_GRAY30, MLV_COLOR_GRAY30, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
        }
        MLV_draw_text_box(40, 110, BTN_WIDTH, BTN_HEIGHT, "Minimize B&W", 20, MLV_COLOR_GRAY30, MLV_COLOR_GRAY30, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
        MLV_draw_text_box(40, 180, BTN_WIDTH, BTN_HEIGHT, "Save minimized B&W 2", 20, MLV_COLOR_GRAY30, MLV_COLOR_GRAY30, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
        MLV_draw_text_box(40, WND_HEIGHT - MARGIN - BTN_HEIGHT, BTN_WIDTH_THIRD, BTN_HEIGHT, "B&W", 20, MLV_COLOR_GRAY30, MLV_COLOR_GRAY30, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    }

    if (qtRGBA)
    {
        if (!RGBAIsMinimized)
        {
            MLV_draw_text_box(40, 250, BTN_WIDTH, BTN_HEIGHT, "Save RGBA", 20, MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
            MLV_draw_text_box(40, 320, BTN_WIDTH, BTN_HEIGHT, "Minimize RGBA", 20, MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
            MLV_draw_text_box(40, 390, BTN_WIDTH, BTN_HEIGHT, "Save minimized RGBA 2", 20, MLV_COLOR_GRAY30, MLV_COLOR_GRAY30, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
        }
        else
        {
            MLV_draw_text_box(40, 250, BTN_WIDTH, BTN_HEIGHT, "Save RGBA", 20, MLV_COLOR_GRAY30, MLV_COLOR_GRAY30, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
            MLV_draw_text_box(40, 320, BTN_WIDTH, BTN_HEIGHT, "Save minimized RGBA", 20, MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
            MLV_draw_text_box(40, 390, BTN_WIDTH, BTN_HEIGHT, "Save minimized RGBA 2", 20, MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
        }
        MLV_draw_text_box(40 + (BTN_WIDTH_THIRD + 10), WND_HEIGHT - MARGIN - BTN_HEIGHT, BTN_WIDTH_THIRD, BTN_HEIGHT, "RGBA", 20, MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    }
    else
    {
        if (image)
        {
            MLV_draw_text_box(40, 250, BTN_WIDTH, BTN_HEIGHT, "Build RGBA", 20, MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
        }
        else
        {
            MLV_draw_text_box(40, 250, BTN_WIDTH, BTN_HEIGHT, "Build RGBA", 20, MLV_COLOR_GRAY30, MLV_COLOR_GRAY30, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
        }
        MLV_draw_text_box(40, 320, BTN_WIDTH, BTN_HEIGHT, "Minimize RGBA", 20, MLV_COLOR_GRAY30, MLV_COLOR_GRAY30, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
        MLV_draw_text_box(40, 390, BTN_WIDTH, BTN_HEIGHT, "Save minimized RGBA 2", 20, MLV_COLOR_GRAY30, MLV_COLOR_GRAY30, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
        MLV_draw_text_box(40 + (BTN_WIDTH_THIRD + 10), WND_HEIGHT - MARGIN - BTN_HEIGHT, BTN_WIDTH_THIRD, BTN_HEIGHT, "RGBA", 20, MLV_COLOR_GRAY30, MLV_COLOR_GRAY30, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    }

    MLV_draw_text_box(40 + 2 * (BTN_WIDTH_THIRD + 10), WND_HEIGHT - MARGIN - BTN_HEIGHT, BTN_WIDTH_THIRD, BTN_HEIGHT, "Close", 20, MLV_COLOR_WHITE, MLV_COLOR_RED, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);

    MLV_actualise_window();
}
