#pragma once
#include "MinMax.h"

class  MinMax_Controler
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

	Point NextMove(int** board)
	{
		int suggestedMove = m_MinMax.MinMaxTurn(m_Player, convertBoard(board, m_MapSize, m_MapSize));
		Point ReturnValue = { suggestedMove / m_MapSize,suggestedMove % m_MapSize };
		return ReturnValue;
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
