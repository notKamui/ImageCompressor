#ifndef __CONTROLER_H
#define __CONTROLER_H

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

typedef enum
{
    ORIGINAL_PICTURE,
    QT_BIN,
    QT_RGBA
} DrawTarget;

/**
 * @brief Displays the menu of the application
 */
void launchApp();

#endif /* __CONTROLER_H */
