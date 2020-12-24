#include "requestutilities.h"

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

using Mahjong::Event;

using namespace GameTable;

auto RequestUtilities::respondWithJSON(ResponseWriter& res, StringBuffer& sb, Code c) -> void {
  MediaType mime(Type::Application, Subtype::Json);
  res.headers().add<ContentType>(mime);
  res.send(c, sb.GetString());
}

auto RequestUtilities::writeValue(Writer<StringBuffer>& writer, std::string str) -> void {
  writer.String(str.c_str(), static_cast<SizeType>(str.length()));
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
