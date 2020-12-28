#include "requestutilities.h"

#include <string>

#include "pistache/endpoint.h"
#include "rapidjson/stringbuffer.h"

#include "event.h"

using Pistache::Http::ResponseWriter;
using Pistache::Http::Code;
using Pistache::Http::Header::ContentType;
using Pistache::Http::Mime::MediaType;
using Pistache::Http::Mime::Type;
using Pistache::Http::Mime::Subtype;

using rapidjson::StringBuffer;

using namespace GameTable;

auto RequestUtilities::respondWithJSON(ResponseWriter& res, StringBuffer& sb, Code c) -> void {
  MediaType mime(Type::Application, Subtype::Json);
  res.headers().add<ContentType>(mime);
  res.send(c, sb.GetString());
}
