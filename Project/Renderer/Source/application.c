/****************************************************************************************************
 * Define
 ****************************************************************************************************/

#define APPLICATION_FOV (60 * (M_PI / 180.0)) // Convert Degrees To Radians
#define APPLICATION_FPS (30)

/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "display.h"
#include "light.h"
#include <math.h>
#include "matrix.h"
#include "mesh.h"
#include "obj.h"
#include <SDL.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "triangle.h"
#include "vector.h"

/****************************************************************************************************
 * Variable
 ****************************************************************************************************/

static bool application_backFaceDisplay;
static bool application_fillTriangle;
static double application_fov;
static double application_fovZoomIncrement;
static light_light application_globalLight;
static bool application_isRunning;
static bool application_zoom;
static vector_3d application_cameraPosition;
static obj_object application_object;
static uint32_t application_objectColor[4];
static int application_objectColorIndex;
static vector_3d application_objectRotation;
static vector_3d application_objectTranslate;
static double application_objectTranslateAngle;
static double application_objectTranslateRadius;

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
            else if(event.key.keysym.sym == SDLK_c)
                application_objectColorIndex = (application_objectColorIndex + 1) % (sizeof(application_objectColor) / sizeof(application_objectColor[0]));
            else if(event.key.keysym.sym == SDLK_f)
                application_fillTriangle = !application_fillTriangle;
            else if(event.key.keysym.sym == SDLK_z)
                application_zoom = !application_zoom;
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
    application_zoom = false;

    /* FOV */
    application_fov = APPLICATION_FOV;
    application_fovZoomIncrement = 0.1;

    /* Global Light */
    application_globalLight.direction.x = 0.0;
    application_globalLight.direction.y = 0.0;
    application_globalLight.direction.z = 1.0;
    application_globalLight.direction = vector_3dNormalize(&application_globalLight.direction);

    /* OBJ File */
    if(application_isRunning)
        application_isRunning = obj_parse("./Asset/cube.obj", &application_object);

    /* Object Color */
    application_objectColor[0] = 0xFFFF0000; // Red
    application_objectColor[1] = 0xFF00FF00; // Green
    application_objectColor[2] = 0xFF0000FF; // Blue
    application_objectColor[3] = 0xFFFFFFFF; // White
    application_objectColorIndex = 0;

    /* Object Rotation */
    application_objectRotation.x = 0.0;
    application_objectRotation.y = 0.0;
    application_objectRotation.z = 0.0;

    /* Object Translate */
    application_objectTranslateAngle = 0.0;
    application_objectTranslateRadius = 5.0;
    application_objectTranslate.x = application_objectTranslateRadius * cos(application_objectTranslateAngle);
    application_objectTranslate.y = application_objectTranslateRadius * sin(application_objectTranslateAngle);
    application_objectTranslate.z = 10.0;
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
    vector_3d cameraRay, crossVector[2], facePoints[3], normal, transformedPoints[3];
    double factor;
    int height, i, j, width, triangleCount;
    matrix_4x4 perspectiveMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, translateMatrix, worldMatrix;
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

    /* Object Translate */
    application_objectTranslateAngle += 0.01;
    application_objectTranslate.x = application_objectTranslateRadius * cos(application_objectTranslateAngle);
    application_objectTranslate.y = application_objectTranslateRadius * sin(application_objectTranslateAngle);
    application_objectTranslate.z += 0.01;

    /* Matrix (Individual) */
    rotateXMatrix = matrix_4x4RotateX(application_objectRotation.x);
    rotateYMatrix = matrix_4x4RotateY(application_objectRotation.y);
    rotateZMatrix = matrix_4x4RotateZ(application_objectRotation.z);
    scaleMatrix = matrix_4x4Scale(1.0, 1.0, 1.0);
    translateMatrix = matrix_4x4Translate(application_objectTranslate.x, application_objectTranslate.y, application_objectTranslate.z);

    /* Matrix (World; Order Matters: Usually Scale, Rotate, Then Translate) */
    matrix_4x4Identity(&worldMatrix);
    worldMatrix = matrix_4x4MultiplyMatrix(&scaleMatrix, &worldMatrix);
    worldMatrix = matrix_4x4MultiplyMatrix(&rotateXMatrix, &worldMatrix);
    worldMatrix = matrix_4x4MultiplyMatrix(&rotateYMatrix, &worldMatrix);
    worldMatrix = matrix_4x4MultiplyMatrix(&rotateZMatrix, &worldMatrix);
    worldMatrix = matrix_4x4MultiplyMatrix(&translateMatrix, &worldMatrix);

    /* Perspective Matrix */
    if(!application_zoom && (application_fov < APPLICATION_FOV))
        application_fov += application_fovZoomIncrement;
    else if(application_zoom && (application_fov > (APPLICATION_FOV / 2.0)))
        application_fov -= application_fovZoomIncrement;
    perspectiveMatrix = matrix_4x4Perspective(width, height, application_fov, 0.1, 100.0);

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
            transformedPoints[j] = matrix_4x4MultiplyVector3D(&worldMatrix, &facePoints[j]);

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
            triangle.points[j] = matrix_4x4Project(&perspectiveMatrix, &transformedPoints[j]);

            /* Scale Projected Point For Screen */
            triangle.points[j].x *= (width / 2.0);
            triangle.points[j].y *= (height / 2.0);
            triangle.points[j].x += (width / 2.0);
            triangle.points[j].y += (height / 2.0);
        }
        if(application_fillTriangle)
        {
            /* Light Intensity Is Based On How Aligned Face Normal Is With Inverse Of Light Ray */
            factor = -vector_3dDotProduct(&normal, &application_globalLight.direction);
            triangle.color = light_applyIntensity(application_objectColor[application_objectColorIndex], factor);
        }
        else
        {
            triangle.color = 0xFFFFFFFF; // White
        }
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
