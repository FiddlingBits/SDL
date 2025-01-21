/****************************************************************************************************
 * Pragma
 ****************************************************************************************************/

#pragma once

/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include <stdbool.h>
#include <stdint.h>

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

extern void display_deinit(void);
extern void display_drawRectangle(const int X, const int Y, const int Width, const int Height, const uint32_t Color);
extern void display_fillColorBuffer(const uint32_t Color);
extern void display_getDimensions(int* const width, int* const height);
extern bool display_init(const int FPS);
extern void display_render(void);
extern void display_setPixel(const int X, const int Y, const uint32_t Color);
