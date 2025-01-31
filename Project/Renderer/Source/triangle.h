/****************************************************************************************************
 * Pragma
 ****************************************************************************************************/

#pragma once

/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "vector.h"

/****************************************************************************************************
 * Structure
 ****************************************************************************************************/

/*** Face ***/
typedef struct
{
    int a;
    int b;
    int c;
} triangle_face;

/*** Triangle ***/
typedef struct
{
    vector_2d points[3];
} triangle_triangle;

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

extern triangle_triangle triangle_sortPoints(const triangle_triangle* const Triangle);
