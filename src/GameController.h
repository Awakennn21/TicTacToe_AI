#pragma once
#include "MCTS_Algorithm/MCTS_Client_API.h"
#include "MinMax/ProjektCpp/MinMax_Controler.h"
#include "Utilities/Map.h"
#include "Utilities/Timer.h"
#include "Utilities/DataAnalizer.h"
#define DEBUG_LOG


class GameController
{
public:
    enum eAlgorithms
    {
        MCTS = 0, MinMax = 1, None = 2
    };
public:

    GameController(size_t mapSize, uint32_t winCondition, uint32_t numberOfGames, uint32_t iterations = 10000)
        : m_MapSize(mapSize), m_WinCondition(winCondition), m_NumberOfGames(numberOfGames),
        m_MCTS_Client(MCTS_Client_API(mapSize, winCondition, iterations)), m_MinMax_Client(MinMax_Controler(mapSize, winCondition, '1')) {}

    void Simulate(std::vector<eAlgorithms> movesSequence, bool ShouldGatherData = false)
    {
        Map MainMap(m_MapSize);
        MainMap.Clear();

        uint32_t MCTSWins = 0;
        uint32_t MinMaxWins = 0;
        uint32_t Draws = 0;

        for (int i = 0; i < m_NumberOfGames; i++)
        {
            std::optional<double> GameResult;
            uint32_t PlayerToMove = 1;
            uint32_t Iterator = 0;
            float MovesId = 0.0;
            uint32_t Y, X;
            double TimeSpentOnMove = 0.0;

            do
            {
                switch (movesSequence[Iterator])
                {
                case MCTS:
                    std::tie(Y, X, TimeSpentOnMove) = MctsMove(MainMap, PlayerToMove);
                    break;
                case MinMax:
                    std::tie(Y, X, TimeSpentOnMove) = MinMaxMove(MainMap, PlayerToMove);
                    break;
                }
                MainMap.At(Y, X, PlayerToMove);
                if (ShouldGatherData)
                {
                    m_Statistics.LoadData({ {Y,X},{MainMap.GetDimensions(),MainMap.GetDimensions() },m_WinCondition,PlayerToMove,(uint32_t)MovesId,(int)(movesSequence[Iterator]), TimeSpentOnMove });
                    MovesId += 0.5f;
                }
                PlayerToMove = PlayerToMove == 1 ? 2 : 1;
                Iterator = Iterator == movesSequence.size() - 1 ? 0 : Iterator + 1;
#ifdef DEBUG_LOG
                MainMap.Print();
#endif // DEBUG_LOG

            } while ((GameResult = m_MCTS_Client.EvaluateGame()).has_value() == false);

            if (GameResult > 0.0)
            {
                MCTSWins++;
            }
            else if (GameResult < 0.0)
            {
                MinMaxWins++;
            }
            else
            {
                Draws++;
            }

            std::cout << "MCTS won:" << MCTSWins << " times and MinMax won:" << MinMaxWins << " times, " << Draws << " draws" << std::endl;
            m_MCTS_Client.NewGame();
            MainMap.Clear();
        }
    }

    DataAnalizer& GetDataStatistics() { return m_Statistics; }

private:
    MCTS_Client_API m_MCTS_Client;
    MinMax_Controler m_MinMax_Client;
    DataAnalizer m_Statistics;

    uint32_t m_WinCondition;
    uint32_t m_Iterations;
    uint32_t m_MapSize;
    uint32_t m_NumberOfGames;

private:
    std::tuple<int, int, double> MctsMove(Map& map, int playerToMove)
    {
        Timer timer;
        double TimeElapsed = 0.0;

        timer.Start();
        auto [Move, Player] = m_MCTS_Client.NextMove(playerToMove);
        TimeElapsed = timer.Stop();
        TimeElapsed *= 1000.0;

#ifdef DEBUG_LOG
        std::cout << "MCTS placed " << playerToMove << " at " << Move / m_MapSize << "," << Move % m_MapSize << ". Time needed to estimate a move: " << TimeElapsed << "ms" << std::endl;
#endif

        return { Move / m_MapSize, Move % m_MapSize, TimeElapsed };
    }

    std::tuple<int, int, double> MinMaxMove(Map& map, int playerToMove)
    {
        Timer timer;
        double TimeElapsed = 0.0;

        timer.Start();
        auto [mmY, mmX] = m_MinMax_Client.NextMove(map.Data());
        TimeElapsed = timer.Stop();
        TimeElapsed *= 1000.0;
        m_MCTS_Client.FeedInMove(mmY * m_MapSize + mmX, playerToMove);

#ifdef DEBUG_LOG
        std::cout << "MinMax placed " << playerToMove << " at " << mmY << "," << mmX << ". Time needed to estimate a move : " << TimeElapsed << "ms" << std::endl;
#endif

        return { mmY,mmX,TimeElapsed };
    }
};
