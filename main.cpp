#include "MCTS_Algorithm/MCTS_Client_API.h"
#include "MinMax/ProjektCpp/MinMax_Controler.h"
#include "Map.h"
#include "Timer.h"
//#define DEBUG_LOG

class GameController
{
public:
    enum eAlgorithms
    {
        MCTS = 0, MinMax = 1, None = 2
    };
public:

    GameController(size_t mapSize, uint32_t winCondition, uint32_t numberOfGames, uint32_t iterations = 10000)
        :   m_MapSize(mapSize), m_WinCondition(winCondition), m_NumberOfGames(numberOfGames),
            m_MCTS_Client(MCTS_Client_API(mapSize, winCondition, iterations)), m_MinMax_Client(MinMax_Controler(mapSize, winCondition, '1')) {}

    void Simulate(std::vector<eAlgorithms> movesSequence)
    {
        Map MainMap(m_MapSize);
        MainMap.Clear();

        int MCTSWins = 0;
        int MinMaxWins = 0;
        int Draws = 0;

        for (int i = 0; i < m_NumberOfGames; i++)
        {
            std::optional<double> GameResult;
            int PlayerToMove = 1;
            int Iterator = 0;
            int Y, X;

            do
            {
                switch (movesSequence[Iterator])
                {
                case MCTS:
                    std::tie(Y, X) = MctsMove(MainMap, PlayerToMove);
                    break;
                case MinMax:
                    std::tie(Y, X) = MinMaxMove(MainMap, PlayerToMove);
                    break;
                }
                MainMap.At(Y, X, PlayerToMove);
                PlayerToMove = PlayerToMove == 1 ? 2 : 1;
                Iterator = Iterator == movesSequence.size() - 1 ? 0 : Iterator+1;
                #ifdef DEBUG_LOG
                                MainMap.Print();
                #endif // DEBUG_LOG

            }
            while ((GameResult = m_MCTS_Client.EvaluateGame()).has_value() == false);

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

            std::cout << "MCTS won:" << MCTSWins << " times and MinMax won:" << MinMaxWins << " times, "<<Draws<<" draws" << std::endl;
            m_MCTS_Client.NewGame();
            MainMap.Clear();
        }
    }

private:
    MCTS_Client_API m_MCTS_Client;
    MinMax_Controler m_MinMax_Client;

    uint32_t m_WinCondition;
    uint32_t m_Iterations;
    uint32_t m_MapSize;
    uint32_t m_NumberOfGames;

private:
    std::pair<int, int> MctsMove(Map& map, int playerToMove)
    {
        Timer timer;
        double TimeElapsed = 0.0;

        timer.Start();
            auto [Move, Player] = m_MCTS_Client.NextMove(playerToMove);
        TimeElapsed = timer.Stop();

        #ifdef DEBUG_LOG
            std::cout << "MCTS placed "<<playerToMove<<" at "<< Move / m_MapSize<<","<< Move % m_MapSize << ". Time needed to estimate a move: " << TimeElapsed * 1000.0 << "ms" << std::endl;
        #endif

        return { Move / m_MapSize, Move % m_MapSize };
    }

    std::pair<int,int> MinMaxMove(Map& map, int playerToMove)
    {
        Timer timer;
        double TimeElapsed = 0.0;

        timer.Start();
            auto [mmY, mmX] = m_MinMax_Client.NextMove(map.Data());
        TimeElapsed = timer.Stop();
        m_MCTS_Client.FeedInMove(mmY * m_MapSize + mmX, playerToMove);

        #ifdef DEBUG_LOG
            std::cout << "MinMax placed " << playerToMove << " at " << mmY << "," << mmX << ". Time needed to estimate a move : " << TimeElapsed * 1000.0 << "ms" << std::endl;
        #endif

        return { mmY,mmX };
    }
};


int main()
{
    constexpr uint32_t MapSize = 3;
    constexpr uint32_t WinCondition = 3;
    constexpr uint32_t NumberOfGames = 100;

    GameController Controller(MapSize, WinCondition, NumberOfGames);

    Controller.Simulate({Controller.MinMax,Controller.MCTS });

    return 0;
    
}
/*
    3x3-3 0/0/100
    4x4-4 1/0/99
    5x5-5 0/0/100
    6x6-5 0/3/97
    6X6-6 0/0/100
*/