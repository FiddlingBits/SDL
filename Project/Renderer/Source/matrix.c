/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include <math.h>
#include "matrix.h"
#include <string.h>

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** 4x4 Identity ***/
void matrix_4x4Identity(matrix_4x4* const matrix)
{
    /*** 4x4 Identity ***/
    (void)memcpy(
        matrix->element,
        (double [4][4]){{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
        sizeof(matrix->element)
    );
}

/*** 4x4 Multiply Matrix ***/
matrix_4x4 matrix_4x4MultiplyMatrix(const matrix_4x4* const M1, const matrix_4x4* const M2)
{
    int i, j;
    matrix_4x4 matrix;

    /*** 4x4 Multiply Matrix ***/
    matrix_4x4Identity(&matrix);
    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++)
            matrix.element[i][j] = (M1->element[i][0] * M2->element[0][j]) + (M1->element[i][1] * M2->element[1][j]) + (M1->element[i][2] * M2->element[2][j]) + (M1->element[i][3] * M2->element[3][j]);

    return matrix;
}

/*** 4x4 Multiply Vector 3D ***/
vector_3d matrix_4x4MultiplyVector3D(const matrix_4x4* const Matrix, const vector_3d* const Vector)
{
    vector_3d vector3d;
    vector_4d vector4d[2];

    /*** 4x4 Multiply Vector 3D ***/
    vector4d[0].x = Vector->x;
    vector4d[0].y = Vector->y;
    vector4d[0].z = Vector->z;
    vector4d[0].w = 1.0;
    vector4d[1] = matrix_4x4MultiplyVector4D(Matrix, &vector4d[0]);
    vector3d.x = vector4d[1].x;
    vector3d.y = vector4d[1].y;
    vector3d.z = vector4d[1].z;

    return vector3d;
}

/*** 4x4 Multiply Vector 4D ***/
vector_4d matrix_4x4MultiplyVector4D(const matrix_4x4* const Matrix, const vector_4d* const Vector)
{
    vector_4d vector4d;

    /*** 4x4 Multiply Vector 4D ***/
    vector4d.x = (Matrix->element[0][0] * Vector->x) +
        (Matrix->element[0][1] * Vector->y) +
        (Matrix->element[0][2] * Vector->z) +
        (Matrix->element[0][3] * Vector->w);
    vector4d.y = (Matrix->element[1][0] * Vector->x) +
        (Matrix->element[1][1] * Vector->y) +
        (Matrix->element[1][2] * Vector->z) +
        (Matrix->element[1][3] * Vector->w);
    vector4d.z = (Matrix->element[2][0] * Vector->x) +
        (Matrix->element[2][1] * Vector->y) +
        (Matrix->element[2][2] * Vector->z) +
        (Matrix->element[2][3] * Vector->w);
    vector4d.w = (Matrix->element[3][0] * Vector->x) +
        (Matrix->element[3][1] * Vector->y) +
        (Matrix->element[3][2] * Vector->z) +
        (Matrix->element[3][3] * Vector->w);

    return vector4d;
}

/*** 4x4 Perspective ***/
matrix_4x4 matrix_4x4Perspective(const int Width, const int Height, const double FOV, const double ZNear, const double ZFar)
{
    matrix_4x4 matrix;

    /*** 4x4 Perspective X ***/
    memset(matrix.element, 0, sizeof(matrix.element));
    matrix.element[0][0] = ((double)Height / Width) * (1.0 / tan(FOV / 2.0));
    matrix.element[1][1] = (1.0 / tan(FOV / 2.0));
    matrix.element[2][2] = ZFar / (ZFar - ZNear);
    matrix.element[2][3] = (-ZFar * ZNear) / (ZFar - ZNear);
    matrix.element[3][2] = 1.0; // Save Original, Unchanged Z In W

    return matrix;
}

/*** 4x4 Project ***/
vector_3d matrix_4x4Project(const matrix_4x4* const Matrix, const vector_3d* const Vector)
{
    vector_3d vector3d = {0.0, 0.0, 0.0};
    vector_4d vector4d[2];

    /*** 4x4 Project ***/
    vector4d[0].x = Vector->x;
    vector4d[0].y = Vector->y;
    vector4d[0].z = Vector->z;
    vector4d[0].w = 1.0;
    vector4d[1] = matrix_4x4MultiplyVector4D(Matrix, &vector4d[0]);
    if(vector4d[1].w != 0.0)
    {
        vector3d.x = vector4d[1].x / vector4d[1].w;
        vector3d.y = vector4d[1].y / vector4d[1].w;
        vector3d.z = vector4d[1].z / vector4d[1].w;
    }

    return vector3d;
}

/*** 4x4 Rotate X ***/
matrix_4x4 matrix_4x4RotateX(const double Angle)
{
    matrix_4x4 matrix;

    /*** 4x4 Rotate X ***/
    matrix_4x4Identity(&matrix);
    matrix.element[1][1] = cos(Angle);
    matrix.element[1][2] = -sin(Angle);
    matrix.element[2][1] = sin(Angle);
    matrix.element[2][2] = cos(Angle);

    return matrix;
}

/*** 4x4 Rotate Y ***/
matrix_4x4 matrix_4x4RotateY(const double Angle)
{
    matrix_4x4 matrix;

    /*** 4x4 Rotate Y ***/
    matrix_4x4Identity(&matrix);
    matrix.element[0][0] = cos(Angle);
    matrix.element[0][2] = sin(Angle);
    matrix.element[2][0] = -sin(Angle);
    matrix.element[2][2] = cos(Angle);

    return matrix;
}

/*** 4x4 Rotate Z ***/
matrix_4x4 matrix_4x4RotateZ(const double Angle)
{
    matrix_4x4 matrix;

    /*** 4x4 Rotate Z ***/
    matrix_4x4Identity(&matrix);
    matrix.element[0][0] = cos(Angle);
    matrix.element[0][1] = -sin(Angle);
    matrix.element[1][0] = sin(Angle);
    matrix.element[1][1] = cos(Angle);

    return matrix;
}

/*** 4x4 Scale ***/
matrix_4x4 matrix_4x4Scale(const double X, const double Y, const double Z)
{
    matrix_4x4 matrix;

    /*** 4x4 Scale ***/
    matrix_4x4Identity(&matrix);
    matrix.element[0][0] = X;
    matrix.element[1][1] = Y;
    matrix.element[2][2] = Z;

    return matrix;
}

/*** 4x4 Translate ***/
matrix_4x4 matrix_4x4Translate(const double X, const double Y, const double Z)
{
    matrix_4x4 matrix;

    /*** 4x4 Scale ***/
    matrix_4x4Identity(&matrix);
    matrix.element[0][3] = X;
    matrix.element[1][3] = Y;
    matrix.element[2][3] = Z;

    return matrix;
}
