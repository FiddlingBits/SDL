/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include <math.h>
#include "vector.h"

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

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
