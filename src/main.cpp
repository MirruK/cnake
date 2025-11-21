#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>
#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>
#include <unistd.h>
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "RenderContext.hpp"
#include "GameState.hpp"
#include "Snake.hpp"
/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
RenderContext* RenderContext::instance = nullptr;
GameContext* GameContext::instance = nullptr;
static int frame_count = 0;
// Desired frametime in milliseconds
constexpr static double desired_frametime = (1/120.0f) * 1000;

using namespace std::chrono;
auto timer = high_resolution_clock::now();

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("cnake", "1.0", "cnake");
    int width = 640*2;
    int height = 480*2;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("cnake", width, height, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

        if (!TTF_Init()) {
        SDL_Log("Couldn't initialize SDL_ttf: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    TTF_Font *font = TTF_OpenFont("/home/mirek/.local/share/fonts/AzeretMono-Regular.ttf", 72.0f);
    
    if(font == NULL) {
        auto err = SDL_GetError();
        std::cout << err;
        return SDL_APP_FAILURE;
    }

    auto render_ctx = RenderContext::get_instance();
    render_ctx->set_font(font);
    render_ctx->set_renderer(renderer);
    render_ctx->WINDOW_HEIGHT = height;
    render_ctx->WINDOW_WIDTH = width;

    auto game_ctx = GameContext::get_instance();
    // TODO: Setup game context
    Food::set_position_bounds(Point{width, height});

    SDL_SetRenderLogicalPresentation(renderer, width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    auto game_ctx = GameContext::get_instance();
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    if (event->key.key == SDLK_1 && event->key.down){
        game_ctx->transition(GameState::RUNNING);
    }
    if (event->key.key == SDLK_2 && event->key.down){
        game_ctx->transition(GameState::PAUSED);
    }
    if (event->key.key == SDLK_3 && event->key.down){
        game_ctx->transition(GameState::START);
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    auto render_ctx = RenderContext::get_instance();
    render_game_ui();

    SDL_RenderPresent(renderer);

    frame_count =  (frame_count + 1) % 10;
    std::this_thread::sleep_for(std::chrono::milliseconds((int)desired_frametime));

    if (frame_count == 0) {
        auto tmp = std::chrono::high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(tmp - timer);
        timer = tmp;
        // std::cout << (10.0f / (float)(duration.count() * std::pow(10, -6))) << "FPS" << std::endl;
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */

    auto render_ctx = RenderContext::get_instance();
    TTF_CloseFont(render_ctx->get_font());
}
