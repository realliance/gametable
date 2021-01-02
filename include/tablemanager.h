#pragma once

#include <map>
#include <string>
#include <vector>
#include <random>

#include "spdlog/spdlog.h"

#include "aiproxy.h"
#include "playercontroller.h"

using Mahjong::PlayerController, Mahjong::Event;

class TableManager {
  private:
    TableManager() {
      ResetProxies();
      scores.resize(4);
      std::fill(scores.begin(), scores.end(), 250);
      queuedEvents.resize(4);
    };
  public:
    static TableManager& getInstance() {
      static TableManager instance;
      return instance;
    }

    std::map<std::string, PlayerController*> playerMap;
    uint8_t numPlayers = 0;
    uint8_t roundNum = 0;
    bool gameRunning = false;

    bool serverRunning = false;
    int exitSignal;

    std::vector<AIProxy*> playerList;
    std::vector<std::string> playerIDs;
    std::vector<int32_t> scores;
    std::vector<std::vector<Event>> queuedEvents;
    std::vector<Event> eventLog;

    void ResetProxies() {
      spdlog::debug("Reset Proxies Called");
      playerList.clear();
      playerList.reserve(4);

      playerIDs.clear();

      std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

      std::random_device rd;
      std::mt19937 generator(rd());

      for (uint8_t i = 0; i < 4; i++) {
        std::shuffle(str.begin(), str.end(), generator);
        playerIDs.push_back(str.substr(0, 25));
      }

      spdlog::debug("playerIDs size: {}", playerIDs.size());

      playerList[0] = new AIProxy("Player1");
      playerList[1] = new AIProxy("Player2");
      playerList[2] = new AIProxy("Player3");
      playerList[3] = new AIProxy("Player4");
    }

    TableManager(TableManager const&) = delete;
    void operator=(TableManager const&)  = delete;

    //void StartGame();
};
