#pragma once
#include "../Map.h"
#include <random>
#include <vector>
#include <list>
#include <iostream>
#include <cmath>
#include <optional>


class MCTS
{
public:

    struct MoveInfo
    {
        int pos;
        int player;
        MoveInfo(int position, int p)
            :pos(position), player(p) {}
    };

    struct Node
    {
        int Visits;
        double WinsCount;
        double WinPropability;
        Node* Parent;
        MoveInfo Move;
        std::vector<Node*> children;
        Node() : Visits(0), WinsCount(0), Move({ -1,0 }), Parent(nullptr), WinPropability(0) {}
        Node(int visits, int wins, Node* parent, MoveInfo move) : Visits(visits), WinsCount(wins), WinPropability(0), Move(move), Parent(parent) {}
    };

public:

    std::list<Node*> m_AllAlocations;

public:

    MCTS(int mapSize, uint32_t winCondition)
        :MAP_SIZE(mapSize), WIN_CONDITION(winCondition), PlayerToBeAsserted(-1),m_GameState(Map(MAP_SIZE)) {}

    int RunMCTS(Node* state, size_t iterations, int playerToBeAsserted);

    std::optional<double> Evaluate() const;

    void RecreatePos(Node* start);

    void Clear();

private:

    const double CONSTANT_C = 1.5;
    const int RolloutIterations = 4;
    const int MAP_SIZE;
    const uint32_t WIN_CONDITION;
    int PlayerToBeAsserted;

    Map m_GameState;

private:

    void SelectNode(Node* start);

    void Expansion(Node* node);

    void RollOut(Node* start);

    void BackPropagate(Node* start, int wins);

    int ReturnBestMove(Node* root);

    void AnalizeMoves(Node* root);

    size_t EmptySpacesOnMap() const;

    std::optional<double> HasGameEnded(int y, int x);

    uint32_t InRowCount(int posX, int posY, int dx, int dy) const;

    std::vector<MoveInfo> GetValidMoves(Node* target);

    void AddChildNode(Node* target, MoveInfo move);
};
