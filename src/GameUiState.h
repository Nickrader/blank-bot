#pragma once
#include <iostream>

namespace aaa {
class GameUiState {
 public:
  GameUiState() : building_depot_(false), building_cc_(false), depot_counter_(0){};

  bool building_depot_;
  bool building_cc_;

	int depot_counter_;
};
}  // namespace aaa