#ifndef MINMAXCONTROLER_H
#define MINMAXCONTROLER_H

#include <iostream>
#include <string>
#include <vector>
#include <tuple>

class MinMax
{
public:
    char free = '0';
    char player1 = '1';
    char player2 = '2';

    int size;
    int inRow;

    MinMax() {}

    MinMax(int s, int i);

    int CountMoves(char** board);

    int CountOccurrences(char c, std::string input);

    std::vector<std::string> GetCombinations(char** board, int n, int rows, int cols);

    int CheckForWinner(char** board);

    int MinMaxFunction(int maxDepth, char** board, int depth, bool isMax, char player, int alpha, int beta);

    int EvaluatePosition(char** board, bool isMax, char player);

    std::pair<int, int> GetBestMove(int depth, bool isMax, char player, char** board);

    char NextPlayer(char player);

    int MinMaxTurn(char player, char** board);

    int CountOccurrencesWithoutInterruption(char c, std::string input, char s);
};
#endif MINMAXCONTROLER_H
