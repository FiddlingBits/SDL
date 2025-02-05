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

/*** Matrix 4x4 ***/
typedef struct
{
    double element[4][4];
} matrix_4x4;

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

extern void matrix_4x4Identity(matrix_4x4* const matrix);
extern matrix_4x4 matrix_4x4MultiplyMatrix(const matrix_4x4* const M1, const matrix_4x4* const M2);
extern vector_3d matrix_4x4MultiplyVector3D(const matrix_4x4* const Matrix, const vector_3d* const Vector);
extern vector_4d matrix_4x4MultiplyVector4D(const matrix_4x4* const Matrix, const vector_4d* const Vector);
extern matrix_4x4 matrix_4x4Perspective(const int Width, const int Height, const double FOV, const double ZNear, const double ZFar);
extern vector_3d matrix_4x4Project(const matrix_4x4* const Matrix, const vector_3d* const Vector);
extern matrix_4x4 matrix_4x4RotateX(const double Angle);
extern matrix_4x4 matrix_4x4RotateY(const double Angle);
extern matrix_4x4 matrix_4x4RotateZ(const double Angle);
extern matrix_4x4 matrix_4x4Scale(const double X, const double Y, const double Z);
extern matrix_4x4 matrix_4x4Translate(const double X, const double Y, const double Z);
