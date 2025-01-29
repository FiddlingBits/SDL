/****************************************************************************************************
 * Define
 ****************************************************************************************************/

#define APPLICATION_FOV_FACTOR (1000)
#define APPLICATION_FPS        (30)

/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "display.h"
#include "mesh.h"
#include "obj.h"
#include <SDL.h>
#include <stdbool.h>
#include <string.h>
#include "triangle.h"
#include "vector.h"

/****************************************************************************************************
 * Variable
 ****************************************************************************************************/

static bool application_isRunning;
static vector_3d application_cameraPosition;
static obj_object application_object;
static vector_3d application_objectRotation;

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

static void application_processInput(void);
static void application_setUp(void);
static void application_update(void);

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** Main Entry ***/
int main(int argc, char *argv[])
{
    /*** Initialize ***/
    application_isRunning = display_init(APPLICATION_FPS);

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

    /*** Deinitialize ***/
    display_deinit();

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
    application_cameraPosition.z = -5.0;

    /* OBJ File */
    if(application_isRunning)
        application_isRunning = obj_parse("./Asset/cube.obj", &application_object);
}

/*** Update ***/
static void application_update(void)
{
    int height, i, j, width;
    vector_3d facePoints[3], transformedPoint;
    triangle_triangle triangle;

    /*** Update ***/
    /* Set Up */
    display_fillColorBuffer(0xFF000000);
    display_getDimensions(&width, &height);

    /* Object Rotation */
    application_objectRotation.x += 0.01;
    application_objectRotation.y += 0.01;
    application_objectRotation.z += 0.01;

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
        {
            /* Transform Point */
            transformedPoint = vector_3dRotateX(&facePoints[j], application_objectRotation.x);
            transformedPoint = vector_3dRotateY(&transformedPoint, application_objectRotation.y);
            transformedPoint = vector_3dRotateZ(&transformedPoint, application_objectRotation.z);

            /* Translate Point From Camera */
            transformedPoint.x += application_cameraPosition.x;
            transformedPoint.y += application_cameraPosition.y;
            transformedPoint.z += application_cameraPosition.z;

            /* Project Point */
            triangle.points[j] = vector_3dProject2d(&transformedPoint, APPLICATION_FOV_FACTOR);

            /* Translate Projected Point */
            triangle.points[j].x += (width / 2);
            triangle.points[j].y += (height / 2);
        }

        /* Draw Triangle */
        display_drawTriangle(&triangle, 0xFFFFFFFF);
    }
}
