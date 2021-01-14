#include <string.h>
#include <stdio.h>
#include "../include/controler.h"
#include "../include/gui.h"
#include "../include/compressor.h"

typedef enum
{
    TYPE_UNKNOWN,
    TYPE_PICTURE,
    TYPE_QTC,
    TYPE_QTN,
    TYPE_GMC,
    TYPE_GMN,
    TYPE_GMC2,
    TYPE_GMN2
} FileType;

MLV_Image *image;
QuadTreeBin *qtBin;
QuadTreeRGBA *qtRGBA;
int binIsMinimized;
int RGBAIsMinimized;

int endsWith(const char *string, const char *suffix)
{
    size_t string_len = strlen(string);
    size_t suffix_len = strlen(suffix);
    if (string_len >= 4)
    {
        return (0 == memcmp(suffix, string + (string_len - suffix_len), suffix_len));
    }
    return 0;
}

FileType getFileType(const char *fname)
{
    if (endsWith(fname, ".png") || endsWith(fname, ".jpg") || endsWith(fname, ".gif"))
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

void openImage(const char *fname)
{
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
    /* temporary, to change with real function */
}

void saveMinBin()
{
    /* temporary, to change with real function */
}

void saveRGBA()
{
    /* temporary, to change with real function */
}

void saveMinRGBA()
{
    /* temporary, to change with real function */
}

void menu()
{
    int choice = 0;

    drawMainMenu();
    while (1)
    {
        choice = getMenuChoice();
        switch (choice)
        {
        case 1: /* Open Image */
            printf("Opening image (to QuadTree)...\n");
            openImage("");
            while (1)
            {
                drawImgMenu(binIsMinimized, RGBAIsMinimized);
                choice = getMenuChoice();
                switch (choice)
                {
                case 1: /* Save B&W OR Save MinB&W */
                    if (binIsMinimized)
                    {
                        /* Save MinB&W */
                        printf("Saving to MinB&W...\n");
                        saveMinBin();
                    }
                    else
                    {
                        /* MinB&W */
                        printf("Saving to B&W...\n");
                        saveBin();
                    }
                    break;
                case 2: /* Minimize to B&W */
                    if (!binIsMinimized)
                    {
                        printf("Minimizing to B&W...\n");
                        /*minimizeQuadTreeBin(qtBin, 0);*/
                        binIsMinimized = 1;
                    }
                    break;
                case 3: /* Save RGBA OR Save MinRGBA */
                    if (RGBAIsMinimized)
                    {
                        /* Save MinRGBA */
                        printf("Saving to MinRGBA...\n");
                        saveMinRGBA();
                    }
                    else
                    {
                        /* MinRGBA */
                        printf("Saving to RGBA...\n");
                        saveRGBA();
                    }
                    break;
                case 4: /* Minimize to RGBA */
                    if (!RGBAIsMinimized)
                    {
                        printf("Minimizing to RGBA...\n");
                        /*minimizeQuadTreeRGBA(qtRGBA, 0);*/
                        RGBAIsMinimized = 1;
                    }
                    break;
                case 5: /* Quit */
                    printf("Quitting...\n");
                    return;
                }
            }
        case 2: /* Quit */
            printf("Quitting...\n");
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

    MLV_create_window("Super image compressor 2000", NULL, WND_WIDTH, WND_HEIGHT);

    menu();

    MLV_free_window();
}