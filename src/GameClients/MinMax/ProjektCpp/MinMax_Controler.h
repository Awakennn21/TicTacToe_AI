#pragma once
#include "MinMax.h"
#include "../GameClients/GameClient.h"
#include <vector>

class  MinMax_Controler : public GameClient
{
public:
	struct Point
	{
		int y;
		int x;
	};
	MinMax m_MinMax;
	char m_Player;
	
	MinMax_Controler(int mapSize, int inRow, char currentPlayer)
	{
		m_MapSize = mapSize;
		m_InRow = inRow;
		m_Player = currentPlayer;
		m_MinMax = MinMax(mapSize, inRow);
	}
	

	Point NextMove(char** board)
	{
		int suggestedMove = m_MinMax.MinMaxTurn(m_Player, board);
		Point ReturnValue = { suggestedMove / m_MapSize,suggestedMove % m_MapSize };
		return ReturnValue;
	}

	std::tuple<uint32_t, uint32_t,double> NextMove(class Map map, int PlayerToMove)
	{
		uint32_t dimensions = map.GetDimensions();
		int** board = new int* [dimensions];
		for (int i = 0; i < dimensions; i++)
		{
			board[i] = new int[dimensions];
		}
		for (int i = 0; i < dimensions; i++)
		{
			for (int j = 0; j < dimensions; j++)
			{
				board[i][j] = map.GetAt(i, j);
			}
		}
		Timer Chrono;
		double TimeElapsed = 0.0;
		Chrono.Start();
		int suggestedMove = m_MinMax.MinMaxTurn(m_Player, convertBoard(board, m_MapSize, m_MapSize));
		TimeElapsed = Chrono.Stop();
		for (int i = 0; i < dimensions; i++)
		{
			delete [] board[i];
		}
		delete[] board;
		return { suggestedMove / m_MapSize,suggestedMove % m_MapSize,TimeElapsed };
	}
	
	int EvaluateGame(char** board)
	{
		return m_MinMax.CheckForWinner(board);
	}

	int CountFreeSpaces(char** board)
	{
		return m_MinMax.CountMoves(board);
	}


private:
	int m_MapSize;
	int m_InRow;

	char** convertBoard(int** intBoard, int numRows, int numCols) {
		char** charBoard = new char* [numRows];

		for (int i = 0; i < numRows; i++) {
			charBoard[i] = new char[numCols];
			for (int j = 0; j < numCols; j++) {
				charBoard[i][j] = static_cast<char>(intBoard[i][j] + 48);
			}
		}

		return charBoard;
	}
};
