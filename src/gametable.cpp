#include "pistache/router.h"
#include "pistache/endpoint.h"

#include "enginecontroller.h"

using Pistache::Rest::Router;
using Pistache::Rest::Route;
using Pistache::Http::Endpoint;
using Pistache::Address;
using Pistache::Ipv4;
using Pistache::Port;

using namespace Pistache::Rest::Routes;
using namespace GameTable::EngineController;

auto configureRoutes(Router& router) -> void {
  Get(router, "/controllers", bind(&getAvaliableControllers));
}

int main () {
    Address addr(Ipv4::any(), Port(8080));

    Router router;
    configureRoutes(router);

    auto opts = Endpoint::options().threads(1);
    Endpoint server(addr);
    server.init(opts);
    server.setHandler(router.handler());
    server.serve();
}
