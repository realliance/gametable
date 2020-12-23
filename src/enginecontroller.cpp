#include <vector>
#include <string>
#include <iostream>

#include "enginecontroller.h"
#include "tablemanager.h"
#include "requestutilities.h"

#include "pistache/endpoint.h"
#include "pistache/router.h"

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "controllermanager.h"
#include "statefunctions.h"

using Mahjong::GetAvailableControllers;
using Mahjong::StartGame;
using Mahjong::GameSettings;

using rapidjson::StringBuffer;
using rapidjson::Writer;

using Pistache::Http::Code;

using namespace GameTable;

using RequestUtilities::respondWithJSON;
using RequestUtilities::writePair;
using RequestUtilities::writeValue;

auto EngineController::getAvaliableControllers(const Request& req, ResponseWriter res) -> void {
  auto controllers = GetAvailableControllers();

  StringBuffer sb;
  Writer<StringBuffer> writer(sb);

  writeValue(writer, controllers);

  respondWithJSON(res, sb, Code::Ok);
}

auto EngineController::getGameStatus(const Request& req, ResponseWriter res) -> void {
  StringBuffer sb;
  Writer<StringBuffer> writer(sb);

  writer.StartObject();

  writePair(writer, "gameRunning", TableManager::getInstance().gameRunning);
  writePair(writer, "playersRegistered", TableManager::getInstance().numPlayers);

  writer.EndObject();

  respondWithJSON(res, sb, Code::Ok);
}

auto EngineController::registerForMatch(const Request& req, ResponseWriter res) -> void {
  StringBuffer sb;
  Writer<StringBuffer> writer(sb);

  if (TableManager::getInstance().gameRunning) {
    writer.StartObject();

    writePair(writer, "error", "Game Already Running");

    writer.EndObject();
    respondWithJSON(res, sb, Code::Bad_Request);
    return;
  }

  auto playerNumber = TableManager::getInstance().numPlayers;

  writer.StartObject();

  writePair(writer, "playerToken", TableManager::getInstance().playerIDs[playerNumber]);

  writer.EndObject();

  TableManager::getInstance().numPlayers++;
  
  if (TableManager::getInstance().numPlayers == 4) {
    std::vector<std::string> players { "Player1", "Player2", "Player3", "Player4" };

    GameSettings settings;
    settings.seatControllers = players;
    StartGame(settings, true);

    TableManager::getInstance().gameRunning = true;
  }

  respondWithJSON(res, sb, Code::Ok);
}
