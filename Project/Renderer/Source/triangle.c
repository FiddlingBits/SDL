/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include <stdlib.h>
#include "triangle.h"

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

static int triangle_comparePoints(const void* P0, const void* P1);
static int triangle_compareTriangles(const void* T0, const void* T1);

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** Sort Points ***/
triangle_triangle triangle_sortPoints(const triangle_triangle* const Triangle)
{
    triangle_triangle triangle;

    /*** Sort Points (By Y Coordinate, Ascending) ***/
    /* Set Up */
    triangle = *(Triangle);
    qsort(triangle.points, 3, sizeof(triangle.points[0]), triangle_comparePoints);

    return triangle;
}

/*** Sort Triangles ***/
void triangle_sortTriangles(triangle_triangle* triangle, const size_t TriangleCount)
{
    /*** Sort Triangles (By Average Depth, Descending) ***/
    qsort(triangle, TriangleCount, sizeof(*triangle), triangle_compareTriangles);
}

/****************************************************************************************************
 * Function Definition (Private)
 ****************************************************************************************************/

/*** Compare Points ***/
static int triangle_comparePoints(const void* P0, const void* P1)
{
    double y0, y1;

    /*** Compare Points ***/
    y0 = ((const vector_2d*)P0)->y;
    y1 = ((const vector_2d*)P1)->y;

    return (y0 > y1) - (y0 < y1);
}

/*** Compare Triangles ***/
static int triangle_compareTriangles(const void* T0, const void* T1)
{
    double d0, d1;

    /*** Compare Points ***/
    d0 = ((const triangle_triangle*)T0)->averageDepth;
    d1 = ((const triangle_triangle*)T1)->averageDepth;

    return -((d0 > d1) - (d0 < d1));
}
