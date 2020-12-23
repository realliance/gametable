#pragma once

#include <string>
#include <vector>
#include "pistache/endpoint.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

using Pistache::Http::ResponseWriter;
using Pistache::Http::Code;
using rapidjson::StringBuffer;
using rapidjson::Writer;

namespace GameTable {
  namespace RequestUtilities {
    auto respondWithJSON(ResponseWriter& res, StringBuffer& sb, Code c) -> void;

    // Overload Function Mapping for Writing JSON values
    auto writeValue(Writer<StringBuffer>& writer, std::string str) -> void;
    auto writeValue(Writer<StringBuffer>& writer, uint8_t unum) -> void;
    auto writeValue(Writer<StringBuffer>& writer, bool b) -> void;

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
