#include <iostream>
#include <functional>
#include <csignal>
#include <cstdlib>
#include <string>
#include <chrono>
#include <thread>

#include "spdlog/spdlog.h"

#include "pistache/router.h"
#include "pistache/endpoint.h"

#include "controllermanager.h"
#include "enginecontroller.h"
#include "aiproxy.h"
#include "tablemanager.h"
#include "event.h"

using Pistache::Rest::Router;
using Pistache::Rest::Route;
using Pistache::Http::Endpoint;
using Pistache::Address;
using Pistache::Ipv4;
using Pistache::Port;
using Pistache::Tcp::Options;

using Mahjong::RegisterController;
using Mahjong::PlayerController;
using Mahjong::Event;

using namespace Pistache::Rest::Routes;
using namespace GameTable::EngineController;

auto configureRoutes(Router& router) -> void {
  Get(router, "/controllers", bind(&getAvaliableControllers));
  Get(router, "/status", bind(&getGameStatus));
  Get(router, "/events/:token", bind(&getEventQueue));

  Post(router, "/register", bind(&registerForMatch));
  Post(router, "/makedecision/:token", bind(&onUserDecision));
}

void signalHandler(int signal) {
  spdlog::debug("Interrupt signal ({}) received.", signal);

  TableManager::getInstance().exitSignal = signal;
  TableManager::getInstance().serverRunning = false;

  // Force End Clients
  for (auto& eventQueue : TableManager::getInstance().queuedEvents) {
    eventQueue.push_back(Event{Event::Type::End,-1,-1-false});
  }
}

int main () {
    // Configure Signal Handler (for graceful server shutdowns, preventing ghost process port stealing)
    std::signal(SIGINT, signalHandler);

    // Configure Logging (Default to Production)
    spdlog::set_level(spdlog::level::info);
    const char* level = std::getenv("LEVEL");
    if (level != nullptr) {
      const std::string levelStr(level);
      if (levelStr == "DEBUG") {
        spdlog::set_level(spdlog::level::debug);
        spdlog::debug("Logging Set to Debug Level");
      }
    }

    spdlog::info("Starting GameTable");

    const auto PORT = 8080;

    Address addr(Ipv4::any(), Port(PORT));

    Router router;
    configureRoutes(router);

    spdlog::debug("Configured Routes");

    RegisterController([]() { return TableManager::getInstance().playerList[0]; }, "Player1");
    RegisterController([]() { return TableManager::getInstance().playerList[1]; }, "Player2");
    RegisterController([]() { return TableManager::getInstance().playerList[2]; }, "Player3");
    RegisterController([]() { return TableManager::getInstance().playerList[3]; }, "Player4");

    spdlog::debug("Initialized Proxy AIs");

    auto opts = Endpoint::options().threads(4).flags(Options::ReuseAddr);
    Endpoint server(addr);
    server.init(opts);
    server.setHandler(router.handler());

    TableManager::getInstance().serverRunning = true;
    server.serveThreaded();
  
    spdlog::info("Server started on port {}", PORT);
    while (TableManager::getInstance().serverRunning) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (TableManager::getInstance().gameRunning) {
      spdlog::info("Match Complete, Terminating Clients...");
    } else {
      spdlog::info("Terminating Clients...");
    }

    spdlog::debug("playerList size {}", TableManager::getInstance().playerList.size());

    bool clientExited = false;
    // Game Running is true forever once all players are registered
    while (!clientExited && TableManager::getInstance().gameRunning) {
      clientExited = true;
      auto i = 0;
      for (const auto& eventQueue : TableManager::getInstance().queuedEvents) {
        clientExited = eventQueue.size() == 0 && clientExited;
        spdlog::debug("Queue for ID {}: {}", i, eventQueue.size());
        for (const auto& event : eventQueue) {
          spdlog::debug("ID {} Event: {}", i, Mahjong::EventTypeToStr(event.type));
        }
        i++;
      }
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));

    if (TableManager::getInstance().gameRunning) {
      spdlog::info("== Match Results ==");

      auto i = 0;
      for (const auto& score : TableManager::getInstance().scores) {
        spdlog::info("Player {}: {}", i, score);
        i++;
      }
    }

    spdlog::info("Shutting Down, Goodbye");

    server.shutdown();
    exit(TableManager::getInstance().exitSignal);
}
