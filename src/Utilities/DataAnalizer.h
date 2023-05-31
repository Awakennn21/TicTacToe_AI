#pragma once
#include<functional>
#include <vector>

class DataAnalizer
{
public:
	struct PreciseMoveData
	{
		std::tuple<uint32_t, uint32_t> MoveCordinates;
		std::tuple<uint32_t, uint32_t> MapSize;
		uint32_t WinCondition;
		uint32_t Player;
		uint32_t MovesID;
		int Algorithm;
		double TimeSpendOnThisMove;
	};
public:
	DataAnalizer()
	{
		m_MovesToAnalize = {};
	}

	void LoadData(PreciseMoveData data) { m_MovesToAnalize.push_back(data); }
	void ClearData() { m_MovesToAnalize.clear(); }

	void FilterData(std::function<bool(PreciseMoveData)> condition)
	{
		std::vector<PreciseMoveData> New;
		for (int i = 0; i < m_MovesToAnalize.size(); i++)
		{
			if (condition(m_MovesToAnalize[i]))
			{
				New.push_back(m_MovesToAnalize[i]);
			}
		}
		m_MovesToAnalize.clear();
		m_MovesToAnalize = New;
	}

	double CalculateTimeMean(std::function<bool(PreciseMoveData)> conditionToMeet = [](PreciseMoveData) { return true; })
	{
		std::vector<PreciseMoveData> Tmp = m_MovesToAnalize;
		FilterData(conditionToMeet);
		double Mean = 0.0;
		for (auto i : m_MovesToAnalize)
		{
			Mean += i.TimeSpendOnThisMove;
		}
		Mean /= m_MovesToAnalize.size();
		m_MovesToAnalize = Tmp;
		return Mean;
	}

private:
	std::vector<PreciseMoveData> m_MovesToAnalize;

};