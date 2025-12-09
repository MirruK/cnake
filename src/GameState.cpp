#include <iostream>
#include <map>
#include <memory>
#include <unordered_set>
#include <utility>
#include "GameState.hpp"
#include "Common.hpp"
#include "Snake.hpp"

void GameContext::log_game_context() {
    std::cout << (int)this->get_state() << this->score << '\n';
}

std::map<std::pair<GameState, GameState>, GameContext::StateTransitionSideEffect>
GameContext::init_transitions() {
    return {
        { {GameState::START, GameState::RUNNING}, [](GameContext& ctx) {
             ctx.reset_score();
            ctx.food.clear();
            if(ctx.get_snake().expired()) {
                ctx.snake = std::shared_ptr<Snake>(new Snake(4));
            }
            ctx.food.insert(create_food());
            std::cout << "Game started!\n";
        }},
        { {GameState::RUNNING, GameState::PAUSED}, [](GameContext& ctx) {
            std::cout << "Pausing the game...\n";
        }},
        { {GameState::PAUSED, GameState::START}, [](GameContext& ctx) {
             ctx.reset_score();
            std::cout << "Returning to start menu.\n";
            auto s = std::shared_ptr<Snake>(new Snake(4));
            ctx.snake.swap(s);
        }},
        { {GameState::RUNNING, GameState::FAILED}, [](GameContext& ctx) {
             ctx.reset_score();
            //ctx.snake.reset();
            auto s = std::shared_ptr<Snake>(new Snake(4));
            ctx.snake.swap(s);
            std::cout << "Game over!\n";
        }},
        { {GameState::PAUSED, GameState::RUNNING}, [](GameContext& ctx) {
            std::cout << "Resuming game...\n";
        }}
    };
}

GameContext::StateTransitionMap GameContext::transition_side_effects = GameContext::init_transitions();

GameState GameContext::get_state() {
  return this->game_state;
}

int GameContext::get_score() {
    return this->score;
}

std::unordered_set<Point, PointHash>& GameContext::get_food(){
    return this->food;
}

bool check_food(){
    auto game_ctx = GameContext::get_instance();
    auto s = game_ctx->get_snake();
    auto f = &game_ctx->get_food();
    for (const auto& value : *f) {
    // Access key with pair.first and value with pair.second
        if(value ==  s.lock().get()->get_head()) {
            auto res = f->erase(value);
            f->insert(create_food());
            game_ctx->increment_score();
            return true;
    }
    }
    return false;
}

GameContext::GameContext(): score(0), game_state(GameState::START) {};

GameContext* GameContext::get_instance() {
    if(!instance){
        instance = new GameContext();
    }
    return instance;
}

void GameContext::increment_score() {
  this->score++;
  this->score_changed = true;
}

void GameContext::reset_score() {
  this->score = 0;
  this->score_changed = true;
}

void GameContext::transition(GameState next) {
  auto func = transition_side_effects.find(mktransition(this->game_state, next));
  if (func == transition_side_effects.end()){
    // Invalid or undefined state transition pair
    // TODO: Implement proper logging mechanism with debug logging
    std::cout << "Invalid state transition initiated" << '\n'; 
    // this->game_state = next;
    return;
  } 
  // Run the side effect registered for this transition
  func->second(*this);
  this->game_state = next;
}

std::weak_ptr<Snake> GameContext::get_snake() {
    // Q: Should this be a shared or weak pointer? Why?
    return std::weak_ptr<Snake>(this->snake);
}
