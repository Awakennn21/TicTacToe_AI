#pragma once
#include <iostream>
#include <vector>
#include <optional>

class Map
{
public:
	Map()
		:m_Map(NULL), m_MapDimension(-1) {}

	Map(size_t dimension)
		:m_MapDimension(dimension)
	{
		m_Map = std::vector<std::vector<int>>(dimension);
		for (size_t i = 0; i < m_MapDimension; i++)
		{
			m_Map[i] = std::vector<int>(dimension);
		}
	}

	size_t GetDimensions() const { return m_MapDimension; }
	void At(int i, int j, int value) { m_Map[i][j] = value; }
	int GetAt(int i, int j) const { return m_Map[i][j]; }

	void Print()
	{
		for (int i = 0; i < m_MapDimension; i++)
		{
			for (int j = 0; j < m_MapDimension; j++)
			{
				std::cout << m_Map[i][j];
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	void Clear()
	{
		for (int i = 0; i < m_MapDimension; i++)
		{
			for (int j = 0; j < m_MapDimension; j++)
			{
				m_Map[i][j] = 0;
			}
		}
	}

	std::optional<int> EvaluateGame(uint32_t winCondition)
	{
		uint32_t EmptySpaces = 0;
		for (int y = 0; y < m_MapDimension; y++)
		{
			for (int x = 0; x < m_MapDimension; x++)
			{
				if (m_Map[y][x] != 0)
				{
					if (InRowCount(x, y, 1, 0) >= winCondition ||
						InRowCount(x, y, -1, 1) >= winCondition ||
						InRowCount(x, y, 0, 1) >= winCondition ||
						InRowCount(x, y, 1, 1) >= winCondition)
					{
						return m_Map[y][x];
					}
				}
				else
				{
					EmptySpaces++;
				}
			}
		}
		if (EmptySpaces == 0)
		{
			return 0;
		}
		else
		{
			return {};
		}
	}


private:

	std::vector<std::vector<int>> m_Map;
	size_t m_MapDimension;

private:
	uint32_t InRowCount(int posX, int posY, int dx, int dy) const
	{
		uint32_t HitCount = 0;
		int Player = m_Map[posY][posX];
		while (m_Map[posY][posX] == Player)
		{
			HitCount++;
			if (posY + dy >= m_MapDimension || posX + dx >= m_MapDimension || posX + dx < 0 || posY + dy < 0)
			{
				break;
			}
			posY += dy;
			posX += dx;
		}
		return HitCount;
	}
};

