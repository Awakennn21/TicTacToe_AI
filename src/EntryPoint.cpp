#include "GameController.h"

int main()
{

    constexpr uint32_t MapSize = 4;
    constexpr uint32_t WinCondition = 4;
    constexpr uint32_t NumberOfGames = 10;

    GameController Controller(MapSize, WinCondition, NumberOfGames);

    Controller.Simulate({Controller.MCTS, Controller.MinMax },true);

    DataAnalizer& Analizer = Controller.GetDataStatistics();
    Analizer.FilterData([](DataAnalizer::PreciseMoveData i) {return i.MovesID == 2; });

    std::cout << "Mean:\n";
    std::cout << Analizer.CalculateTimeMean([](DataAnalizer::PreciseMoveData i) { return i.Player == 1; });

    return 0;
    
}

/*
    3x3-3 0/0/100
    4x4-4 1/0/99
    5x5-5 0/0/100
    6x6-5 0/3/97
    6X6-6 0/0/100
*/