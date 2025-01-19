/****************************************************************************************************
 * Define
 ****************************************************************************************************/

#define APPLICATION_N_CUBE_POINTS      (                                     \
                                            APPLICATION_N_CUBE_SIDE_POINTS * \
                                            APPLICATION_N_CUBE_SIDE_POINTS * \
                                            APPLICATION_N_CUBE_SIDE_POINTS   \
                                       )
#define APPLICATION_N_CUBE_SIDE_POINTS (9)
#define APPLICATION_FOV_FACTOR         (2000)

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

static bool application_isRunning = false;
static vector_3d application_cameraPosition;
static vector_3d application_cubePoints[APPLICATION_N_CUBE_POINTS];
static vector_2d application_projectedPoints[APPLICATION_N_CUBE_POINTS];

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
    application_isRunning = display_init();

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

    /* Projected Points */
    for(i = 0; i < APPLICATION_N_CUBE_POINTS; i++)
    {
        application_projectedPoints[i].x = (APPLICATION_FOV_FACTOR * application_cubePoints[i].x);
        application_projectedPoints[i].x /= (application_cubePoints[i].z + application_cameraPosition.z);
        application_projectedPoints[i].y = (APPLICATION_FOV_FACTOR * application_cubePoints[i].y);
        application_projectedPoints[i].y /= (application_cubePoints[i].z + application_cameraPosition.z);
    }
}

/*** Update ***/
static void application_update(void)
{
    int height, i, width;

    /*** Update ***/
    /* Set Up */
    display_fillColorBuffer(0xFF000000);
    display_getDimensions(&width, &height);

    /* Draw Projected Cube Points */
    for(i = 0; i < APPLICATION_N_CUBE_POINTS; i++)
    {
        display_drawRectangle(
            application_projectedPoints[i].x + (width / 2),
            application_projectedPoints[i].y + (height / 2),
            5,
            5,
            0xFFFF0000
        );
    }

    /* Render */
    display_render();
}
