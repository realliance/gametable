#pragma once

#include "pistache/router.h"

using Pistache::Rest::Router;

namespace GameTable {
  auto configureRoutes(Router&) -> void;
  int main();
}
