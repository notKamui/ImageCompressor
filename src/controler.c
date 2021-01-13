#include "../include/controler.h"

MLV_Image *image;
QuadTreeBin *qtBin;
QuadTreeRGBA *qtRGBA;
int binIsMinimized;
int RGBAIsMinimized;

void openImage(const char *fname) {
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

void saveBin() {
    /* temporary, to change with real function */
}

void saveMinBin() {
    /* temporary, to change with real function */
}

void saveRGBA() {
    /* temporary, to change with real function */
}

void saveMinRGBA() {
    /* temporary, to change with real function */
}

void menu() {
    int choice = 0;

    drawMainMenu();
    while(1)
    {
        choice = getMenuChoice();
        switch (choice) 
        {
            case 1: /* Open Image */
                printf("Opening image (to QuadTree)...\n");
                openImage("");
                while(1)
                {
                    drawImgMenu(binIsMinimized, RGBAIsMinimized);
                    choice = getMenuChoice();
                    switch (choice)
                    {
                        case 1: /* Save B&W */
                            if (!binIsMinimized)
                            {
                                printf("Saving to B&W...\n");
                                saveBin();
                            }
                            break;
                        case 2: /* Save RGBA */
                            if (!RGBAIsMinimized)
                            {
                                printf("Saving to RGBA...\n");
                                saveRGBA();
                            }
                            break;
                        case 3: /* Minimize to B&W OR Save MinB&W */
                            if (binIsMinimized)
                            {
                                /* Save MinB&W */
                                printf("Saving to MinB&W...\n");
                                saveMinBin();
                            }
                            else
                            {
                                /* Minimize to B&W */
                                printf("Minimizing to B&W...\n");
                                /*minimizeQuadTreeBin(qtBin, 0);*/
                                binIsMinimized = 1;
                            }
                            break;
                        case 4: /* Minimize to RGBA OR Save MinRGBA */
                            if (RGBAIsMinimized)
                            {
                                /* Save MinRGBA */
                                printf("Saving to MinRGBA...\n");
                                saveMinRGBA();
                            }
                            else
                            {
                                /* Minimize to RGBA */
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

    MLV_create_window("Super image compressor 2000", "", WND_WIDTH, WND_HEIGHT);

    menu();

    MLV_free_window();
}