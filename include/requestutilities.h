#pragma once

#include "event.h"
#include "piecetype.h"

#include <string>
#include <vector>
#include "pistache/endpoint.h"
#include "rapidjson/stringbuffer.h"

using Pistache::Http::ResponseWriter;
using Pistache::Http::Code;

using rapidjson::StringBuffer;

namespace GameTable {
  namespace RequestUtilities {
    auto respondWithJSON(ResponseWriter&, StringBuffer&, Code) -> void;
  }
}
