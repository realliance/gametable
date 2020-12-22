#pragma once

#include <map>
#include <string>
#include <vector>
#include <random>

#include "aiproxy.h"
#include "playercontroller.h"

using Mahjong::PlayerController;

class TableManager {
  private:
    TableManager() {
      ResetProxies();
    };
  public:
    static TableManager& getInstance() {
      static TableManager instance;
      return instance;
    }

    std::map<std::string, PlayerController*> playerMap;
    uint8_t numPlayers = 0;
    bool gameRunning = false;

    bool serverRunning = false;
    int exitSignal;

    std::vector<AIProxy*> playerList;
    std::vector<std::string> playerIDs;

    void ResetProxies() {
      playerList.clear();
      playerList.reserve(4);

      playerIDs.clear();
      playerIDs.reserve(4);

      std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

      std::random_device rd;
      std::mt19937 generator(rd());

      for (uint8_t i = 0; i < 4; i++) {
        std::shuffle(str.begin(), str.end(), generator);
        playerIDs[i] = str.substr(0, 25);
      }

      playerList[0] = new AIProxy("Player1");
      playerList[1] = new AIProxy("Player2");
      playerList[2] = new AIProxy("Player3");
      playerList[3] = new AIProxy("Player4");
    }

    TableManager(TableManager const&) = delete;
    void operator=(TableManager const&)  = delete;

    //void StartGame();
};
