#include <vector>
#include <string>
#include <iostream>

#include "enginecontroller.h"
#include "tablemanager.h"

#include "pistache/endpoint.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "controllermanager.h"
#include "statefunctions.h"

using Mahjong::GetAvailableControllers;
using Mahjong::StartGame;
using Mahjong::GameSettings;

using rapidjson::StringBuffer;
using rapidjson::Writer;
using rapidjson::SizeType;

using Pistache::Http::Code;
using Pistache::Http::Header::ContentType;
using Pistache::Http::Mime::MediaType;
using Pistache::Http::Mime::Type;
using Pistache::Http::Mime::Subtype;

using namespace GameTable;

auto EngineController::getAvaliableControllers(const Request& req, ResponseWriter res) -> void {
  auto controllers = GetAvailableControllers();

  StringBuffer sb;
  Writer<StringBuffer> writer(sb);

  writer.StartArray();
  for (const auto& controller : controllers) {
    writer.String(controller.c_str(), static_cast<SizeType>(controller.length()));
  }
  writer.EndArray();

  MediaType mime(Type::Application, Subtype::Json);
  res.headers().add<ContentType>(mime);
  res.send(Code::Ok, sb.GetString());
}

auto EngineController::getGameStatus(const Request& req, ResponseWriter res) -> void {
  StringBuffer sb;
  Writer<StringBuffer> writer(sb);

  writer.StartObject();
  writer.String("gameRunning");
  writer.Bool(TableManager::getInstance().gameRunning);

  writer.String("playersRegistered");
  writer.Uint(TableManager::getInstance().numPlayers);
  writer.EndObject();

  MediaType mime(Type::Application, Subtype::Json);
  res.headers().add<ContentType>(mime);
  res.send(Code::Ok, sb.GetString());
}

auto EngineController::registerForMatch(const Request& req, ResponseWriter res) -> void {
  StringBuffer sb;
  Writer<StringBuffer> writer(sb);

  if (TableManager::getInstance().gameRunning) {
    writer.StartObject();
    writer.String("error");
    writer.String("Game Already Running");
    writer.EndObject();
    MediaType mime(Type::Application, Subtype::Json);
    res.headers().add<ContentType>(mime);
    res.send(Code::Bad_Request, sb.GetString());
    return;
  }

  auto playerNumber = TableManager::getInstance().numPlayers;

  writer.StartObject();
  writer.String("playerToken");
  writer.String(
    TableManager::getInstance().playerIDs[playerNumber].c_str(),
    static_cast<SizeType>(TableManager::getInstance().playerIDs[playerNumber].length())
  );
  writer.EndObject();

  TableManager::getInstance().numPlayers++;
  
  if (TableManager::getInstance().numPlayers == 4) {
    std::vector<std::string> players { "Player1", "Player2", "Player3", "Player4" };

    GameSettings settings;
    settings.seatControllers = players;
    StartGame(settings, true);

    TableManager::getInstance().gameRunning = true;
  }

  MediaType mime(Type::Application, Subtype::Json);
  res.headers().add<ContentType>(mime);
  res.send(Code::Ok, sb.GetString());
}
