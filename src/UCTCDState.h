#pragma once
#include <vector>
#include "sc2api/sc2_api.h"
#include "UCTCDPlayer.h"

class UCTCDAction;
class UCTCDUnit;
class UCTCDValue;
class UCTCDMove;

class UCTCDState {
public:
    // parameters for move generation
    bool attackClosest;
    bool attackWeakest;
    bool attackPriority;
    bool considerDistance;

    // actual state
    float time; //temps du jeu
    UCTCDPlayer playerMin; // joueur ayant fait l'action move pour g�n�rer ce state
    UCTCDPlayer playerMax; // joueur ayant fait l'action move pour g�n�rer ce state

    UCTCDState(UCTCDPlayer playerMin, UCTCDPlayer playerMax, float time, bool attackClosest, bool attackWeakest, bool attackPriority, bool considerDistance);
    void doMove(UCTCDMove & move);
    bool bothCanMove();
    bool playerToMove();
    bool isTerminal();
    std::vector<UCTCDMove> generateMoves(bool isMax);
    int eval();
};