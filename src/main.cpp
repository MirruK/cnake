#include "Snake.hpp"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_render.h>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <thread>
#include <unistd.h>
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "RenderContext.hpp"
#include "GameState.hpp"
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

    TTF_Font *font = TTF_OpenFont("/home/mirek/.local/share/fonts/IosevkaTerm-Medium.ttf", 72.0f);
    
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
    Point board_pos = { render_ctx->WINDOW_WIDTH / 12, render_ctx->WINDOW_HEIGHT / 12};
    render_ctx->BOARD_POS = board_pos;
    // Board size is total height - bottom padding - top padding (height / 12)
    Point board_size = {50,50};
    // Tile size is chosen such that the game board fits in the window
    render_ctx->TILE_SIZE = p_min(Point {render_ctx->WINDOW_WIDTH - 50, render_ctx->WINDOW_HEIGHT - 50} / board_size);
    game_ctx->BOARD_SIZE = board_size;
    std::cout << "Tile size: " << render_ctx->TILE_SIZE << '\n';
    std::cout << "Board pos: " << render_ctx->BOARD_POS << '\n';

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
    if (event->key.down){
        Snake* s;
        switch (event->key.key) {
            case SDLK_UP:
                s = game_ctx->get_snake().lock().get();
                if (s->get_direction() == SnakeDirection::DOWN) {
                    break;
                }
                s->set_direction(SnakeDirection::UP);
                break;
            case SDLK_RIGHT:
                s = game_ctx->get_snake().lock().get();
                if (s->get_direction() == SnakeDirection::LEFT) {
                    break;
                }
                s->set_direction(SnakeDirection::RIGHT);
                break;
            case SDLK_DOWN:
                s = game_ctx->get_snake().lock().get();
                if (s->get_direction() == SnakeDirection::UP) {
                    break;
                }
                s->set_direction(SnakeDirection::DOWN);
                break;
            case SDLK_LEFT:
                s = game_ctx->get_snake().lock().get();
                if (s->get_direction() == SnakeDirection::RIGHT) {
                    break;
                }
                s->set_direction(SnakeDirection::LEFT);
                break;
            default:
            break;
        }
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
    auto game_ctx = GameContext::get_instance();
    SDL_SetRenderDrawColor(render_ctx->renderer, 30, 30, 30, 255);
    SDL_RenderClear(render_ctx->renderer);
    render_game_ui();
    if (game_ctx->get_state() == GameState::RUNNING){
        if (frame_count == 0){
            auto s= game_ctx->get_snake().lock().get();
            s->step();
            int c = 0;
            for (auto part : s->get_body()) {
                if (c != 0 && part == s->get_body().front()) {
                    game_ctx->transition(GameState::FAILED);
                    // Q: Does this release the weak_ptr lock?
                    goto skip_render;
                }
                c++;
            }
            if(check_food()) {
                s->grow();
            }
        }
        render_game_container();
        render_food();
        render_snake();
        skip_render:
    }

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
