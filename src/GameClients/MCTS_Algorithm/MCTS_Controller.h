#pragma once
#include<vector>
#include "../GameClient.h"
#include "MCTS.h"
#include "Timer.h"

class MCTS_Controller : public GameClient
{
public:
    MCTS_Controller(uint32_t mapSize, uint32_t winCondition, uint32_t iterations)
        :m_MctsIterations(iterations),m_MapSize(mapSize), m_Algorithm(mapSize, winCondition), m_Tree(nullptr) {}

    virtual std::tuple<uint32_t, uint32_t, double> NextMove(Map map, int PlayerToMove) override
    {
        Timer Stopwatch;
        double TimeMeasured = 0.0;

        m_Tree = ConvertPosIntoTree(map);

        Stopwatch.Start();
        int SuggestedMove = m_Algorithm.RunMCTS(m_Tree, m_MctsIterations, PlayerToMove);
        TimeMeasured = Stopwatch.Stop();

        return { SuggestedMove / m_MapSize,SuggestedMove % m_MapSize,TimeMeasured };
    }

private:
    MCTS::Node* m_Tree;
    uint32_t m_MctsIterations;
    uint32_t m_MapSize;
    MCTS m_Algorithm;

private:

    MCTS::Node* ConvertPosIntoTree(Map map)
    {
        std::vector<MCTS::Node*> Nodes1;
        std::vector<MCTS::Node*> Nodes2;
        std::vector<MCTS::Node*> Nodes;
        for (int i = 0; i < map.GetDimensions(); i++)
        {
            for (int j = 0; j < map.GetDimensions(); j++)
            {
                if (map.GetAt(i, j) != 0)
                {
                    MCTS::Node* move = new MCTS::Node(0, 0, nullptr, { (int)(i * m_MapSize + j),map.GetAt(i, j) });
                    if (map.GetAt(i, j) == 1)
                    {
                        Nodes1.push_back(move);
                    }
                    else
                    {
                        Nodes2.push_back(move);
                    }
                }
            }
        }
        if (Nodes1.size() > Nodes2.size())
        {
            Nodes.insert(Nodes.end(), Nodes2.begin(), Nodes2.end());
            Nodes.insert(Nodes.end(), Nodes1.begin(), Nodes1.end());
        }
        else
        {
            Nodes.insert(Nodes.end(), Nodes1.begin(), Nodes1.end());
            Nodes.insert(Nodes.end(), Nodes2.begin(), Nodes2.end());
        }
        for (int i=0;i<(int)(Nodes.size())-1;i++)
        {
            Nodes[i]->children.push_back(Nodes[i + 1]);
            Nodes[i + 1]->Parent = Nodes[i];
        }
        return Nodes.size() == 0 ? nullptr : Nodes[Nodes.size() - 1];
    }
};

