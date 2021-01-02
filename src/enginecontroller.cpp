#include <vector>
#include <string>
#include <iostream>
#include<iterator>

#include "spdlog/spdlog.h"

#include "enginecontroller.h"
#include "tablemanager.h"
#include "requestutilities.h"

#include "pistache/endpoint.h"
#include "pistache/router.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "controllermanager.h"
#include "statefunctions.h"

#include "libmahjonghttp/jsonhelper.h"

using Mahjong::GetAvailableControllers;
using Mahjong::StartGame;
using Mahjong::GameSettings;

using rapidjson::StringBuffer;
using rapidjson::Writer;
using rapidjson::Document;

using Pistache::Http::Code;

using namespace GameTable;
using namespace MahjongHttp::JSONHelper;

using RequestUtilities::respondWithJSON;

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

  if (TableManager::getInstance().gameRunning) {
    writeValue(writer, "match");
    writer.StartObject();
    writePair(writer, "roundNum", TableManager::getInstance().roundNum);
    writePair(writer, "scores", TableManager::getInstance().scores);
    writePair(writer, "eventLog", TableManager::getInstance().eventLog);
    writer.EndObject();
  } else {
    writePair(writer, "playersRegistered", TableManager::getInstance().numPlayers);
  }

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

  spdlog::debug("Granted Token {}", TableManager::getInstance().playerIDs[playerNumber]);

  TableManager::getInstance().numPlayers++;
  
  if (TableManager::getInstance().numPlayers == 4) {
    std::vector<std::string> players { "Player1", "Player2", "Player3", "Player4" };

    GameSettings settings;
    settings.seatControllers = players;
    StartGame(settings, true);

    TableManager::getInstance().gameRunning = true;
    spdlog::info("Match Started");
  }

  respondWithJSON(res, sb, Code::Ok);
}

auto EngineController::getEventQueue(const Request& req, ResponseWriter res) -> void {
  auto token = req.param(":token").as<std::string>();

  spdlog::debug("Events requested from {}", token);

  StringBuffer sb;
  Writer<StringBuffer> writer(sb);

  uint8_t i = 0;
  for (const auto& id : TableManager::getInstance().playerIDs) {
    if (id.compare(token) == 0) {
      auto playerAI = TableManager::getInstance().playerList[i];
      auto eventQueue = TableManager::getInstance().queuedEvents[i];
      TableManager::getInstance().queuedEvents[i].clear();
      writer.StartObject();
      writePair(writer, "playerID", playerAI->GetPlayerID());
      writeValue(writer, "round");
      writer.StartObject();
      writePair(writer, "startingHand", playerAI->GetStartingHand());
      writePair(writer, "seatWind", (uint8_t) playerAI->GetSeatWind());
      writePair(writer, "prevalentWind", (uint8_t) playerAI->GetPrevalentWind());
      writer.EndObject();
      writePair(writer, "scores", TableManager::getInstance().scores);
      writePair(writer, "waitingOnDecision", playerAI->WaitingOnDecision());
      writePair(writer, "queue", eventQueue);
      writer.EndObject();
      respondWithJSON(res, sb, Code::Ok);
      return;
    }
    i++;
  }

  writer.StartObject();

  writePair(writer, "error", "Player Not Found");

  writer.EndObject();
  respondWithJSON(res, sb, Code::Not_Found);  
}

auto EngineController::onUserDecision(const Request& req, ResponseWriter res) -> void {
  auto token = req.param(":token").as<std::string>();

  spdlog::debug("Decision Posted by token {}", token);

  std::string body = req.body();

  StringBuffer sb;
  Writer<StringBuffer> writer(sb);
  Document d;
  d.Parse(body.c_str());

  if (d.HasParseError() || !canParseEvent(d)) {
    writer.StartObject();
    writePair(writer, "error", "Malformed JSON Body");
    writer.EndObject();
    respondWithJSON(res, sb, Code::Bad_Request);
    return;
  }

  uint8_t i = 0;
  for (const auto& id : TableManager::getInstance().playerIDs) {
    if (id.compare(token) == 0) {
      auto playerAI = TableManager::getInstance().playerList[i];
      auto event = parseEvent(d);
      playerAI->MakeDecision(event);
      writer.StartObject();
      writePair(writer, "receipt", event);
      writer.EndObject();
      respondWithJSON(res, sb, Code::Ok);  
      return;
    }
    i++;
  }

  writer.StartObject();
  writePair(writer, "error", "Player Not Found");
  writer.EndObject();
  respondWithJSON(res, sb, Code::Not_Found);  
}
