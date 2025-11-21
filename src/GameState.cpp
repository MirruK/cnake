#include <iostream>
#include <map>
#include <utility>
#include "GameState.hpp"

void GameContext::log_game_context() {
    std::cout << (int)this->get_state() << this->score << '\n';
}

std::map<std::pair<GameState, GameState>, GameContext::StateTransitionSideEffect>
GameContext::init_transitions() {
    return {
        { {GameState::START, GameState::RUNNING}, [](GameContext& ctx) {
            ctx.score = 0;
            // TODO: Reinitialize the snake here
            std::cout << "Game started!\n";
        }},
        { {GameState::RUNNING, GameState::PAUSED}, [](GameContext& ctx) {
            std::cout << "Pausing the game...\n";
        }},
        { {GameState::PAUSED, GameState::START}, [](GameContext& ctx) {
            ctx.score = 0;
            std::cout << "Returning to start menu.\n";
        }},
        { {GameState::RUNNING, GameState::FAILED}, [](GameContext& ctx) {
            ctx.score = 0;
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

GameContext::GameContext(): score(0), game_state(GameState::START) {};

GameContext* GameContext::get_instance() {
    if(!instance){
        instance = new GameContext();
    }
    return instance;
}

void GameContext::incrementScore() {
  this->score++;
}

void GameContext::transition(GameState next) {
  auto func = transition_side_effects.find(mktransition(this->game_state, next));
  if (func == transition_side_effects.end()){
    // Invalid or undefined state transition pair
    // TODO: Implement proper logging mechanism with debug logging
    // TODO: Allow resetting of state with a keybind
    std::cout << "Invalid state transition initiated" << '\n'; 
    this->game_state = next;
    return;
  } 
  // Run the side effect registered for this transition
  func->second(*this);
  this->game_state = next;
}
