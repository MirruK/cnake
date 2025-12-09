#include "GameState.hpp"
#include "RenderContext.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_ttf/SDL_ttf.h>

// AI: Thanks gipiti
void SDLTextureDeleter::operator()(SDL_Texture* tx) const noexcept {
  if (tx){
    SDL_DestroyTexture(tx);
  }
};

RenderContext::RenderContext(){};

RenderContext* RenderContext::get_instance(){
  if (!instance) {
    instance = new RenderContext();
  }
  return instance;
};

void RenderContext::render_text_at(std::string key, std::string text, SDL_Color c, SDL_FRect* dst_rect, bool center, bool invalidate) {
  auto texture = this->get_text(key, c, text, invalidate);
  float w, h;
  SDL_GetTextureSize(texture, &w, &h);
  dst_rect->w = w;
  dst_rect->h = h;
  auto temp_rect = SDL_FRect(dst_rect->x, dst_rect->y, dst_rect->w, dst_rect->h);
  if (center) {
    temp_rect.x -= w / 2.0f;
    temp_rect.y -= h / 2.0f;
  }
  SDL_RenderTexture(this->renderer, texture, nullptr, &temp_rect);
}

TTF_Font* RenderContext::get_font(){
  return this->font;
}

void RenderContext::set_font(TTF_Font* font){
  this->font = font;
}

void RenderContext::set_renderer(SDL_Renderer* renderer){
  this->renderer = renderer;
}

std::string RenderContext::update_key(std::string key, TexturePtr texture) {
  this->texture_cache[key] = std::move(texture);
  return key;
}

SDL_Texture* RenderContext::get_text(std::string key, SDL_Color color, const std::string text_string, bool invalidate) {
  if (!invalidate){
    auto it = this->texture_cache.find(key);
    if (it != this->texture_cache.end()){
      return it->second.get();
    }
  }

  SDL_Surface s;
  SDL_Texture* texture;
 /* Create the text */
  auto text = TTF_RenderText_Blended(font, text_string.c_str(), 0, color);
  if (text) {
      texture = SDL_CreateTextureFromSurface(this->renderer, text);
      SDL_DestroySurface(text);
  }
  if (texture == NULL) {
      SDL_Log("Couldn't create text: %s\n", SDL_GetError());
      throw SDL_APP_FAILURE;
  }
  TexturePtr tex = TexturePtr(texture);
  this->update_key(key,std::move(tex));
  return texture;
}

void render_game_container(){
  auto render_ctx = RenderContext::get_instance();
  auto game_ctx = GameContext::get_instance();
  Point board_pos = render_ctx->BOARD_POS;
  Point board_size = game_ctx->BOARD_SIZE * render_ctx->TILE_SIZE;
  auto frect = SDL_FRect {(float)board_pos.x,(float)board_pos.y,(float)board_size.x, (float)board_size.y};
  SDL_SetRenderDrawColor(render_ctx->renderer, 200, 200, 200, 255);
  SDL_RenderRect(render_ctx->renderer, &frect);
}
void render_snake(){
  auto render_ctx = RenderContext::get_instance();
  auto game_ctx = GameContext::get_instance();
  auto snek = game_ctx->get_snake();
  auto snake = snek.lock();
  if (!snek.expired()){
    auto s = snake.get();
    auto body = s->get_body();
    auto ts = render_ctx->TILE_SIZE;
    auto bposx = render_ctx->BOARD_POS.x;
    auto bposy = render_ctx->BOARD_POS.y;
    for(const auto& part : body){
      SDL_FRect r = {(float)part.x*ts + bposx, (float)part.y*ts + bposy, (float)ts, (float)ts};
      SDL_SetRenderDrawColor(render_ctx->renderer, 0, 100, 255, 255);
      SDL_RenderRect(render_ctx->renderer, &r);
    }
  }
  snek.reset();
}

void render_food(){
  auto game_ctx = GameContext::get_instance();
  auto f = game_ctx->get_food();
  auto render_ctx = RenderContext::get_instance();
  auto t = render_ctx->TILE_SIZE;
  auto bp = render_ctx->BOARD_POS;
  for (const auto& food : f) {
    auto r = SDL_FRect{(float)food.x*t + bp.x, (float)food.y*t + bp.y, (float)t, (float)t };
    SDL_SetRenderDrawColor(render_ctx->renderer, 255, 50, 100, 255);
    SDL_RenderRect(render_ctx->renderer, &r);
}
}

void render_game_ui() {
  auto render_ctx = RenderContext::get_instance();
  SDL_FRect text_rect = SDL_FRect();
  SDL_Color text_color;
  SDL_Texture* tx = nullptr;
  GameContext* game_ctx = GameContext::get_instance();
  std::string text_string;
  switch (game_ctx->get_state()) {
  case GameState::START: 
      text_string = "START: Press 1 to play";
      text_color = SDL_Color(200, 60, 70, 255);
      /* Render game title (TOP-MIDDLE)*/
      text_rect.x =  render_ctx->WINDOW_WIDTH / 2.0f;
      text_rect.y =  render_ctx->WINDOW_HEIGHT / 6.0f;
      render_ctx->render_text_at("START", text_string, text_color, &text_rect);
      break;
  case GameState::RUNNING:
      text_string = "RUNNING: Press 2 to pause";
      text_color = SDL_Color(200, 60, 70, 255);
      // RENDER PAUSE TEXT
      text_rect.x = 0;
      text_rect.y = 0;
      render_ctx->render_text_at("RUNNING", text_string, text_color, &text_rect, false);
      // REPOSITION RECT
      text_rect.x += (text_rect.w + 64);
      text_rect.w -= 120;
      // RENDER SCORE COUNTER
      render_ctx->render_text_at("SCORE_COUNTER", std::format("SCORE: {}", game_ctx->get_score()), text_color, &text_rect, false, game_ctx->score_changed);
      // We rendered the changed score and cached the texture,
      // therefore stop triggering invalidations of the texture cache
      game_ctx->score_changed = false;
      break;
  case GameState::PAUSED:
      text_string = "PAUSED: Press 1 to continue";
      text_color = SDL_Color(200, 60, 70, 255);
      text_rect.x =  render_ctx->WINDOW_WIDTH / 2.0f;
      text_rect.y =  render_ctx->WINDOW_HEIGHT / 6.0f;
      render_ctx->render_text_at("PAUSED", text_string, text_color, &text_rect);
      text_string = "PAUSED: Press 3 to return to start";
      text_rect.y += 16+text_rect.h;
      render_ctx->render_text_at("PAUSED_BACK", text_string, text_color, &text_rect);
      break;
  case GameState::FAILED:
      text_string = "FAILED: Press 1 to go to start";
      text_color = SDL_Color(200, 60, 70, 255);
      text_rect.x =  render_ctx->WINDOW_HEIGHT / 2.0f;
      text_rect.y =  render_ctx->WINDOW_HEIGHT / 6.0f;
      render_ctx->render_text_at("FAILED", text_string, text_color,&text_rect);
      break;
  }
}
