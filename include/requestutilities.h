#pragma once

#include "../build/_deps/libmahjong-src/include/event.h"

#include <string>
#include <vector>
#include "pistache/endpoint.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

using Pistache::Http::ResponseWriter;
using Pistache::Http::Code;
using rapidjson::StringBuffer;
using rapidjson::Writer;

using Mahjong::Event;

namespace GameTable {
  namespace RequestUtilities {
    auto respondWithJSON(ResponseWriter&, StringBuffer&, Code) -> void;

    // Overload Function Mapping for Writing JSON values
    auto writeValue(Writer<StringBuffer>&, std::string) -> void;
    auto writeValue(Writer<StringBuffer>&, unsigned int) -> void;
    auto writeValue(Writer<StringBuffer>&, int) -> void;
    auto writeValue(Writer<StringBuffer>&, bool) -> void;
    auto writeValue(Writer<StringBuffer>&, Event) -> void;

    template <typename T>
    auto writeValue(Writer<StringBuffer>& writer, std::vector<T> list) -> void {
        writer.StartArray();
        for (const auto& value : list) {
          writeValue(writer, value);
        }
        writer.EndArray();
    }

    // Pair writing using above functions
    template <typename V>
    auto writePair(Writer<StringBuffer>& writer, std::string key, V value) -> void {
      writeValue(writer, key);
      writeValue(writer, value);
    }
  }
}
