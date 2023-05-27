#pragma once
#include <iostream>
class Map
{
public:

	Map(size_t dimension)
		:m_MapDimension(dimension)
	{
		m_Map = new int* [m_MapDimension];
		for (size_t i = 0; i < m_MapDimension; i++)
		{
			m_Map[i] = new int[m_MapDimension];
		}
	}

	~Map()
	{
		for (size_t i = 0; i < m_MapDimension; i++)
		{
			delete[] m_Map[i];
		}
		delete[] m_Map;
	}

	size_t GetDimensions() const { return m_MapDimension; }
	int** Data() const { return m_Map; }

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

	void At(int i, int j, int value)
	{
		m_Map[i][j] = value;
	}


private:

	int** m_Map;
	size_t m_MapDimension;

};

