/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include <math.h>
#include "vector.h"

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** 3D Add ***/
vector_3d vector_3dAdd(const vector_3d* const V1, const vector_3d* const V2)
{
    vector_3d vector;

    /*** 3D Add ***/
    vector.x = V1->x + V2->x;
    vector.y = V1->y + V2->y;
    vector.z = V1->z + V2->z;

    return vector;
}

/*** 3D Cross Product ***/
vector_3d vector_3dCrossProduct(const vector_3d* const V1, const vector_3d* const V2)
{
    vector_3d vector;

    /*** 3D Cross Product ***/
    vector.x = (V1->y * V2->z) - (V1->z * V2->y);
    vector.y = (V1->z * V2->x) - (V1->x * V2->z);
    vector.z = (V1->x * V2->y) - (V1->y * V2->x);

    return vector;
}

/*** 3D Divide ***/
vector_3d vector_3dDivide(const vector_3d* const Vector3d, const double Scalar)
{
    vector_3d vector;

    /*** 3D Divide ***/
    vector.x = Vector3d->x / Scalar;
    vector.y = Vector3d->y / Scalar;
    vector.z = Vector3d->z / Scalar;

    return vector;
}

/*** 3D Dot Product ***/
double vector_3dDotProduct(const vector_3d* const V1, const vector_3d* const V2)
{
    /*** 3D Dot Product ***/
    return (V1->x * V2->x) + (V1->y * V2->y) + (V1->z * V2->z);
}

/*** 3D Magnitude ***/
double vector_3dMagnitude(const vector_3d* const Vector3d)
{
    /*** 3D Magnitude ***/
    return sqrt(pow(Vector3d->x, 2.0) + pow(Vector3d->y, 2.0) + pow(Vector3d->z, 2.0));
}

/*** 3D Multiply ***/
vector_3d vector_3dMultiply(const vector_3d* const Vector3d, const double Scalar)
{
    vector_3d vector;

    /*** 3D Multiply ***/
    vector.x = Vector3d->x * Scalar;
    vector.y = Vector3d->y * Scalar;
    vector.z = Vector3d->z * Scalar;

    return vector;
}

/*** 3D Normalize ***/
vector_3d vector_3dNormalize(const vector_3d* const Vector3d)
{
    double magnitude;
    vector_3d vector;

    /*** 3D Normalize ***/
    magnitude = vector_3dMagnitude(Vector3d);
    vector.x = Vector3d->x / magnitude;
    vector.y = Vector3d->y / magnitude;
    vector.z = Vector3d->z / magnitude;

    return vector;
}

/*** 3D Rotate X ***/
vector_3d vector_3dRotateX(const vector_3d* const Input, const double Angle)
{
    vector_3d output;

    /*** 3D Rotate X ***/
    output.x = Input->x;
    output.y = (Input->y * cos(Angle)) - (Input->z * sin(Angle));
    output.z = (Input->y * sin(Angle)) + (Input->z * cos(Angle));

    return output;
}

/*** 3D Rotate Y ***/
vector_3d vector_3dRotateY(const vector_3d* const Input, const double Angle)
{
    vector_3d output;

    /*** 3D Rotate Y ***/
    output.x = (Input->x * cos(Angle)) - (Input->z * sin(Angle));
    output.y = Input->y;
    output.z = (Input->x * sin(Angle)) + (Input->z * cos(Angle));

    return output;
}

/*** 3D Rotate Z ***/
vector_3d vector_3dRotateZ(const vector_3d* const Input, const double Angle)
{
    vector_3d output;

    /*** 3D Rotate Z ***/
    output.x = (Input->x * cos(Angle)) - (Input->y * sin(Angle));
    output.y = (Input->x * sin(Angle)) + (Input->y * cos(Angle));
    output.z = Input->z;

    return output;
}

/*** 3D Subtract ***/
vector_3d vector_3dSubtract(const vector_3d* const V1, const vector_3d* const V2)
{
    vector_3d vector;

    /*** 3D Subtract ***/
    vector.x = V1->x - V2->x;
    vector.y = V1->y - V2->y;
    vector.z = V1->z - V2->z;

    return vector;
}
