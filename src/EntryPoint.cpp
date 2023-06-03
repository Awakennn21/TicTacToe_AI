#include "GameControler.h"

int main()
{

    constexpr uint32_t MapSize = 6;
    constexpr uint32_t WinCondition = 4;
    constexpr uint32_t NumberOfGames = 100;

    GameControler Controller(MapSize, WinCondition);
    Controller.AddClient(GameControler::MinMax);
    Controller.AddClient(GameControler::MCTS);
    Controller.Simulate(NumberOfGames, true);


    DataAnalizer& Analizer = Controller.GetDataStatistics();

    std::cout << "Max MCTS: ";
    std::cout << Analizer.Max<decltype(DataAnalizer::PreciseMoveData::TimeSpendOnThisMove)>
                ([](const DataAnalizer::PreciseMoveData& i) { return i.TimeSpendOnThisMove; },
                [](const DataAnalizer::PreciseMoveData& i) { return i.Player == 1; }).TimeSpendOnThisMove << std::endl;

    std::cout << "Max MinMax: ";
    std::cout << Analizer.Max<decltype(DataAnalizer::PreciseMoveData::TimeSpendOnThisMove)>
                ([](const DataAnalizer::PreciseMoveData& i) { return i.TimeSpendOnThisMove; },
                [](const DataAnalizer::PreciseMoveData& i) { return i.Player == 2; }).TimeSpendOnThisMove << std::endl;

    std::cout << "Mean MCTS: ";
    std::cout << Analizer.Mean<decltype(DataAnalizer::PreciseMoveData::TimeSpendOnThisMove)>
                ([](const DataAnalizer::PreciseMoveData& i) { return i.TimeSpendOnThisMove; },
                [](const DataAnalizer::PreciseMoveData& i) { return i.Player == 1; }) << std::endl;

    std::cout << "Mean MinMax: ";
    std::cout << Analizer.Mean<decltype(DataAnalizer::PreciseMoveData::TimeSpendOnThisMove)>
                ([](const DataAnalizer::PreciseMoveData& i) { return i.TimeSpendOnThisMove; },
                [](const DataAnalizer::PreciseMoveData& i) { return i.Player == 2; }) << std::endl;

    for (int j = 0; j < (MapSize* MapSize+1)/2; j++)
    {
        std::cout << "Mean MCTS"<<"(move:"<<j<<"): ";
        std::cout << Analizer.Mean<decltype(DataAnalizer::PreciseMoveData::TimeSpendOnThisMove)>
                ([](const DataAnalizer::PreciseMoveData& i) { return i.TimeSpendOnThisMove; },
                [j](const DataAnalizer::PreciseMoveData& i) { return i.Player == 1&&i.MovesID==j; }) << std::endl;
    }

    for (int j = 0; j < (MapSize * MapSize + 1) / 2; j++)
    {
        std::cout << "Mean MinMax"<<"(move:"<<j<<"): ";
        std::cout << Analizer.Mean<decltype(DataAnalizer::PreciseMoveData::TimeSpendOnThisMove)>
            ([](const DataAnalizer::PreciseMoveData& i) { return i.TimeSpendOnThisMove; },
                [j](const DataAnalizer::PreciseMoveData& i) { return i.Player == 2 && i.MovesID == j; }) << std::endl;
    }

    return 0;
    
}