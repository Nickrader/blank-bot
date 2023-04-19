// The MIT License (MIT)
//
// Copyright (c) 2021-2022 Alexander Kurbatov

#include "Bot.h"

#include <sc2api/sc2_api.h>
#include <sc2api/sc2_common.h>
#include <sc2api/sc2_unit.h>
#include <sc2api/sc2_unit_filters.h>
#include <sc2lib/sc2_search.h>

#include <iostream>

#include "GameUiState.h"

// Things I normally do OnGameXyzabc
// May not translate to code atm

void Bot::OnGameStart() {
  // Set camera-location hotkeys.
  // Set unit-group hotkeys
  Bot::expansions_ =
      sc2::search::CalculateExpansionLocations(Observation(), Query());

  auto aa = Observation()->GetUnits(sc2::IsTownHall());
  Bot::main_ = aa[0]->pos;

  std::cout << "New game started!" << std::endl;
}

void Bot::OnGameEnd() {
  // I hit 'pause' then quit the game.  j/k ;)
  std::cout << "Game over!" << std::endl;
}

void Bot::OnBuildingConstructionComplete(const sc2::Unit* building_) {
  // check to see if unit is queued up.
  // !:? check resources, queue up unit
  // set Rally point(s)

  if (building_->unit_type == sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT)
    ui_state_.building_depot_ = false;

  std::cout << sc2::UnitTypeToName(building_->unit_type) << "("
            << building_->tag << ") constructed" << std::endl;
}

void Bot::OnStep() {
  // Should probably be more like Alerts in a scripted AI
  // OnStep for a hooman is many steps of a game.

  // check minimap {i.e. bottom-left}
  // check saturation/production {i.e. check wireframe}
  // check resources/supply {i.e. top-right}
  // check mainscreen, do actions {i.e. the fun part}
  // a.k.a. The Circle of Starcraft.
  BuildScv();
  BuildDepot();

  int loop = Observation()->GetGameLoop();
  if (loop % 100 == 0) std::cout << "OnStep: " << loop << std::endl;
}

void Bot::OnUnitCreated(const sc2::Unit* unit_) {
  //  Ideally should be rallying.
  //  Shouldn't need command until reaches destination.
  //  At times would need 'attack' command issued as rally is 'move'

  std::cout << sc2::UnitTypeToName(unit_->unit_type) << "(" << unit_->tag
            << ") was created" << std::endl;
}

void Bot::OnUnitIdle(const sc2::Unit* unit_) {
  // if worker, we want to get buildings/mining ... unless waiting to build
  // sumthgin

  std::cout << sc2::UnitTypeToName(unit_->unit_type) << "(" << unit_->tag
            << ") is idle" << std::endl;
}

void Bot::OnUnitDestroyed(const sc2::Unit* unit_) {
  // Could change plans (running to certain death)
  // I usually try to locate camera there unless already there, or know I don't
  // care
  //

  std::cout << sc2::UnitTypeToName(unit_->unit_type) << "(" << unit_->tag
            << ") was destroyed" << std::endl;
}

void Bot::OnUpgradeCompleted(sc2::UpgradeID id_) {
  std::cout << sc2::UpgradeIDToName(id_) << " completed" << std::endl;
}

void Bot::OnError(const std::vector<sc2::ClientError>& client_errors,
                  const std::vector<std::string>& protocol_errors) {
  for (const auto i : client_errors) {
    std::cerr << "Encountered client error: " << static_cast<int>(i)
              << std::endl;
  }

  for (const auto& i : protocol_errors)
    std::cerr << "Encountered protocol error: " << i << std::endl;
}

void Bot::BuildDepot() {
  if (!ui_state_.building_depot_) {
    auto food = Observation()->GetFoodUsed();
    auto supply = Observation()->GetFoodCap();
    auto mineralz = Observation()->GetMinerals();
    if (food >= supply && mineralz >= 100) {
      ui_state_.building_depot_ = true;
      auto workers =
          Observation()->GetUnits(sc2::Unit::Alliance::Self, sc2::IsWorker());

      const sc2::Unit* gopher = workers[1];

      const sc2::Point2D start = Observation()->GetStartLocation();

      const sc2::Point2D target = DepotPlacement();

      Actions()->UnitCommand(gopher, sc2::ABILITY_ID::BUILD_SUPPLYDEPOT,
                             target);

      Debug()->DebugSphereOut(gopher->pos, 0.5, sc2::Colors::Red);
      Debug()->SendDebug();

      for (const sc2::UnitOrder& order : gopher->orders)
        std::cout << "Order: " << sc2::AbilityTypeToName(order.ability_id)
                  << std::endl;
    }
  }
}

void Bot::BuildScv() {
  sc2::Units units =
      Observation()->GetUnits(sc2::Unit::Alliance::Self, sc2::IsTownHall());
  for (auto unit : units) {
    if (unit->orders.size() == 0) {
      Actions()->UnitCommand(unit, sc2::ABILITY_ID::TRAIN_SCV);
    }
  }
}

const sc2::Point2D Bot::DepotPlacement() {
  sc2::Point2D target;

  // We've proven sort with DistanceSqaured, now need to do this with 'valid'
  // locations in main.
  std::sort(expansions_.begin(), expansions_.end(),
            [this](sc2::Point3D a, sc2::Point3D b) {
              float dista = sc2::DistanceSquared2D(main_, a);
              float distb = sc2::DistanceSquared2D(main_, b);
              return dista < distb;
            });

  for (auto& a : expansions_) {
    float dist = sc2::DistanceSquared2D(main_, a);
    std::cout << a.x << '\t' << a.y << '\t' << a.z << '\t' << dist << std::endl;
  }

  // Gather candidate locations {x,y}
  // locs in Main
  //
  //// This could be done several ways.
  ////  1: pathing from main to other expo, take where path drops in z axis.
  /// give us ramp /  2: radiate out from main check z, /  ??? do all these
  /// create borders?  Is some heuristic radius distance that main generally
  /// encompasses.???

  // CONSTRAINTS::
  // locs not in mineral line
  // preferably closest to ramp

  ++ui_state_.depot_counter_;
  target = expansions_[ui_state_.depot_counter_];
  return target;
}