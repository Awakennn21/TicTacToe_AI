#include "MCTS_Algorithm/MCTS_Client_API.h"
#include "MinMax/ProjektCpp/MinMax_Controler.h"
#include "Map.h"


constexpr size_t MapSize = 6;
constexpr int WinCondition = 5;
constexpr int Iterations = 20000;
constexpr uint32_t NumberOfGames = 10;

void MCTSvsMCTS()
{
    Map MainMap(MapSize);
    MainMap.Clear();

    MCTS_Client_API MCTSClient(MapSize, WinCondition, Iterations);

    int MCTSWins = 0;
    int Draws = 0;
    int PlayerToMove = 2;

    for (int i = 0; i < NumberOfGames; i++)
    {
        std::optional<double> GameResult;

        while (true)
        {
            PlayerToMove = PlayerToMove == 1 ? 2 : 1;
            auto [Move, Player] = MCTSClient.NextMove(PlayerToMove);
            MainMap.At(Move / MapSize, Move % MapSize, Player);


            std::cout << "MCTS " << Move << std::endl;;
            MainMap.Print();

            GameResult = MCTSClient.EvaluateGame();

            if (GameResult.has_value())
            {
                if (GameResult != 0)
                {
                    MCTSWins++;
                }
                else
                {
                    Draws++;
                }
                break;
            }
        }

        std::cout << "MCTS won:" << MCTSWins << " times against itself and drew "<<Draws<<" times" << std::endl;
        MCTSClient.NewGame();
        MainMap.Clear();
        PlayerToMove = 2;
    }

}

void MinMaxvsMCTS()
{
    Map MainMap(MapSize);
    MainMap.Clear();

    MCTS_Client_API MCTSClient(MapSize, WinCondition, Iterations);
    MinMax_Controler MM(MapSize, WinCondition, '1');

    int MCTSWins = 0;
    int MinMaxWins = 0;
    int Draws = 0;

    int PlayerToMove = 2;

    for (int i = 0; i < NumberOfGames; i++)
    {
        std::optional<double> GameResult;

        while (true)
        {
            PlayerToMove = PlayerToMove == 1 ? 2 : 1;
            auto [mmY, mmX] = MM.NextMove(MainMap.Data());
            MCTSClient.FeedInMove(mmY * MapSize + mmX, PlayerToMove);
            MainMap.At(mmY, mmX, PlayerToMove);


            std::cout << "MinMax\n";
            MainMap.Print();

            GameResult = MCTSClient.EvaluateGame();

            if (GameResult.has_value())
            {
                if (GameResult != 0)
                {
                    MinMaxWins++;
                }
                else
                {
                    Draws++;
                }
                break;
            }

            PlayerToMove = PlayerToMove == 1 ? 2 : 1;
            auto [Move, Player] = MCTSClient.NextMove(PlayerToMove);
            MainMap.At(Move / MapSize, Move % MapSize, Player);


            std::cout << "MCTS " << Move << std::endl;;
            MainMap.Print();

            GameResult = MCTSClient.EvaluateGame();

            if (GameResult.has_value())
            {
                if (GameResult != 0)
                {
                    MCTSWins++;
                }
                else
                {
                    Draws++;
                }
                break;
            }
        }

        std::cout << "MCTS won:" << MCTSWins << " times and MinMax won:" << MinMaxWins << " times, "<<Draws<<" draws" << std::endl;
        MCTSClient.NewGame();
        MainMap.Clear();
        PlayerToMove = 2;
    }
}

int main()
{
    //MinMaxvsMCTS();
    MCTSvsMCTS();
    return 0;
    
}