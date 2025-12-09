#pragma once

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <Common.hpp>

void render_game_ui();
void render_snake();
void render_food();
void render_game_container();

struct SDLTextureDeleter {
  void operator()(SDL_Texture* tx) const noexcept;
};

// Convenience type that creates a unique pointer that manages the texture lifetime
using TexturePtr = std::unique_ptr<SDL_Texture, SDLTextureDeleter>;

class RenderContext {
  public:
    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;
    Point BOARD_POS;
    int TILE_SIZE;
    SDL_Renderer* renderer;
    TTF_Font* get_font();
    void set_font(TTF_Font* font);
    void set_renderer(SDL_Renderer* renderer);
    std::string update_key(std::string str, TexturePtr texture);
    SDL_Texture* get_text(std::string key, SDL_Color color, const std::string text_string, bool invalidate = false);
    void render_text_at(std::string key, std::string text, SDL_Color c, SDL_FRect* text_rect, bool center = true, bool invalidate = false);
    static RenderContext* get_instance();
  private:
    RenderContext();
    static RenderContext* instance;
    TTF_Font* font;
    std::unordered_map<std::string, TexturePtr> texture_cache;
};
