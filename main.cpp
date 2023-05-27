#include "MCTS_Algorithm/MCTS_Client_API.h"
#include "MinMax/ProjektCpp/MinMax_Controler.h"
#include "Map.h"


constexpr size_t MapSize = 3;
constexpr int WinCondition = 3;
constexpr int Iterations = 10000;
constexpr uint32_t NumberOfGames = 5;


int main()
{
    Map MainMap(MapSize);
    MainMap.Clear();

    MCTS_Client_API MCTSClient(MapSize, WinCondition, Iterations);

    int MCTSWins = 0;
    int MinMaxWins = 0;
    int PlayerToMove = 2;

    for (int i = 0; i < NumberOfGames; i++)
    {
        MinMax_Controler MM(MapSize, 3, PlayerToMove == 1 ? 2 : 1);
        std::optional<double> GameResult;
        while (true)
        {

            PlayerToMove = PlayerToMove == 1 ? 2 : 1;
            auto[Move,Player] = MCTSClient.NextMove(PlayerToMove);
            MainMap.At(Move / MapSize, Move % MapSize, Player);


            std::cout << "MCTS\n";
            MainMap.Print();

            GameResult = MCTSClient.EvaluateGame();

            if (GameResult.has_value())
            {
                if (GameResult != 0)
                {
                    MCTSWins++;
                }
                break;
            }

            PlayerToMove = PlayerToMove == 1 ? 2 : 1;
            auto[mmY,mmX] = MM.NextMove(MainMap.Data());
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
                break;
            }
        }

        std::cout << "MCTS won:" << MCTSWins << " times and MinMax won:" << MinMaxWins << " times" << std::endl;
        MCTSClient.NewGame();
        MainMap.Clear();
    }

    return 0;
    
}