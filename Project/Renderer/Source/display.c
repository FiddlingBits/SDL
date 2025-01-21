/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "display.h"
#include <SDL.h>
#include <stdio.h>

/****************************************************************************************************
 * Variable
 ****************************************************************************************************/

static uint32_t* display_colorBuffer = NULL;
static SDL_Texture* display_colorBufferTexture = NULL;
static int display_fpsDelay = 0;
static uint32_t display_previousMilliseconds = 0;
static SDL_Renderer* display_renderer = NULL;
static SDL_Window* display_window = NULL;
static int display_width = 0, display_height = 0;

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** Deinitialize ***/
void display_deinit(void)
{
    /*** Deinitialize (Opposite Order Of Initialization) ***/
    SDL_DestroyTexture(display_colorBufferTexture);
    free(display_colorBuffer);
    SDL_DestroyRenderer(display_renderer);
    SDL_DestroyWindow(display_window);
    SDL_Quit();
}

/*** Draw Rectangle ***/
void display_drawRectangle(const int X, const int Y, const int Width, const int Height, const uint32_t Color)
{
    int x, y;

    /*** Draw Rectangle ***/
    for(x = X; x < (X + Width); x++)
        for(y = Y; y < (Y + Height); y++)
            display_setPixel(x, y, Color);
}

/*** Fill Color Buffer ***/
void display_fillColorBuffer(const uint32_t Color)
{
    int x, y;

    /*** Fill Color Buffer ***/
    for(x = 0; x < display_width; x++)
        for(y = 0; y < display_height; y++)
            display_colorBuffer[(y * display_width) + x] = Color;
}

/*** Get Dimensions ***/
void display_getDimensions(int* const width, int* const height)
{
    /*** Get Dimensions ***/
    if(width != NULL)
        *width = display_width;
    if(height != NULL)
        *height = display_height;
}

/*** Initialize ***/
bool display_init(const int FPS)
{
    SDL_DisplayMode display_mode;

    /*** Initialize ***/
    /* SDL */
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        (void)printf("SDL_Init Failed: %s\n", SDL_GetError());
        return false;
    }

    /* Max Window Size */
    (void)SDL_GetCurrentDisplayMode(0, &display_mode);
    display_width = display_mode.w;
    display_height = display_mode.h;

    /* Window */
    display_window = SDL_CreateWindow(
        "Renderer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        display_width,
        display_height,
        SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE
    );
    if(!display_window)
    {
        (void)printf("SDL_CreateWindow Failed: %s\n", SDL_GetError());
        return false;
    }
    SDL_SetWindowResizable(display_window, SDL_FALSE);

    /* Renderer */
    display_renderer = SDL_CreateRenderer(display_window, -1, 0);
    if(!display_renderer)
    {
        (void)printf("SDL_CreateRenderer Failed: %s\n", SDL_GetError());
        return false;
    }

    /* Color Buffer */
    display_colorBuffer = malloc(sizeof(*display_colorBuffer) * display_width * display_height);
    display_colorBufferTexture = SDL_CreateTexture(
        display_renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        display_width,
        display_height
    );

    /* FPS */
    display_fpsDelay = 1000 / FPS;

    return true;
}

/*** Render ***/
void display_render(void)
{
    int delayMilliseconds;

    /*** Render ***/
    /* Delay */
    delayMilliseconds = (display_previousMilliseconds + display_fpsDelay) - SDL_GetTicks();
    if(delayMilliseconds > 0)
        SDL_Delay(delayMilliseconds);
    display_previousMilliseconds = SDL_GetTicks();

    /* Render */
    (void)SDL_UpdateTexture(
        display_colorBufferTexture,
        NULL,
        display_colorBuffer,
        sizeof(*display_colorBuffer) * display_width
    );
    (void)SDL_RenderCopy(display_renderer, display_colorBufferTexture, NULL, NULL);
    SDL_RenderPresent(display_renderer);
}

/*** Set Pixel ***/
void display_setPixel(const int X, const int Y, const uint32_t Color)
{
    if((X >= 0) && (X < display_width) && (Y >= 0) && (Y < display_height))
        display_colorBuffer[(Y * display_width) + X] = Color;
}
