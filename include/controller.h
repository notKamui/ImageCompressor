#ifndef __CONTROLLER_H
#define __CONTROLLER_H

/**
 * @brief Defines the different supported file types
 * 
 */
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

/**
 * @brief Defines the possible targets to draw on the window
 * 
 */
typedef enum
{
    ORIGINAL_PICTURE,
    QT_BIN,
    QT_RGBA,
    WIREFRAME_QT_BIN,
    WIREFRAME_QT_RGBA
} DrawTarget;

/**
 * @brief Displays the menu of the application
 */
void launchApp();

#endif /* __CONTROLLER_H */
