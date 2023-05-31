#pragma once
#include<vector>
#include "MCTS.h"

class MCTS_Controller
{
    MCTS m_Algorithm;

private:
    std::vector<MCTS::MoveInfo> m_MovesHistory;
    MCTS::Node* m_Tree;
    uint32_t m_MctsIterations;
    uint32_t m_MapSize;

public:
    MCTS_Controller(uint32_t mapSize, uint32_t winCondition, uint32_t iterations)
        :m_MctsIterations(iterations),m_MapSize(mapSize), m_Algorithm(mapSize, winCondition), m_Tree(nullptr) {}

    void FeedInMove(int move,int player)
    {
        m_MovesHistory.push_back({ move,player });
        m_Tree = ConvertPosIntoTree(m_MovesHistory);
    }

    std::tuple<int,int> NextMove(int PlayerToMove)
    {
        m_Tree = ConvertPosIntoTree(m_MovesHistory);
        int SuggestedMove = m_Algorithm.RunMCTS(m_Tree, m_MctsIterations, PlayerToMove);

        m_MovesHistory.push_back({ SuggestedMove, PlayerToMove });
        return { SuggestedMove,PlayerToMove };
    }

    void NewGame()
    {
        std::shared_ptr<MCTS::Node> Tree(m_Tree);
        m_Algorithm.Clear();
        m_MovesHistory.clear();
        Tree.reset();
    }

    std::optional<double> EvaluateGame()
    {
        m_Algorithm.Clear();
        m_Tree = ConvertPosIntoTree(m_MovesHistory);
        m_Algorithm.RecreatePos(m_Tree);
        std::optional<double> GameResult = m_Algorithm.Evaluate();
        if (m_Tree != nullptr)
        {
            return GameResult;
        }
        return {};
    }

private:

    MCTS::Node* ConvertPosIntoTree(std::vector<MCTS::MoveInfo> moves)
    {
        if (moves.size() > 0)
        {
            std::vector<MCTS::Node*> Nodes;
            for (int i = 0; i < moves.size(); i++)
            {
                MCTS::Node* move = new MCTS::Node(0, 0, nullptr, moves[i]);
                Nodes.push_back(move);
            }
            for (int i = 0; i < Nodes.size() - 1; i++)
            {
                Nodes[i]->children.push_back(Nodes[i + 1]);
                Nodes[i + 1]->Parent = Nodes[i];
            }
            return Nodes[Nodes.size() - 1];
        }
        else
        {
            return nullptr;
        }
    }
};

