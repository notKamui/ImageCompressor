/* Created by Irwin on 14/11/2020. */
#ifndef __IMAGE_H
#define __IMAGE_H

#include <MLV/MLV_all.h>

/**
 * @brief Compute average color on the given area of the given image. 
 * 
 * @param image Image to compute.
 * @param width Width of the area.
 * @param height Height of the area.
 * @return MLV_Color average color of the area.
 */
MLV_Color averageColor(MLV_Image *image, int x, int y, int width, int height);

/**
 * @brief Compute Euclidian distance between two colors
 */
int distColor(MLV_Color color1, MLV_Color color2);

/**
 * @brief Compute color error on the given area of the given image.
 * 
 * @param image Image to compute.
 * @param average Average color.
 * @param width Width of the area.
 * @param height Height of the area.
 * @return int Color error.
 */
int colorError(MLV_Image *image, MLV_Color average, int x, int y, int width, int height);

#endif /* __IMAGE_H */