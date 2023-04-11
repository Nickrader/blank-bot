// The MIT License (MIT)
//
// Copyright (c) 2021-2022 Alexander Kurbatov

#include "Bot.h"

#include <sc2api/sc2_api.h>
#include <sc2api/sc2_common.h>
#include <sc2api/sc2_unit.h>
#include <sc2api/sc2_unit_filters.h>

#include <iostream>

// Things I normally do OnGameXyzabc
// May not translate to code atm

void Bot::OnGameStart() {
  // Set camera-location hotkeys.
  // Set unit-group hotkeys

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
  std::cout << "OnStep: " << loop << std::endl;
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
  // if supply is less than some number
  auto food = Observation()->GetFoodUsed();
  auto supply = Observation()->GetFoodCap();
  if (food >= supply) {
    auto workers =
        Observation()->GetUnits(sc2::Unit::Alliance::Self, sc2::IsWorker());
    // Observation()->GetUnit(workers[0]->tag); // redundant
    const sc2::Unit* gopher = workers[0];
    // need position to build.
    // point direction enemy
    // offset Townhall loc by constant

    const sc2::Units cc_all =
        Observation()->GetUnits(sc2::Unit::Alliance::Self, sc2::IsTownHall());
    const sc2::Unit* cc_main = cc_all[0];


    Actions()->UnitCommand(gopher, sc2::ABILITY_ID::BUILD_SUPPLYDEPOT,
                           {gopher->pos.x, gopher->pos.y});

    Debug()->DebugSphereOut(gopher->pos, 0.5, sc2::Colors::Yellow);
    Debug()->SendDebug();

    for (const sc2::UnitOrder& order : gopher->orders)
      std::cout << "Order: " << sc2::AbilityTypeToName(order.ability_id)
                << std::endl;
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
