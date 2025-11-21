#include "GameState.hpp"
#include "RenderContext.hpp"
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdio>
#include <iostream>
#include <ostream>

void SDLTextureDeleter::operator()(SDL_Texture* tx) const noexcept {
    if (tx){
      std::cout << "Destroying texture with ptr: " << tx << "\n";
      SDL_DestroyTexture(tx);}
};

RenderContext::RenderContext(){};

RenderContext* RenderContext::get_instance(){
  if (!instance) {
    instance = new RenderContext();
  }
  return instance;
};

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
  int n = 1;
  while(this->texture_cache.contains(key)) {
    if (n==1) {
    key.append(std::to_string(n));
    } else{
      // Key is guaranteed to have at least one element!
      key.pop_back();
      key.append(std::to_string(n));
    }
    n++;
  }
  this->texture_cache[key] = std::move(texture);
  return key;
}

SDL_Texture* RenderContext::get_text(std::string key, SDL_Color color, const std::string text_string) {
  auto it = this->texture_cache.find(key);
  if (it != this->texture_cache.end()){
    return it->second.get();
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
  Point board_pos = { render_ctx->WINDOW_WIDTH / 2, render_ctx->WINDOW_HEIGHT / 12};
  // Board size is total height - bottom padding - top padding (height / 12)
  Point board_size = { render_ctx->WINDOW_WIDTH - 60, render_ctx->WINDOW_HEIGHT - (render_ctx->WINDOW_HEIGHT / 6)};
  auto frect = SDL_FRect {(float)board_pos.x,(float)board_pos.y,(float)board_size.x, (float)board_size.y};
  SDL_SetRenderDrawColor(render_ctx->renderer, 200, 200, 200, 255);
  SDL_RenderRect(render_ctx->renderer, &frect);
}
void render_snake(){}


void render_game_ui() {
  auto render_ctx = RenderContext::get_instance();
  SDL_FRect dst_rect1;
  SDL_Color txt_color;
  GameContext* game_ctx = GameContext::get_instance();
  std::string text_string;
  switch (game_ctx->get_state()) {
  case GameState::START: 
      text_string = "START: Press 1 to play";
      txt_color = SDL_Color(200, 60, 70, 255);
      /* Render game title (TOP-MIDDLE)*/
      dst_rect1 = SDL_FRect();
      // 50%
      dst_rect1.w =  render_ctx->WINDOW_WIDTH / 2.0f;
      // 72pt
      dst_rect1.h =  48;
      dst_rect1.x =  0;
      dst_rect1.y =  (render_ctx->WINDOW_HEIGHT / 6.0f);
      SDL_RenderTexture(render_ctx->renderer, render_ctx->get_text("START", txt_color, text_string), NULL, &dst_rect1);
      break;
  case GameState::RUNNING:
      text_string = "RUNNING: Press 2 to pause";
      txt_color = SDL_Color(200, 60, 70, 255);
      dst_rect1 = SDL_FRect();
      dst_rect1.x =  0;
      dst_rect1.y =  0;
      dst_rect1.w =  render_ctx->WINDOW_WIDTH / 8.0f;
      dst_rect1.h =  16;
      SDL_RenderTexture(render_ctx->renderer, render_ctx->get_text("RUNNING", txt_color, text_string), NULL, &dst_rect1);
      dst_rect1.x += (dst_rect1.w + 32);
      SDL_RenderTexture(render_ctx->renderer, render_ctx->get_text("SCORE_COUNTER", txt_color, std::format("SCORE: {}", game_ctx->get_score())), NULL, &dst_rect1);
      
      break;
  case GameState::PAUSED:
      text_string = "PAUSED: Press 3 to continue";
      txt_color = SDL_Color(200, 60, 70, 255);
      dst_rect1 = SDL_FRect();
      dst_rect1.x =  800 / 2.0f;
      dst_rect1.y =  600 / 2.0f;
      dst_rect1.w =  200;
      dst_rect1.h =  28;
      SDL_RenderTexture(render_ctx->renderer, render_ctx->get_text("PAUSED", txt_color, text_string), NULL, &dst_rect1);
      break;
  case GameState::FAILED:
      text_string = "FAILED: Press 1 to go to start";
      txt_color = SDL_Color(200, 60, 70, 255);
      dst_rect1 = SDL_FRect();
      dst_rect1.x =  800 / 2.0f;
      dst_rect1.y =  600 / 2.0f;
      dst_rect1.w =  200;
      dst_rect1.h =  28;
      SDL_RenderTexture(render_ctx->renderer, render_ctx->get_text("FAILED", txt_color, text_string), NULL, &dst_rect1);
      break;
  }
}
