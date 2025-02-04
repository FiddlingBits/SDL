/****************************************************************************************************
 * Define
 ****************************************************************************************************/

#define APPLICATION_FOV_FACTOR (1000)
#define APPLICATION_FPS        (30)

/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "display.h"
#include <math.h>
#include "matrix.h"
#include "mesh.h"
#include "obj.h"
#include <SDL.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "triangle.h"
#include "vector.h"

/****************************************************************************************************
 * Variable
 ****************************************************************************************************/

static bool application_backFaceDisplay;
static bool application_fillTriangle;
static bool application_isRunning;
static vector_3d application_cameraPosition;
static obj_object application_object;
static vector_3d application_objectRotation;
static vector_3d application_objectScale;
static vector_3d application_objectTranslate;

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

static void application_processInput(void);
static void application_setUp(void);
static void application_tearDown(void);
static void application_update(void);

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** Main Entry ***/
int main(int argc, char *argv[])
{
    /*** Set Up ***/
    application_setUp();

    /*** Run ***/
    while(application_isRunning)
    {
        /* Render */
        application_processInput();
        application_update();
        display_render();
    }

    /*** Tear Down ***/
    application_tearDown();

    return 0;
}

/****************************************************************************************************
 * Function Definition (Private)
 ****************************************************************************************************/

/*** Process Input ***/
static void application_processInput(void)
{
    SDL_Event event;

    /*** Process Input ***/
    /* Get Input */
    (void)SDL_PollEvent(&event);

    /* Handle Input */
    switch(event.type)
    {
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE)
                application_isRunning = false;
            else if(event.key.keysym.sym == SDLK_b)
                application_backFaceDisplay = !application_backFaceDisplay;
            else if(event.key.keysym.sym == SDLK_f)
                application_fillTriangle = !application_fillTriangle;
            break;
        case SDL_QUIT:
            application_isRunning = false;
            break;
    }
}

/*** Set Up ***/
static void application_setUp(void)
{
    /*** Set Up ***/
    /* Camera (Left-Handed Coordinate System) */
    application_cameraPosition.x = 0.0;
    application_cameraPosition.y = 0.0;
    application_cameraPosition.z = 0.0;

    /* Display */
    application_isRunning = display_init(APPLICATION_FPS);

    /* Flags */
    application_backFaceDisplay = false;
    application_fillTriangle = true;

    /* OBJ File */
    if(application_isRunning)
        application_isRunning = obj_parse("./Asset/cube.obj", &application_object);

    /* Object Rotation */
    application_objectRotation.x = 0.0;
    application_objectRotation.y = 0.0;
    application_objectRotation.z = 0.0;

    /* Object Scale */
    application_objectScale.x = 1.0;
    application_objectScale.y = 1.0;
    application_objectScale.z = 1.0;

    /* Object Translate */
    application_objectTranslate.x = -1.0;
    application_objectTranslate.y = -1.0;
    application_objectTranslate.z = 5.0;
}

/*** Tear Down ***/
static void application_tearDown(void)
{
    /*** Tear Down ***/
    display_deinit();
    obj_destroy(&application_object);
}

/*** Update ***/
static void application_update(void)
{
    int height, i, j, width, triangleCount;
    vector_3d cameraRay, crossVector[2], facePoints[3], normal, transformedPoints[3];
    matrix_4x4 rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, translateMatrix, worldMatrix;
    triangle_triangle triangle, *triangles;

    /*** Update ***/
    /* Set Up */
    display_fillColorBuffer(0xFF000000);
    display_getDimensions(&width, &height);
    if((triangles = malloc(application_object.nFace * sizeof(*triangles))) == NULL)
        return;
    triangleCount = 0;

    /* Object Rotation */
    application_objectRotation.x += 0.01;
    application_objectRotation.y += 0.02;
    application_objectRotation.z += 0.03;

    /* Object Scale */
    application_objectScale.x += 0.02;
    application_objectScale.y += 0.03;
    application_objectScale.z += 0.04;

    /* Object Translate */
    application_objectTranslate.x += 0.05;
    application_objectTranslate.y += 0.05;
    application_objectTranslate.z += 0.25;

    /* Matrix (Individual) */
    rotateXMatrix = matrix_4x4RotateX(application_objectRotation.x);
    rotateYMatrix = matrix_4x4RotateY(application_objectRotation.y);
    rotateZMatrix = matrix_4x4RotateZ(application_objectRotation.z);
    scaleMatrix = matrix_4x4Scale(application_objectScale.x, application_objectScale.y, application_objectScale.z);
    translateMatrix = matrix_4x4Translate(application_objectTranslate.x, application_objectTranslate.y, application_objectTranslate.z);

    /* Matrix (World; Order Matters: Usually Scale, Rotate, Then Translate) */
    matrix_4x4Identity(&worldMatrix);
    worldMatrix = matrix_4x4MultiplyMatrix(&scaleMatrix, &worldMatrix);
    worldMatrix = matrix_4x4MultiplyMatrix(&rotateXMatrix, &worldMatrix);
    worldMatrix = matrix_4x4MultiplyMatrix(&rotateYMatrix, &worldMatrix);
    worldMatrix = matrix_4x4MultiplyMatrix(&rotateZMatrix, &worldMatrix);
    worldMatrix = matrix_4x4MultiplyMatrix(&translateMatrix, &worldMatrix);

    /* Transform Object Face Points */
    for(i = 0; i < application_object.nFace; i++)
    {
        /* Set Up */
        (void)memcpy(
            &facePoints[0],
            &application_object.vertex[application_object.face[i].a],
            sizeof(application_object.vertex[application_object.face[i].a])
        );
        (void)memcpy(
            &facePoints[1],
            &application_object.vertex[application_object.face[i].b],
            sizeof(application_object.vertex[application_object.face[i].b])
        );
        (void)memcpy(
            &facePoints[2],
            &application_object.vertex[application_object.face[i].c],
            sizeof(application_object.vertex[application_object.face[i].c])
        );

        /* Transform Points */
        for(j = 0; j < 3; j++)
            transformedPoints[j] = matrix_4x4MultiplyVector(&worldMatrix, &facePoints[j]);

        /* Cull Back-Face */
        if(!application_backFaceDisplay)
        {
            crossVector[0] = vector_3dSubtract(&transformedPoints[1], &transformedPoints[0]);
            crossVector[1] = vector_3dSubtract(&transformedPoints[2], &transformedPoints[0]);
            normal = vector_3dCrossProduct(&crossVector[0], &crossVector[1]);
            normal = vector_3dNormalize(&normal);
            cameraRay = vector_3dSubtract(&application_cameraPosition, &transformedPoints[0]);
            if(vector_3dDotProduct(&normal, &cameraRay) < 0.0)
                continue; // Cull
        }

        /* Project Points */
        triangle.averageDepth = 0.0;
        for(j = 0; j < 3; j++)
        {
            /* Average Depth */
            triangle.averageDepth += transformedPoints[j].z; // Sum

            /* Project Point */
            triangle.points[j] = vector_3dProject2d(&transformedPoints[j], APPLICATION_FOV_FACTOR);

            /* Translate Projected Point */
            triangle.points[j].x += (width / 2);
            triangle.points[j].y += (height / 2);
        }
        if(application_fillTriangle)
            triangle.color = application_object.faceColor[i];
        else
            triangle.color = 0xFFFFFFFF; // White
        triangle.averageDepth /= 3; // Average
        triangles[triangleCount++] = triangle;
    }

    /* Draw (Sorted) Triangles */
    triangle_sortTriangles(triangles, triangleCount);
    for(i = 0; i < triangleCount; i++)
    {
        if(application_fillTriangle)
            display_drawTriangle(&triangles[i], application_fillTriangle);
        else
            display_drawTriangle(&triangles[i], application_fillTriangle);
    }

    /* Clean Up */
    free(triangles);
}
