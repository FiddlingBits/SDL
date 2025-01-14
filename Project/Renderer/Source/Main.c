/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>

/****************************************************************************************************
 * Variable
 ****************************************************************************************************/

static bool is_running = false;
static SDL_Renderer* renderer = NULL;
static SDL_Window* window = NULL;

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

static bool initialize(void);
static void process_input(void);
static void render(void);
static void setup(void);
static void update(void);

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** Main Entry ***/
int main(int argc, char *argv[])
{
    /*** Initialize ***/
    is_running = initialize();

    /*** Run ***/
    /* Setup */
    setup();

    /* Run */
    while(is_running)
    {
        /* Render */
        process_input();
        update();
        render();
    }

    return 0;
}

/****************************************************************************************************
 * Function Definition (Private)
 ****************************************************************************************************/

/*** Initialize ***/
static bool initialize(void)
{
    /*** Initialize ***/
    /* Initialize */
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        (void)printf("SDL_Init Failed: %s\n", SDL_GetError());
        return false;
    }

    /* Create Window */
    window = SDL_CreateWindow("Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, 0);
    if(!window)
    {
        (void)printf("SDL_CreateWindow Failed: %s\n", SDL_GetError());
        return false;
    }

    /* Create Renderer */
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer)
    {
        (void)printf("SDL_CreateRenderer Failed: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

/*** Process Input ***/
static void process_input(void)
{
    SDL_Event event;

    /*** Process Input ***/
    /* Get Input */
    SDL_PollEvent(&event);

    /* Handle Input */
    switch(event.type)
    {
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE)
                is_running = false;
            break;
        case SDL_QUIT:
            is_running = false;
            break;
    }
}

/*** Render ***/
static void render(void)
{
    /*** Render ***/
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

/*** Setup ***/
static void setup(void)
{
    /*** Setup ***/
}

/*** Update ***/
static void update(void)
{
    /*** Update ***/
}
