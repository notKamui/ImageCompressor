#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "../include/controler.h"
#include "../include/gui.h"
#include "../include/compressor.h"
#include "../include/image.h"
#include "../include/writer.h"
#include "../include/parser.h"

#define INTENT_UNKNOWN -1
#define INTENT_QUIT 0
#define INTENT_OPEN_IMAGE 1
#define INTENT_SAVE_RGBA 2
#define INTENT_SAVE_BIN 3
#define INTENT_MIN_RGBA 4
#define INTENT_MIN_BIN 5
#define INTENT_SAVE_MIN_RGBA 6
#define INTENT_SAVE_MIN_BIN 7
#define INTENT_SAVE_MIN_RGBA_2 8
#define INTENT_SAVE_MIN_BIN_2 9
#define INTENT_CLOSE 10
#define INTENT_SW_BW 11
#define INTENT_SW_RGBA 12

MLV_Image *image;
MLV_Input_box *fileInput;
QuadTreeBin qtBin;
QuadTreeRGBA qtRGBA;
DrawTarget target;
int binIsMinimized;
int RGBAIsMinimized;
char *inputStr, *fileName, *outputFileName;
FILE *file;

int endsWith(const char *string, const char *suffix)
{
    size_t string_len = strlen(string), suffix_len = strlen(suffix);
    if (string_len >= 4)
    {
        return (0 == memcmp(suffix, string + (string_len - suffix_len), suffix_len));
    }
    return 0;
}

FileType getFileType(const char *fname)
{
    if (endsWith(fname, ".png") || endsWith(fname, ".jpg") || endsWith(fname, ".jpeg") || endsWith(fname, ".gif"))
    {
        return TYPE_PICTURE;
    }
    else if (endsWith(fname, ".qtc"))
    {
        return TYPE_QTC;
    }
    else if (endsWith(fname, ".qtn"))
    {
        return TYPE_QTN;
    }
    else if (endsWith(fname, ".gmc"))
    {
        return TYPE_GMC;
    }
    else if (endsWith(fname, ".gmn"))
    {
        return TYPE_GMN;
    }
    else if (endsWith(fname, ".gmc2"))
    {
        return TYPE_GMC2;
    }
    else if (endsWith(fname, ".gmn2"))
    {
        return TYPE_GMN2;
    }
    return TYPE_UNKNOWN;
}

int openImage(const char *fname)
{
    FileType type = getFileType(fname);
    target = ORIGINAL_PICTURE;

    switch (type)
    {
    case TYPE_PICTURE:
        if ((image = MLV_load_image(fname)) == NULL)
        {
            return 0;
        }
        MLV_resize_image(image, PIC_WIDTH, PIC_HEIGHT);
        qtRGBA = allocQuadTreeRGBA(0, 0, 0, 0);
        qtBin = allocQuadTreeBin(0);
        buildRGBATree(qtRGBA, image, 0, 0, PIC_WIDTH, PIC_HEIGHT, 50);
        buildBinTree(qtBin, image, 0, 0, PIC_WIDTH, PIC_HEIGHT, 50);
        return 1;
    default:
        return 0;
    }
    /*
    - Open file
    - Check extension: 
        png,jpg,etc -> image
        qtb -> qtBin
        qtc -> qtRGBA
        ...
    - if image, immediately create qtBin and qtRGBA
    */
}

void saveBin()
{
    if ((outputFileName = calloc(sizeof(char), strlen(fileName) + 4)) == NULL)
    {
        fprintf(stderr, "FATAL: Allocation error.\n");
        exit(1);
    }
    strcat(outputFileName, ".qtn");
    file = fopen(outputFileName, "w");
    writeBin(qtBin, file);
    fclose(file);
    free(outputFileName);
    outputFileName = NULL;
}

void saveMinBin()
{
    if ((outputFileName = calloc(sizeof(char), strlen(fileName) + 4)) == NULL)
    {
        fprintf(stderr, "FATAL: Allocation error.\n");
        exit(1);
    }
    strcat(outputFileName, ".gmn");
    file = fopen(outputFileName, "w");
    writeMinimizedBin(qtBin, file);
    fclose(file);
    free(outputFileName);
    outputFileName = NULL;
}

void saveMinBin2()
{
    if ((outputFileName = calloc(sizeof(char), strlen(fileName) + 4)) == NULL)
    {
        fprintf(stderr, "FATAL: Allocation error.\n");
        exit(1);
    }
    strcat(outputFileName, ".gmn2");
    file = fopen(outputFileName, "w");
    writeMinimizedBin2(qtBin, file);
    fclose(file);
    free(outputFileName);
    outputFileName = NULL;
}

void saveRGBA()
{
    if ((outputFileName = calloc(sizeof(char), strlen(fileName) + 4)) == NULL)
    {
        fprintf(stderr, "FATAL: Allocation error.\n");
        exit(1);
    }
    strcat(outputFileName, ".qtc");
    file = fopen(outputFileName, "w");
    writeRGBA(qtRGBA, file);
    fclose(file);
    free(outputFileName);
    outputFileName = NULL;
}

void saveMinRGBA()
{
    if ((outputFileName = calloc(sizeof(char), strlen(fileName) + 4)) == NULL)
    {
        fprintf(stderr, "FATAL: Allocation error.\n");
        exit(1);
    }
    strcpy(outputFileName, fileName);
    strcat(outputFileName, ".gmc");
    printf("Saving '%s'\n", outputFileName);
    file = fopen(outputFileName, "w");
    writeMinimizedRGBA(qtRGBA, file);
    fclose(file);
    free(outputFileName);
    outputFileName = NULL;
}

void saveMinRGBA2()
{
    if ((outputFileName = calloc(sizeof(char), strlen(fileName) + 4)) == NULL)
    {
        fprintf(stderr, "FATAL: Allocation error.\n");
        exit(1);
    }
    strcat(outputFileName, ".gmc2");
    file = fopen(outputFileName, "w");
    writeMinimizedRGBA2(qtRGBA, file);
    fclose(file);
    free(outputFileName);
    outputFileName = NULL;
}

int getMenuChoice(int page)
{
    int mousex = 0, mousey = 0;
    int choice;
    MLV_Event event;
    MLV_Mouse_button button;
    MLV_Button_state state;

    choice = INTENT_UNKNOWN;
    while (choice == INTENT_UNKNOWN)
    {
        event = MLV_get_event(NULL, NULL, NULL, &inputStr, NULL, &mousex, &mousey, &button, &state);

        /*printf("%d %p='%s'\n", event, fileInput, fileName);*/

        if (event == MLV_INPUT_BOX && page == 0)
        {
            free(fileName);
            fileName = inputStr;
            choice = INTENT_OPEN_IMAGE;
        }
        else if (event == MLV_MOUSE_BUTTON && button == MLV_BUTTON_LEFT && state == MLV_PRESSED)
        {
            if (mousex >= MARGIN && mousex <= WND_WIDTH - 2 * MARGIN - PIC_WIDTH)
            {
                if (mousey >= 40 && mousey <= 100)
                {
                    choice = page == 0 ? -1 : (binIsMinimized ? INTENT_UNKNOWN : INTENT_SAVE_BIN);
                }
                else if (mousey >= 110 && mousey <= 170)
                {
                    choice = page == 0 ? -1 : (binIsMinimized ? INTENT_SAVE_MIN_BIN : INTENT_MIN_BIN);
                }
                else if (mousey >= 180 && mousey <= 240)
                {
                    choice = page == 0 ? -1 : (binIsMinimized ? INTENT_SAVE_MIN_BIN_2 : INTENT_UNKNOWN);
                }
                else if (mousey >= 250 && mousey <= 310)
                {
                    choice = page == 0 ? -1 : (RGBAIsMinimized ? INTENT_UNKNOWN : INTENT_SAVE_RGBA);
                }
                else if (mousey >= 320 && mousey <= 380)
                {
                    choice = page == 0 ? -1 : (RGBAIsMinimized ? INTENT_SAVE_MIN_RGBA : INTENT_MIN_RGBA);
                }
                else if (mousey >= 390 && mousey <= 450)
                {
                    choice = page == 0 ? -1 : (RGBAIsMinimized ? INTENT_SAVE_MIN_RGBA_2 : INTENT_UNKNOWN);
                }
                else if (mousey >= WND_HEIGHT - MARGIN - BTN_HEIGHT && mousey <= WND_HEIGHT - MARGIN)
                {
                    if (page == 0)
                    {
                        choice = INTENT_QUIT;
                    }
                    else
                    {
                        if (mousex >= 40 && mousex <= 40 + BTN_WIDTH_THIRD)
                        {
                            choice = INTENT_SW_BW;
                        }
                        else if (mousex >= 40 + (10 + BTN_WIDTH_THIRD) && mousex <= 40 + (10 + 2 * BTN_WIDTH_THIRD))
                        {
                            choice = INTENT_SW_RGBA;
                        }
                        else if (mousex >= 40 + 2 * (10 + BTN_WIDTH_THIRD) && mousex <= 40 + BTN_WIDTH)
                        {
                            choice = INTENT_CLOSE;
                        }
                    }
                }
            }
        }
        event = MLV_NONE;
        button = -1;
        state = -1;
        if (page == 0)
        {
            drawMainMenu();
        }
        else
        {
            drawImgMenu(binIsMinimized, RGBAIsMinimized);
        }
        MLV_wait_milliseconds(1000 / 60);
    }

    return choice;
}

void closeImage()
{
    MLV_free_image(image);
    image = NULL;
    binIsMinimized = 0;
    RGBAIsMinimized = 0;
}

void quitApp()
{
    QuadTreeRGBABuffer rgbaBuffer = allocQuadTreeRGBABuffer();
    QuadTreeBinBuffer binBuffer = allocQuadTreeBinBuffer();
    freeQuadTreeRGBA(qtRGBA, &rgbaBuffer);
    freeQuadTreeBin(qtBin, &binBuffer);
    freeRGBABuffer(rgbaBuffer);
    freeBinBuffer(binBuffer);
    MLV_free_input_box(fileInput);
}

void menu()
{
    int choice = 0, open = 0;

    while (1)
    {
        drawMainMenu();
        choice = getMenuChoice(0);
        switch (choice)
        {
        case INTENT_OPEN_IMAGE:
            open = openImage(fileName);
            while (open)
            {
                drawImgMenu(binIsMinimized, RGBAIsMinimized);
                choice = getMenuChoice(1);
                MLV_draw_text_box(2 * MARGIN + BTN_WIDTH, MARGIN + PIC_HEIGHT / 2 - 30, PIC_WIDTH, 60, "Please wait...", 40, MLV_COLOR_BLACK, MLV_COLOR_ORANGE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
                MLV_update_window();
                switch (choice)
                {
                case INTENT_SAVE_BIN:
                    saveBin();
                    break;
                case INTENT_SAVE_MIN_BIN:
                    assert(binIsMinimized == 1);
                    saveMinBin();
                    break;
                case INTENT_SAVE_MIN_BIN_2:
                    assert(binIsMinimized == 1);
                    saveMinBin2();
                    break;
                case INTENT_MIN_BIN:
                    assert(binIsMinimized == 0);
                    minimizeQuadTreeBin(&qtBin, 0);
                    target = QT_BIN;
                    binIsMinimized = 1;
                    break;
                case INTENT_SAVE_RGBA:
                    saveRGBA();
                    break;
                case INTENT_SAVE_MIN_RGBA:
                    assert(RGBAIsMinimized == 1);
                    saveMinRGBA();
                    break;
                case INTENT_SAVE_MIN_RGBA_2:
                    assert(RGBAIsMinimized == 1);
                    saveMinRGBA2();
                    break;
                case INTENT_MIN_RGBA:
                    assert(RGBAIsMinimized == 0);
                    minimizeQuadTreeRGBA(&qtRGBA, 5);
                    target = QT_RGBA;
                    RGBAIsMinimized = 1;
                    break;
                case INTENT_SW_BW:
                    target = QT_BIN;
                    break;
                case INTENT_SW_RGBA:
                    target = QT_RGBA;
                    break;
                case INTENT_CLOSE:
                    open = 0;
                    closeImage();
                    break;
                }
            }
            break;
        case INTENT_QUIT:
            quitApp();
            return;
        }
    }
}

void launchApp()
{
    image = NULL;
    qtBin = NULL;
    binIsMinimized = 0;
    qtRGBA = NULL;
    RGBAIsMinimized = 0;
    fileName = malloc(sizeof(char));
    *fileName = '\0';
    target = ORIGINAL_PICTURE;

    MLV_create_window_with_default_font("Super image compressor 2000", NULL, WND_WIDTH, WND_HEIGHT, "fonts/din.ttf", 30);
    menu();

    MLV_free_window();
}