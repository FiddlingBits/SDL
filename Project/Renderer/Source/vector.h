/****************************************************************************************************
 * Pragma
 ****************************************************************************************************/

#pragma once

/****************************************************************************************************
 * Structure
 ****************************************************************************************************/

/*** Vector 2D ***/
typedef struct
{
    double x;
    double y;
} vector_2d;

/*** Vector 3D ***/
typedef struct
{
    double x;
    double y;
    double z;
} vector_3d;

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

extern vector_2d vector_3dProject2d(const vector_3d* const Vector3d, const double FovFactor);
extern vector_3d vector_3dRotateX(const vector_3d* const Input, const double Angle);
extern vector_3d vector_3dRotateY(const vector_3d* const Input, const double Angle);
extern vector_3d vector_3dRotateZ(const vector_3d* const Input, const double Angle);
