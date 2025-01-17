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
static SDL_Renderer* display_renderer = NULL;
static SDL_Window* display_window = NULL;
static int display_width = 0, display_height = 0;

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** Initialize ***/
bool display_init(void)
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
    display_window = SDL_CreateWindow("Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, display_width, display_height, 0);
    if(!display_window)
    {
        (void)printf("SDL_CreateWindow Failed: %s\n", SDL_GetError());
        return false;
    }
    (void)SDL_SetWindowFullscreen(display_window, SDL_WINDOW_FULLSCREEN);

    /* Renderer */
    display_renderer = SDL_CreateRenderer(display_window, -1, 0);
    if(!display_renderer)
    {
        (void)printf("SDL_CreateRenderer Failed: %s\n", SDL_GetError());
        return false;
    }

    /* Color Buffer */
    display_colorBuffer = malloc(sizeof(*display_colorBuffer) * display_width * display_height);
    display_colorBufferTexture = SDL_CreateTexture(display_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, display_width, display_height);

    return true;
}

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
    int col, row;

    /*** Draw Rectangle ***/
    for(col = X; col < (X + Width); col++)
        for(row = Y; row < (Y + Height); row++)
            display_colorBuffer[(row * display_width) + col] = Color;
}

/*** Fill Color Buffer ***/
void display_fillColorBuffer(const uint32_t Color)
{
    int col, row;

    /*** Fill Color Buffer ***/
    for(row = 0; row < display_height; row++)
        for(col = 0; col < display_width; col++)
            display_colorBuffer[(row * display_width) + col] = Color;
}

/*** Render ***/
void display_render(void)
{
    /*** Render ***/
    (void)SDL_UpdateTexture(display_colorBufferTexture, NULL, display_colorBuffer, sizeof(*display_colorBuffer) * display_width);
    (void)SDL_RenderCopy(display_renderer, display_colorBufferTexture, NULL, NULL);
    SDL_RenderPresent(display_renderer);
}
