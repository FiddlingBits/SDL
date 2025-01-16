/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include <SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/****************************************************************************************************
 * Variable
 ****************************************************************************************************/

static uint32_t* color_buffer = NULL;
static SDL_Texture* color_buffer_texture = NULL;
static bool is_running = false;
static SDL_Renderer* renderer = NULL;
static SDL_Window* window = NULL;
static int window_width = 0, window_height = 0;

/****************************************************************************************************
 * Function Prototype
 ****************************************************************************************************/

static void cleanup(void);
static void clear_color_buffer(const uint32_t Color);
static void deinitialize(void);
static void draw_grid(const uint32_t Color);
static void draw_rectangle(const int X, const int Y, const int Width, const int Height, const uint32_t Color);
static bool initialize(void);
static void process_input(void);
static void render(void);
static void render_color_buffer(void);
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

    /* Cleanup */
    cleanup();

    /*** Deinitialize ***/
    deinitialize();

    return 0;
}

/****************************************************************************************************
 * Function Definition (Private)
 ****************************************************************************************************/

/*** Cleanup ***/
static void cleanup(void)
{
    /*** Cleanup ***/
    free(color_buffer);
    SDL_DestroyTexture(color_buffer_texture);
}

/*** Clear Color Buffer ***/
static void clear_color_buffer(const uint32_t Color)
{
    int col, row;

    /*** Clear Color Buffer ***/
    for(row = 0; row < window_height; row++)
        for(col = 0; col < window_width; col++)
            color_buffer[(row * window_width) + col] = Color;
}

/*** Deinitialize ***/
static void deinitialize(void)
{
    /*** Deinitialize ***/
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

/*** Draw Grid ***/
static void draw_grid(const uint32_t Color)
{
    int col, row;

    /*** Draw Grid ***/
    for(row = 0; row < window_height; row++)
    {
        for(col = 0; col < window_width; col++)
        {
            if(((row % 10) == 0) || ((col % 10) == 0))
                color_buffer[(row * window_width) + col] = Color;
        }
    }
}

/*** Draw Rectangle ***/
static void draw_rectangle(const int X, const int Y, const int Width, const int Height, const uint32_t Color)
{
    int col, row;

    /*** Draw Rectangle ***/
    for(col = X; col < (X + Width); col++)
        for(row = Y; row < (Y + Height); row++)
            color_buffer[(row * window_width) + col] = Color;
}

/*** Initialize ***/
static bool initialize(void)
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
    window_width = display_mode.w;
    window_height = display_mode.h;

    /* Window */
    window = SDL_CreateWindow("Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, 0);
    if(!window)
    {
        (void)printf("SDL_CreateWindow Failed: %s\n", SDL_GetError());
        return false;
    }
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    /* Renderer */
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
    (void)SDL_PollEvent(&event);

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
    /* Clear */
    (void)SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    (void)SDL_RenderClear(renderer);

    /* Draw */
    draw_grid(0xFFFFFFFF);
    draw_rectangle(100, 50, 250, 125, 0xFFFF0000);

    /* Render */
    render_color_buffer();
    SDL_RenderPresent(renderer);

    /* Reset */
    clear_color_buffer(0xFF000000);
}

/*** Render Color Buffer ***/
static void render_color_buffer(void)
{
    /*** Render Color Buffer ***/
    (void)SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, sizeof(*color_buffer) * window_width);
    (void)SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

/*** Setup ***/
static void setup(void)
{
    /*** Setup ***/
    color_buffer = malloc(sizeof(*color_buffer) * window_width * window_height);
    color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);
}

/*** Update ***/
static void update(void)
{
    /*** Update ***/
}
