#pragma once

#include <functional>
#include <map>
#include <memory>
#include <vector>
#include "Snake.hpp"

enum class GameState {
  START,
  RUNNING,
  PAUSED,
  FAILED,
};


class GameContext {
  constexpr auto mktransition(GameState curr, GameState next){
    return std::make_pair(curr, next);
  }

  public:
    int score;
    void incrementScore();
    void transition(GameState next);
    GameState get_state();
    int get_score();
    void log_game_context();
    static GameContext* get_instance();

  private: 
    static GameContext* instance;
    GameContext();
    GameState game_state;
    std::unique_ptr<Snake> snake;
    std::vector<Food> food;
    using StateTransitionMap = std::map<std::pair<GameState, GameState>, std::function<void(GameContext&)>>;
    using StateTransitionSideEffect = std::function<void(GameContext&)>;

    static StateTransitionMap transition_side_effects;

    static StateTransitionMap init_transitions();
};

