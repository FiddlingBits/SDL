/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include "display.h"
#include <math.h>
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
 * Function Prototype
 ****************************************************************************************************/

static void display_fillFlatBottomTriangle(const int X0, const int Y0, const int X1, const int Y1, const int X2, const int Y2, const uint32_t Color);
static void display_fillFlatTopTriangle(const int X0, const int Y0, const int X1, const int Y1, const int X2, const int Y2, const uint32_t Color);

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

/*** Draw Line ***/
void display_drawLine(const int X0, const int Y0, const int X1, const int Y1, const uint32_t Color)
{
    int deltaX, deltaY, i, longestSideLength;
    double incrementX, incrementY;

    /*** Draw Line (Digital Differential Analyzer Algorithm) ***/
    /* Delta */
    deltaX = (X1 - X0);
    deltaY = (Y1 - Y0);

    /* Longest Side Length */
    longestSideLength = (abs(deltaX) >= abs(deltaY)) ? abs(deltaX) : abs(deltaY);

    /* Increment */
    incrementX = (double)deltaX / longestSideLength;
    incrementY = (double)deltaY / longestSideLength;

    /* Draw Line */
    for(i = 0; i <= longestSideLength; i++)
        display_setPixel((int)round(X0 + (i * incrementX)), (int)round(Y0 + (i * incrementY)), Color);
}

/*** Draw Triangle ***/
void display_drawTriangle(const triangle_triangle* const Triangle, const uint32_t Color, const bool Fill)
{
    vector_2d flatPoints[2];
    triangle_triangle triangle;

    /*** Draw Triangle ***/
    /* Line */
    display_drawLine(
        (int)Triangle->points[0].x,
        (int)Triangle->points[0].y,
        (int)Triangle->points[1].x,
        (int)Triangle->points[1].y,
        Color);
    display_drawLine(
        (int)Triangle->points[1].x,
        (int)Triangle->points[1].y,
        (int)Triangle->points[2].x,
        (int)Triangle->points[2].y,
        Color);
    display_drawLine(
        (int)Triangle->points[2].x,
        (int)Triangle->points[2].y,
        (int)Triangle->points[0].x,
        (int)Triangle->points[0].y,
        Color);

    /* Fill */
    if(Fill)
    {
        /* Sort Points (By Y Coordinate, Ascending) */
        triangle = triangle_sortPoints(Triangle);

        /* Fill Triangle */
        if((int)triangle.points[0].y == (int)triangle.points[1].y)
        {
            /* Triangle Is Already A Flat-Top Triangle */
            display_fillFlatTopTriangle(
                (int)triangle.points[0].x,
                (int)triangle.points[0].y,
                (int)triangle.points[1].x,
                (int)triangle.points[1].y,
                (int)triangle.points[2].x,
                (int)triangle.points[2].y,
                Color
            );
        }
        else if((int)triangle.points[1].y == (int)triangle.points[2].y)
        {
            /* Triangle Is Already A Flat-Bottom Triangle */
            display_fillFlatBottomTriangle(
                (int)triangle.points[0].x,
                (int)triangle.points[0].y,
                (int)triangle.points[1].x,
                (int)triangle.points[1].y,
                (int)triangle.points[2].x,
                (int)triangle.points[2].y,
                Color
            );
        }
        else
        {
            /* Triangle Has Both Flat-Bottom And Top Sub-Triangles; Find Flat-Bottom/Top Points */
            flatPoints[0] = triangle.points[1]; // Not Needed, But Set Anyway For Clarity
            flatPoints[1].x = triangle.points[2].x - triangle.points[0].x;
            flatPoints[1].x *= triangle.points[1].y - triangle.points[0].y;
            flatPoints[1].x /= triangle.points[2].y - triangle.points[0].y;
            flatPoints[1].x += triangle.points[0].x;
            flatPoints[1].y = triangle.points[1].y;

            /* Fill Flat-Bottom Sub-Triangle */
            display_fillFlatBottomTriangle(
                (int)triangle.points[0].x,
                (int)triangle.points[0].y,
                (int)triangle.points[1].x,
                (int)triangle.points[1].y,
                (int)flatPoints[1].x,
                (int)flatPoints[1].y,
                Color);

            /* Fill Flat-Top Sub-Triangle */
            display_fillFlatTopTriangle(
                (int)triangle.points[1].x,
                (int)triangle.points[1].y,
                (int)flatPoints[1].x,
                (int)flatPoints[1].y,
                (int)triangle.points[2].x,
                (int)triangle.points[2].y,
                Color);
        }
    }
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

/****************************************************************************************************
 * Function Definition (Private)
 ****************************************************************************************************/

/*** Fill Flat-Bottom Triangle ***/
static void display_fillFlatBottomTriangle(const int X0, const int Y0, const int X1, const int Y1, const int X2, const int Y2, const uint32_t Color)
{
    double m0, m1, xStart, xEnd;
    int y;

    /*** Fill Flat-Bottom Triangle ***/
    /*                               */
    /*            (X0,Y0)            */
    /*              / \              */
    /*             /   \             */
    /*            /     \            */
    /*           /       \           */
    /*          /         \          */
    /*      (X1,Y1)------(X2,Y2)     */
    /*                               */
    /*********************************/ 
    
    /* Slopes */
    m0 = (double)(X1 - X0) / (Y1 - Y0);
    m1 = (double)(X2 - X0) / (Y2 - Y0);

    /* X Start/End */
    xStart = X0;
    xEnd = X0;

    /* Fill Triangle From Top To Bottom */
    for(y = Y0; y <= Y2; y++)
    {
        display_drawLine((int)xStart, y, (int)xEnd, y, Color);
        xStart += m0;
        xEnd += m1;
    }
}

/*** Fill Flat-Top Triangle ***/
static void display_fillFlatTopTriangle(const int X0, const int Y0, const int X1, const int Y1, const int X2, const int Y2, const uint32_t Color)
{
    double m0, m1, xStart, xEnd;
    int y;

    /*** Fill Flat-Top Triangle ***/
    /*                            */
    /*     (X0,Y0)------(X1,Y1)   */
    /*         \         /        */
    /*          \       /         */
    /*           \     /          */
    /*            \   /           */
    /*             \ /            */
    /*           (X2,Y2)          */
    /*                            */
    /******************************/

    /* Slopes */
    m0 = (double)(X2 - X0) / (Y2 - Y0);
    m1 = (double)(X2 - X1) / (Y2 - Y1);

    /* X Start/End */
    xStart = X2;
    xEnd = X2;

    /* Fill Triangle From Bottom To Top */
    for(y = Y2; y >= Y0; y--)
    {
        display_drawLine((int)xStart, y, (int)xEnd, y, Color);
        xStart -= m0;
        xEnd -= m1;
    }
}
