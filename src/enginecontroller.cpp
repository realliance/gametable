#include "enginecontroller.h"

#include "pistache/endpoint.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "controllermanager.h"

using Mahjong::GetAvailableControllers;

using rapidjson::StringBuffer;
using rapidjson::Writer;
using rapidjson::SizeType;

using Pistache::Http::Code;
using Pistache::Http::Header::ContentType;
using Pistache::Http::Mime::MediaType;
using Pistache::Http::Mime::Type;
using Pistache::Http::Mime::Subtype;

using namespace GameTable;

auto EngineController::getAvaliableControllers(const Request& req, ResponseWriter res) -> void {
  auto controllers = GetAvailableControllers();

  StringBuffer sb;
  Writer<StringBuffer> writer(sb);

  writer.StartArray();
  for (const auto& controller : controllers) {
    writer.String(controller.c_str(), static_cast<SizeType>(controller.length()));
  }
  writer.EndArray();

  MediaType mime(Type::Application, Subtype::Json);

  res.headers().add<ContentType>(mime);
  res.send(Code::Ok, sb.GetString());
}
