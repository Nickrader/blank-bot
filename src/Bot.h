// The MIT License (MIT)
//
// Copyright (c) 2021-2022 Alexander Kurbatov

#pragma once

#include <sc2api/sc2_agent.h>
#include <sc2api/sc2_common.h>

// The main bot class.
struct Bot : sc2::Agent {
  Bot() = default;

 private:
  void OnGameStart() final;

  void OnGameEnd() final;

  void OnStep() final;

  void OnBuildingConstructionComplete(const sc2::Unit* building_) final;

  void OnUnitCreated(const sc2::Unit* unit_) final;

  void OnUnitIdle(const sc2::Unit* unit_) final;

  void OnUnitDestroyed(const sc2::Unit* unit_) final;

  void OnUpgradeCompleted(sc2::UpgradeID id_) final;

  void OnError(const std::vector<sc2::ClientError>& client_errors,
               const std::vector<std::string>& protocol_errors = {}) final;

  void BuildDepot();

  void BuildScv();

  bool ToggleBuildDepot = false;

  const sc2::Point2D DepotPlacement();

  std::vector<sc2::Point3D> expansions_;

	GameUiState UiState_;

};

struct GameUiState {
	GameUiState();
	
	int depots_ = 0;
	int CCs_ = 1;
};
