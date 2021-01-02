#include "aiproxy.h"

#include "tablemanager.h"
#include "spdlog/spdlog.h"
#include <string>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>

AIProxy::AIProxy(std::string n) {
  name = n;
}

auto AIProxy::Name() -> std::string {
  return name;
}

auto AIProxy::GameStart(int id) -> void {
  playerID = id;
}

auto AIProxy::RoundStart(std::vector<Piece> h, Wind sW, Wind pW) -> void {
  startingHand = h;
  seatWind = sW;
  prevalentWind = pW;
  if (playerID == 0) {
    TableManager::getInstance().roundNum++;
    spdlog::info("Round {} Started", TableManager::getInstance().roundNum);
  }
}

auto AIProxy::ReceiveEvent(Event e) -> void {
  if (e.type == Event::Type::PointDiff && playerID == 0) {
    TableManager::getInstance().scores[e.player] += e.piece;
    spdlog::debug("Added {} to player {}", e.piece, e.player);
  }

  if (!e.decision) {
    TableManager::getInstance().eventLog.push_back(e);
  }

  waitingOnDecision = e.decision;
  TableManager::getInstance().queuedEvents[playerID].push_back(e);

  if (e.type == Event::Type::End && playerID == 0) {
    spdlog::debug("End Event Found, Beginning Shutdown");
    TableManager::getInstance().serverRunning = false;
  }
}

auto AIProxy::RetrieveDecision() -> Event {
  while (waitingOnDecision) {
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  return decision;
}

auto AIProxy::MakeDecision(Event e) -> void { 
  decision = e;
  waitingOnDecision = false;
}
