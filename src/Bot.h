// The MIT License (MIT)
//
// Copyright (c) 2021-2022 Alexander Kurbatov

#pragma once

#include <sc2api/sc2_agent.h>
#include <sc2api/sc2_common.h>

// struct UiState {
//	UiState() {};
//
//	bool building_depot_ = false;
//	bool building_cc_ = false;
//};
struct UiState;

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

  const sc2::Point2D DepotPlacement();

  std::vector<sc2::Point3D> expansions_;

  UiState ui_state_;
};

// struct UiState {
//	UiState() {};
//
//	bool building_depot_ = false;
//	bool building_cc_ = false;
//};