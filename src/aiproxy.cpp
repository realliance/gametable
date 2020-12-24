#include "aiproxy.h"

#include <string>
#include <vector>
#include <algorithm>

AIProxy::AIProxy(std::string n) {
  name = n;
  std::fill(scores.begin(), scores.end(), 250);
}

auto AIProxy::Name() -> std::string {
  return name;
}

auto AIProxy::GameStart(int id) -> void {
  playerID = id;
}

auto AIProxy::RoundStart(std::vector<Piece> h, Wind sW, Wind pW) -> void {
  roundNum++;
  startingHand = h;
  seatWind = sW;
  prevalentWind = pW;
}

auto AIProxy::ReceiveEvent(Event e) -> void {
  if (e.type == Event::Type::PointDiff) {
    scores[e.player] += e.piece;
  }
  if (!e.decision) {
    eventLog.push_back(e);
  }
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

auto AIProxy::MakeDecision(Event e) -> void { 
  decision = e;
  waitingOnDecision = false;
}
