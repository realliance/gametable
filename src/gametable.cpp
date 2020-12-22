#include <iostream>
#include <functional>
#include <csignal>

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
  std::cout << "Interrupt signal (" << signal << ") received." << std::endl;

  TableManager::getInstance().exitSignal = signal;
  TableManager::getInstance().serverRunning = false;
}

int main () {
    std::cout << "Starting GameTable" << std::endl;

    const auto PORT = 8080;

    Address addr(Ipv4::any(), Port(PORT));

    Router router;
    configureRoutes(router);

    std::cout << "Configured Routes" << std::endl;

    RegisterController([]() { return TableManager::getInstance().playerList[0]; }, "Player1");
    RegisterController([]() { return TableManager::getInstance().playerList[1]; }, "Player2");
    RegisterController([]() { return TableManager::getInstance().playerList[2]; }, "Player3");
    RegisterController([]() { return TableManager::getInstance().playerList[3]; }, "Player4");

    std::cout << "Initialized AIs" << std::endl;

    auto opts = Endpoint::options().threads(1);
    Endpoint server(addr);
    server.init(opts);
    server.setHandler(router.handler());

    server.serveThreaded();

    TableManager::getInstance().serverRunning = true;
    std::cout << "Server started on port " << PORT << std::endl;
    while (TableManager::getInstance().serverRunning);

    std::cout << "Shutting down..." << std::endl;

    server.shutdown();
    exit(TableManager::getInstance().exitSignal);
}
