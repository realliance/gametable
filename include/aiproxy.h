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
    auto WaitingOnDecision() -> bool { return waitingOnDecision; };
    auto MakeDecision(Event) -> void;
    auto GetScores() -> std::vector<int16_t> { return scores; };
    auto GetSeatWind() -> Wind { return seatWind; };
    auto GetPrevalentWind() -> Wind { return prevalentWind; };
    auto GetStartingHand() -> std::vector<Piece> { return startingHand; };
    auto GetPlayerID() -> int { return playerID; };
    auto GetRoundNum() -> uint16_t { return roundNum; };
    auto GetEventLog() -> std::vector<Event> { return eventLog; };
  private:
    int playerID = -1;
    uint16_t roundNum = 0;
    std::string name;
    std::vector<Event> queuedEvents;
    std::vector<int16_t> scores {0,0,0,0};
    Wind seatWind;
    Wind prevalentWind;
    std::vector<Piece> startingHand;
    std::vector<Event> eventLog;
    Event decision;
    bool waitingOnDecision = false;
};
