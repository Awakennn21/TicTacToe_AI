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
		double TimeSpendOnThisMove;
	};
public:
	DataAnalizer()
	{
		m_MovesToAnalize = {};
	}

	void LoadData(PreciseMoveData data) { m_MovesToAnalize.push_back(data); }
	void ClearData() { m_MovesToAnalize.clear(); }

	void FilterData(std::function<bool(const PreciseMoveData&)> condition = [](PreciseMoveData) { return true; })
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

	template<typename T>
	double Mean(std::function<T(const PreciseMoveData&)> fieldSelector, std::function<bool(const PreciseMoveData&)> conditionToMeet = [](const PreciseMoveData&) { return true; })
	{
		std::vector<PreciseMoveData> Tmp = m_MovesToAnalize;
		FilterData(conditionToMeet);
		double Mean = 0.0;
		for (const auto& i : m_MovesToAnalize)
		{
			Mean += static_cast<double>(fieldSelector(i));
		}
		Mean /= m_MovesToAnalize.size();
		m_MovesToAnalize = Tmp;
		return Mean;
	}

	template<typename T>
	PreciseMoveData& Max(std::function<T(const PreciseMoveData&)> fieldSelector, std::function<bool(const PreciseMoveData&)> conditionToMeet = [](const PreciseMoveData&) { return true; })
	{
		std::vector<PreciseMoveData> Tmp = m_MovesToAnalize;
		FilterData(conditionToMeet);
		PreciseMoveData Max = m_MovesToAnalize.front();
		for (const auto& i : m_MovesToAnalize)
		{
			if (fieldSelector(i) > fieldSelector(Max))
			{
				Max = i;
			}
		}
		m_MovesToAnalize = Tmp;
		return Max;
	}

	template<typename T>
	PreciseMoveData& Min(std::function<T(const PreciseMoveData&)> fieldSelector, std::function<bool(const PreciseMoveData&)> conditionToMeet = [](const PreciseMoveData&) { return true; })
	{
		std::vector<PreciseMoveData> Tmp = m_MovesToAnalize;
		FilterData(conditionToMeet);
		PreciseMoveData Min = m_MovesToAnalize.front();
		for (const auto& i : m_MovesToAnalize)
		{
			if (fieldSelector(i) < fieldSelector(Min))
			{
				Min = i;
			}
		}
		m_MovesToAnalize = Tmp;
		return Min;
	}

private:
	std::vector<PreciseMoveData> m_MovesToAnalize;

};