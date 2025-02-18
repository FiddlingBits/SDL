/****************************************************************************************************
 * Pragma
 ****************************************************************************************************/

#pragma once

/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include <stdint.h>
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
    double averageDepth;
    uint32_t color;
    vector_3d points[3];
} triangle_triangle;

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

extern triangle_triangle triangle_sortPoints(const triangle_triangle* const Triangle);
extern void triangle_sortTriangles(triangle_triangle* triangle, const size_t TriangleCount);
