/****************************************************************************************************
 * Pragma
 ****************************************************************************************************/

#pragma once

/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include "triangle.h"

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

extern void display_deinit(void);
extern void display_drawLine(const int X0, const int Y0, const int X1, const int Y1, const uint32_t Color);
extern void display_drawTriangle(const triangle_triangle* const Triangle, const bool Fill);
extern void display_fillColorBuffer(const uint32_t Color);
extern void display_getDimensions(int* const width, int* const height);
extern bool display_init(const int FPS);
extern void display_render(void);
extern void display_setPixel(const int X, const int Y, const uint32_t Color);
