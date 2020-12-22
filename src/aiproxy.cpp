#include "aiproxy.h"

#include <string>
#include <vector>

AIProxy::AIProxy(std::string n) {
  name = n;
}

auto AIProxy::Name() -> std::string {
  return name;
}

auto AIProxy::GameStart(int id) -> void {
  playerID = id;
}

auto AIProxy::RoundStart(std::vector<Piece> h, Wind sW, Wind pW) -> void {
  startingHand = h;
  seatWind = sW;
  prevalentWind = pW;
}

auto AIProxy::ReceiveEvent(Event e) -> void {
  queuedEvents.push_back(e);
}

auto AIProxy::RetrieveDecision() -> Event {
  waitingOnDecision = true;
  while (waitingOnDecision);
  return decision;
}

auto AIProxy::PopEventQueue() -> std::vector<Event> {
  auto events = queuedEvents;
  queuedEvents.clear();
  return events;
}
