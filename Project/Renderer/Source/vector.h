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

/*** Vector 4D ***/
typedef struct
{
    double x;
    double y;
    double z;
    double w;
} vector_4d;

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

/*** 2D ***/
extern vector_2d vector_2dAdd(const vector_2d* const V1, const vector_2d* const V2);
extern vector_2d vector_2dDivide(const vector_2d* const Vector2d, const double Scalar);
extern double vector_2dDotProduct(const vector_2d* const V1, const vector_2d* const V2);
extern double vector_2dMagnitude(const vector_2d* const Vector2d);
extern vector_2d vector_2dMultiply(const vector_2d* const Vector2d, const double Scalar);
extern vector_2d vector_2dNormalize(const vector_2d* const Vector2d);
extern vector_2d vector_2dSubtract(const vector_2d* const V1, const vector_2d* const V2);

/*** 3D ***/
extern vector_3d vector_3dAdd(const vector_3d* const V1, const vector_3d* const V2);
extern vector_3d vector_3dCrossProduct(const vector_3d* const V1, const vector_3d* const V2);
extern vector_3d vector_3dDivide(const vector_3d* const Vector3d, const double Scalar);
extern double vector_3dDotProduct(const vector_3d* const V1, const vector_3d* const V2);
extern double vector_3dMagnitude(const vector_3d* const Vector3d);
extern vector_3d vector_3dMultiply(const vector_3d* const Vector3d, const double Scalar);
extern vector_3d vector_3dNormalize(const vector_3d* const Vector3d);
extern vector_2d vector_3dProject2d(const vector_3d* const Vector3d, const double FovFactor);
extern vector_3d vector_3dRotateX(const vector_3d* const Input, const double Angle);
extern vector_3d vector_3dRotateY(const vector_3d* const Input, const double Angle);
extern vector_3d vector_3dRotateZ(const vector_3d* const Input, const double Angle);
extern vector_3d vector_3dSubtract(const vector_3d* const V1, const vector_3d* const V2);
