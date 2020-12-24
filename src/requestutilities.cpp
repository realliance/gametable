#include "requestutilities.h"

#include <string>

#include "spdlog/spdlog.h"
#include "pistache/endpoint.h"
#include "rapidjson/stringbuffer.h"

#include "../build/_deps/libmahjong-src/include/event.h"

using Pistache::Http::ResponseWriter;
using Pistache::Http::Code;
using Pistache::Http::Header::ContentType;
using Pistache::Http::Mime::MediaType;
using Pistache::Http::Mime::Type;
using Pistache::Http::Mime::Subtype;

using rapidjson::StringBuffer;
using rapidjson::SizeType;
using rapidjson::Document;
using rapidjson::Value;

using Mahjong::Event;

using namespace GameTable;

auto RequestUtilities::respondWithJSON(ResponseWriter& res, StringBuffer& sb, Code c) -> void {
  MediaType mime(Type::Application, Subtype::Json);
  res.headers().add<ContentType>(mime);
  res.send(c, sb.GetString());
}

auto RequestUtilities::canParseEvent(Document& d) -> bool {
  // Check Membership
  if (!d.HasMember("type") || !d.HasMember("player") || !d.HasMember("piece") || !d.HasMember("decision")) {
    spdlog::debug("JSON failed membership test.");
    return false;
  }

  auto& type = d["type"];
  auto& player = d["player"];
  auto& piece = d["piece"];
  auto& decision = d["decision"];

  // Check Type
  if (!type.IsUint() || !player.IsInt() || !piece.IsInt() || !decision.IsBool()) {
    spdlog::debug("JSON Failed Type Checking.");
    return false;
  }

  // Check Type Range
  auto typeValue = type.GetUint();
  if (typeValue < Event::Type::Ron || typeValue > Event::Type::End) {
    spdlog::debug("JSON Failed Event Type Range");
    return false;
  }

  return true;
}

auto RequestUtilities::parseEvent(Document& d) -> Event {
  return Event {
    (Event::Type) d["type"].GetUint(),
    d["player"].GetInt(),
    (int16_t) d["piece"].GetInt(),
    d["decision"].GetBool(),
  };
}

auto RequestUtilities::writeValue(Writer<StringBuffer>& writer, std::string str) -> void {
  writer.String(str.c_str(), static_cast<SizeType>(str.length()));
}

auto RequestUtilities::writeValue(Writer<StringBuffer>& writer, const char* chrStr) -> void {
  writer.String(chrStr, static_cast<SizeType>(std::strlen(chrStr)));
}

auto RequestUtilities::writeValue(Writer<StringBuffer>& writer, unsigned int unum) -> void {
  writer.Uint(unum);
}

auto RequestUtilities::writeValue(Writer<StringBuffer>& writer, int num) -> void {
  writer.Int(num);
}

auto RequestUtilities::writeValue(Writer<StringBuffer>& writer, bool b) -> void {
  writer.Bool(b);
}

auto RequestUtilities::writeValue(Writer<StringBuffer>& writer, Event event) -> void {
  writer.StartObject();
  writePair(writer, "type", (unsigned int) event.type);
  writePair(writer, "player", event.player);
  writePair(writer, "piece", event.piece);
  writePair(writer, "decision", event.decision);
  writer.EndObject();
}

auto RequestUtilities::writeValue(Writer<StringBuffer>& writer, Piece p) -> void {
  writer.Uint(p.raw_value());
}
