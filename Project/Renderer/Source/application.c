/****************************************************************************************************
 * Define
 ****************************************************************************************************/

#define APPLICATION_N_CUBE_POINTS      (                                     \
                                            APPLICATION_N_CUBE_SIDE_POINTS * \
                                            APPLICATION_N_CUBE_SIDE_POINTS * \
                                            APPLICATION_N_CUBE_SIDE_POINTS   \
                                       )
#define APPLICATION_N_CUBE_SIDE_POINTS (9)
#define APPLICATION_FOV_FACTOR         (1000)
#define APPLICATION_FPS                (30)

/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "display.h"
#include <SDL.h>
#include <stdbool.h>
#include "vector.h"

/****************************************************************************************************
 * Variable
 ****************************************************************************************************/

static bool application_isRunning;
static vector_3d application_cameraPosition;
static vector_3d application_cubePoints[APPLICATION_N_CUBE_POINTS];
static vector_3d application_cubeRotation;

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
    int i, x, y, z;

    /*** Set Up ***/
    /* Camera (Left-Handed Coordinate System) */
    application_cameraPosition.x = 0.0;
    application_cameraPosition.y = 0.0;
    application_cameraPosition.z = -5.0;

    /* Cube Points */
    i = 0;
    for(x = 0; x < APPLICATION_N_CUBE_SIDE_POINTS; x++)
    {
        for(y = 0; y < APPLICATION_N_CUBE_SIDE_POINTS; y++)
        {
            for(z = 0; z < APPLICATION_N_CUBE_SIDE_POINTS; z++)
            {
                application_cubePoints[i].x = -1.0 + (x * 0.25);
                application_cubePoints[i].y = -1.0 + (y * 0.25);
                application_cubePoints[i].z = -1.0 + (z * 0.25);
                i++;
            }
        }
    }
    
    /* Cube Rotation */
    application_cubeRotation.x = 0.0;
    application_cubeRotation.y = 0.0;
    application_cubeRotation.z = 0.0;
}

/*** Update ***/
static void application_update(void)
{
    int height, i, width;
    vector_2d projectedPoints[APPLICATION_N_CUBE_POINTS];
    vector_3d transformedPoint;

    /*** Update ***/
    /* Set Up */
    display_fillColorBuffer(0xFF000000);
    display_getDimensions(&width, &height);

    /* Cube Rotation */
    application_cubeRotation.x += 0.01;
    application_cubeRotation.y += 0.01;
    application_cubeRotation.z += 0.01;

    /* Projected Points */
    for(i = 0; i < APPLICATION_N_CUBE_POINTS; i++)
    {
        /* Rotate */
        transformedPoint = vector_3dRotateX(&application_cubePoints[i], application_cubeRotation.x);
        transformedPoint = vector_3dRotateY(&transformedPoint, application_cubeRotation.y);
        transformedPoint = vector_3dRotateZ(&transformedPoint, application_cubeRotation.z);

        /* Project */
        projectedPoints[i].x = (APPLICATION_FOV_FACTOR * transformedPoint.x);
        projectedPoints[i].x /= (transformedPoint.z + application_cameraPosition.z);
        projectedPoints[i].y = (APPLICATION_FOV_FACTOR * transformedPoint.y);
        projectedPoints[i].y /= (transformedPoint.z + application_cameraPosition.z);
    }

    /* Draw Projected Cube Points */
    for(i = 0; i < APPLICATION_N_CUBE_POINTS; i++)
    {
        display_drawRectangle(
            (int)projectedPoints[i].x + (width / 2),
            (int)projectedPoints[i].y + (height / 2),
            5,
            5,
            0xFFFF0000
        );
    }
}
