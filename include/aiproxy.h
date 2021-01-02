#pragma once
#include <event.h>
#include <piecetype.h>
#include <playercontroller.h>
#include <winds.h>

#include <cstddef>
#include <string>
#include <vector>

using Mahjong::PlayerController;
using Mahjong::Piece;
using Mahjong::Event;
using Mahjong::Wind;

class AIProxy : public PlayerController {
  public:
    AIProxy(std::string n);
    auto Name() -> std::string override;
    auto GameStart(int _playerID) -> void override;
    auto RoundStart(std::vector<Piece> hand, Wind seatWind, Wind prevalentWind) -> void override;
    auto ReceiveEvent(Event e) -> void override;
    auto RetrieveDecision() -> Event override;
    auto PopEventQueue() -> std::vector<Event>;
    auto EventQueueSize() -> uint8_t;
    auto WaitingOnDecision() -> bool { return waitingOnDecision; };
    auto MakeDecision(Event) -> void;
    auto GetSeatWind() -> Wind { return seatWind; };
    auto GetPrevalentWind() -> Wind { return prevalentWind; };
    auto GetStartingHand() -> std::vector<Piece> { return startingHand; };
    auto GetPlayerID() -> int { return playerID; };
  private:
    int playerID = -1;
    std::string name;
    std::vector<Event> queuedEvents;
    Wind seatWind;
    Wind prevalentWind;
    std::vector<Piece> startingHand;
    Event decision;
    bool waitingOnDecision = false;
};
