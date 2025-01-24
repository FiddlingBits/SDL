/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include <math.h>
#include "vector.h"

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** 3D Project 2D ***/
vector_2d vector_3dProject2d(const vector_3d* const Vector3d, const double FovFactor)
{
    vector_2d vector2d;

    /*** 3D Project 2D ***/
    vector2d.x = (FovFactor * Vector3d->x) / Vector3d->z;
    vector2d.y = (FovFactor * Vector3d->y) / Vector3d->z;
    
    return vector2d;
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
