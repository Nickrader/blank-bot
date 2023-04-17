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

// Things I normally do OnGameXyzabc
// May not translate to code atm

void Bot::OnGameStart() {
  // Set camera-location hotkeys.
  // Set unit-group hotkeys
  Bot::expansions_ =
      sc2::search::CalculateExpansionLocations(Observation(), Query());

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

    const sc2::Unit* gopher = workers[0];

    const sc2::Point2D start = Observation()->GetStartLocation();
    // sc2::Point2D mutalble_target = {start.x + 5, start.y + 5};

    // if (Query()->Placement(sc2::ABILITY_ID::BUILD_SUPPLYDEPOT,
    //                       mutalble_target)) {

    const sc2::Point2D target = DepotPlacement();

    // Error: "Can't find placement location."
    // Didn't see debug sphere, so assume bad target equals error.
    Actions()->UnitCommand(gopher, sc2::ABILITY_ID::BUILD_SUPPLYDEPOT, target);

    Debug()->DebugSphereOut(gopher->pos, 0.5, sc2::Colors::Red);
    Debug()->SendDebug();

    for (const sc2::UnitOrder& order : gopher->orders)
      std::cout << "Order: " << sc2::AbilityTypeToName(order.ability_id)
                << std::endl;
    //} else
    //  std::cout << "Cannot Place: " << std::endl;
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

// for now we cheat, and use an expansion location to test
// sc2::search::CalculateQueries seems to have what we need,
// can strip it down a bit and make use of logic.
// should be able to find/issue buildable locations with what
// we have in sc2_search.cc

const sc2::Point2D Bot::DepotPlacement() {
  sc2::Point2D target;
  for (auto& expo : expansions_) {
    if (Query()->Placement(sc2::ABILITY_ID::BUILD_SUPPLYDEPOT,
                           {expo.x, expo.y})) {
      target = {expo.x, expo.y};
			break;
    }
  }
  return target;
}