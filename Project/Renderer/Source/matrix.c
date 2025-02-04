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

/*** 4x4 Multiply Vector ***/
vector_3d matrix_4x4MultiplyVector(const matrix_4x4* const Matrix, const vector_3d* const Vector)
{
    vector_3d vector3;
    vector_4d vector4;

    /*** 4x4 Multiply Vector ***/
    vector4.x = (Matrix->element[0][0] * Vector->x) +
        (Matrix->element[0][1] * Vector->y) +
        (Matrix->element[0][2] * Vector->z) +
        (Matrix->element[0][3] * 1.0);
    vector4.y = (Matrix->element[1][0] * Vector->x) +
        (Matrix->element[1][1] * Vector->y) +
        (Matrix->element[1][2] * Vector->z) +
        (Matrix->element[1][3] * 1.0);
    vector4.z = (Matrix->element[2][0] * Vector->x) +
        (Matrix->element[2][1] * Vector->y) +
        (Matrix->element[2][2] * Vector->z) +
        (Matrix->element[2][3] * 1.0);
    vector4.w = (Matrix->element[3][0] * Vector->x) +
        (Matrix->element[3][1] * Vector->y) +
        (Matrix->element[3][2] * Vector->z) +
        (Matrix->element[3][3] * 1.0);
    vector3.x = vector4.x;
    vector3.y = vector4.y;
    vector3.z = vector4.z;

    return vector3;
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
