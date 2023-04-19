#pragma once
#include <iostream>

namespace aaa {
class GameUiState {
 public:
  GameUiState() : building_depot_(true), building_cc_(false){};

  bool building_depot_;
  bool building_cc_;
};
}  // namespace aaa