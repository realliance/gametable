#pragma once

#include "pistache/router.h"

using Pistache::Rest::Request;
using Pistache::Http::ResponseWriter;

namespace GameTable {
  namespace EngineController {
      auto getAvaliableControllers(const Request&, ResponseWriter) -> void;
      auto getGameStatus(const Request&, ResponseWriter) -> void;
      auto registerForMatch(const Request&, ResponseWriter) -> void;
  }
}
