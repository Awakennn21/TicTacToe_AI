#include "GameController.h"

int main()
{

    constexpr uint32_t MapSize = 3;
    constexpr uint32_t WinCondition = 3;
    constexpr uint32_t NumberOfGames = 10;

    GameController Controller(MapSize, WinCondition, NumberOfGames);

    Controller.Simulate({Controller.MCTS, Controller.MinMax },true);


    DataAnalizer& Analizer = Controller.GetDataStatistics();
    //Analizer.FilterData([](DataAnalizer::PreciseMoveData i) {return i.MovesID == 3; });

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

    for (int j = 0; j <5; j++)
    {
        std::cout << "Mean MCTS"<<"(move:"<<j<<"): ";
        std::cout << Analizer.Mean<decltype(DataAnalizer::PreciseMoveData::TimeSpendOnThisMove)>
            ([](const DataAnalizer::PreciseMoveData& i) { return i.TimeSpendOnThisMove; },
                [j](const DataAnalizer::PreciseMoveData& i) { return i.Player == 1&&i.MovesID==j; }) << std::endl;
    }

    for (int j = 0; j < 5; j++)
    {
        std::cout << "Mean MinMax"<<"(move:"<<j<<"): ";
        std::cout << Analizer.Mean<decltype(DataAnalizer::PreciseMoveData::TimeSpendOnThisMove)>
            ([](const DataAnalizer::PreciseMoveData& i) { return i.TimeSpendOnThisMove; },
                [j](const DataAnalizer::PreciseMoveData& i) { return i.Player == 2 && i.MovesID == j; }) << std::endl;
    }

    return 0;
    
}

/*
    3x3-3 0/0/100
    4x4-4 1/0/99
    5x5-5 0/0/100
    6x6-5 0/3/97
    6X6-6 0/0/100
*/