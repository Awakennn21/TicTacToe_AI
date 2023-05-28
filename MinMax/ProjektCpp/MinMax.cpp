#include "MinMax.h"

MinMax::MinMax(int s, int i)
{
    size = s;
    inRow = i;
}

int MinMax::CountMoves(char** board)
{
    int moveCounter = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (board[i][j] == free)
                moveCounter++;
        }
    }
    return moveCounter;
}
int MinMax::CountOccurrences(char c, std::string input)
{
    int count = 0;
    for (char ch : input)
    {
        if (ch == c)
        {
            count++;
        }
    }
    return count;
}
int MinMax::CountOccurrencesWithoutInterruption(char c, std::string input, char s)
{
    int count = 0;
    int maxCount = 0;
    for (char ch : input)
    {
        if (ch == c)
        {
            count++;
            if (count > maxCount) maxCount = count;
        }
        else if (ch == s)
        {
            if (count > maxCount) maxCount = count;
            count = 0;
        }
    }
    return maxCount;
}
std::vector<std::string> MinMax::GetCombinations(char** board, int n, int rows, int cols)
{
    std::vector<std::string> combinations;

    // Iterate through all positions on the board
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            // Get characters vertically
            if (row + n <= rows)
            {
                std::string verticalCombination = "";
                for (int i = 0; i < n; i++)
                {
                    verticalCombination += board[row + i][col];
                }
                combinations.push_back(verticalCombination);
            }

            // Get characters horizontally
            if (col + n <= cols)
            {
                std::string horizontalCombination = "";
                for (int i = 0; i < n; i++)
                {
                    horizontalCombination += board[row][col + i];
                }
                combinations.push_back(horizontalCombination);
            }

            // Get characters diagonally (top-left to bottom-right)
            if (row + n <= rows && col + n <= cols)
            {
                std::string diagonalCombination = "";
                for (int i = 0; i < n; i++)
                {
                    diagonalCombination += board[row + i][col + i];
                }
                combinations.push_back(diagonalCombination);
            }

            // Get characters diagonally (top-right to bottom-left)
            if (row + n <= rows && col - n + 1 >= 0)
            {
                std::string diagonalCombination = "";
                for (int i = 0; i < n; i++)
                {
                    diagonalCombination += board[row + i][col - i];
                }
                combinations.push_back(diagonalCombination);
            }
        }
    }

    return combinations;
}

int MinMax::CheckForWinner(char** board)
{
    std::vector<std::string> fours = GetCombinations(board, inRow, size, size);

    for (std::string four : fours)
    {
        int player1Chars = CountOccurrencesWithoutInterruption(player1, four, player2);
        //std::cout << player1Chars << ", ";
        int player2Chars = CountOccurrencesWithoutInterruption(player2, four, player1);
        //std::cout << player2Chars << std::endl;
        if (player1Chars >= inRow)
        {
            return 10;
        }
        else if (player2Chars >= inRow)
        {
            return -10;
        }
    }

    return 0;
}

char MinMax::NextPlayer(char player)
{
    return (player == player2) ? player1 : player2;
}

int MinMax::MinMaxTurn(char player, char** board)
{
    //int winner;
    std::pair<int, int> move = GetBestMove(0, false, player, board);
    

    int linear = move.first * size + move.second;
    return move.first * size + move.second;
}

int MinMax::MinMaxFunction(int maxDepth, char** board, int depth, bool isMax, char player, int alpha, int beta)
{
    if (depth == maxDepth)
    {
        return EvaluatePosition(board, isMax, player);
    }

    int score = CheckForWinner(board);

    if (score == 10)
    {
        if (!isMax)
        {
            return score * 10  - depth;
        }
        else
        {
            return -score * 10 + depth;
        }
    }

    if (score == -10)
    {
        if (isMax)
        {
            return score * 10 + depth;
        }
        else
        {
            return -score * 10 - depth;
        }
    }

    if (CountMoves(board) == 0)
    {
        return 0;
    }

    if (isMax)
    {
        int maxScore = -9999999;

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (board[i][j] == free)
                {
                    board[i][j] = NextPlayer(player);

                    int value = MinMaxFunction(maxDepth, board, depth + 1, false, NextPlayer(player), alpha, beta);
                    maxScore = std::max(maxScore, value);
                    alpha = std::max(alpha, maxScore);

                    board[i][j] = free;

                    if (beta <= alpha)
                    {
                        break;
                    }
                }
            }
        }

        return maxScore;
    }
    else
    {
        int maxScore = 9999999;

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (board[i][j] == free)
                {
                    board[i][j] = NextPlayer(player);

                    int value = MinMaxFunction(maxDepth, board, depth + 1, true, NextPlayer(player), alpha, beta);
                    maxScore = std::min(maxScore, value);
                    beta = std::min(beta, maxScore);

                    board[i][j] = free;

                    if (beta <= alpha)
                    {
                        break;
                    }
                }
            }
        }

        return maxScore;
    }
}
int MinMax::EvaluatePosition(char** board, bool isMax, char player)
{
    std::vector<std::string> fours = GetCombinations(board, inRow, size, size);

    int score = 0;
    for (std::string four : fours)
    {
        int player1Chars = CountOccurrences(player, four);
        int player2Chars = CountOccurrences(NextPlayer(player), four);
        if (player2Chars == 0 && player1Chars > 1)
        {
            score += player1Chars;
            if (player1Chars == inRow-1)
            {
                score += 5;
            }
            if (player1Chars == inRow)
            {
                score += 10;
            }
        }
        //else if (player1Chars == 0 && player2Chars != 0)
        //{
        //    score -= player2Chars;
        //   /* if (player2Chars == inRow - 1)
        //    {
        //        score -= 5;
        //    }
        //    if (player2Chars == inRow)
        //    {
        //        score -= 10;
        //    }*/
        //}
    }

    return score;
}

std::pair<int, int> MinMax::GetBestMove(int depth, bool isMax, char player, char** board)
{
    if (CountMoves(board) == size * size)
    {
        std::pair<int, int> bestMove(rand() % size, rand() % size);
        return bestMove;
    }
    std::pair<int, int> bestMove(0, 0);
    int bestScoreMax = -99999;

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (board[i][j] == free)
            {
                board[i][j] = player;

                int score = MinMaxFunction(3, board, depth, isMax, player, -9999, 9999);

                board[i][j] = free;

                if (score > bestScoreMax)
                {
                    bestScoreMax = score;
                    bestMove = std::make_pair(i, j);
                }
            }
        }
    }

    return bestMove;
}


