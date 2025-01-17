/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "display.h"
#include <SDL.h>
#include <stdbool.h>

/****************************************************************************************************
 * Variable
 ****************************************************************************************************/

static bool application_isRunning = false;

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

static void application_processInput(void);

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** Main Entry ***/
int main(int argc, char *argv[])
{
    /*** Initialize ***/
    application_isRunning = display_init();

    /*** Run ***/
    while(application_isRunning)
    {
        /* Render */
        application_processInput();
        display_fillColorBuffer(0xFF000000);
        display_drawRectangle(100, 50, 250, 125, 0xFFFF0000);
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
