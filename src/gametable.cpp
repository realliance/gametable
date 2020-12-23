#include <iostream>
#include <functional>
#include <csignal>
#include <cstdlib>
#include <string>

#include "spdlog/spdlog.h"

#include "pistache/router.h"
#include "pistache/endpoint.h"

#include "controllermanager.h"
#include "enginecontroller.h"
#include "aiproxy.h"
#include "tablemanager.h"

using Pistache::Rest::Router;
using Pistache::Rest::Route;
using Pistache::Http::Endpoint;
using Pistache::Address;
using Pistache::Ipv4;
using Pistache::Port;

using Mahjong::RegisterController;
using Mahjong::PlayerController;

using namespace Pistache::Rest::Routes;
using namespace GameTable::EngineController;

auto configureRoutes(Router& router) -> void {
  Get(router, "/controllers", bind(&getAvaliableControllers));
  Get(router, "/status", bind(&getGameStatus));
  Post(router, "/register", bind(&registerForMatch));
}

void signalHandler(int signal) {
  spdlog::debug("Interrupt signal ({}) received.", signal);

  TableManager::getInstance().exitSignal = signal;
  TableManager::getInstance().serverRunning = false;
}

int main () {
    // Configure Logging (Default to Production)
    spdlog::set_level(spdlog::level::info);
    const char* level = std::getenv("LEVEL");
    if (level != nullptr) {
      const std::string levelStr(level);
      if (levelStr == "DEBUG") {
        spdlog::set_level(spdlog::level::debug);
        spdlog::debug("Logging Set to Debug");
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

    spdlog::debug("Initialized AIs");

    auto opts = Endpoint::options().threads(1);
    Endpoint server(addr);
    server.init(opts);
    server.setHandler(router.handler());

    server.serveThreaded();

    TableManager::getInstance().serverRunning = true;
    spdlog::info("Server started on port {}", PORT);
    while (TableManager::getInstance().serverRunning);

    spdlog::debug("Shutting Down...");

    server.shutdown();
    exit(TableManager::getInstance().exitSignal);
}
