#include <cassert>
#include <unordered_map>

#include "GameControler.h"
#include "GameClients/MCTS_Algorithm/MCTS_Controller.h"
#include "GameClients/MinMax/ProjektCpp/MinMax_Controler.h"
#include "GameClients/PlayerClient.h"
#include "GameClients/RandomClient.h"

GameControler::GameControler(uint32_t mapSize, uint32_t winCondition)
	:m_MainMap(Map(mapSize)), m_WinCondition(winCondition) 
{
	m_MainMap.Clear();
}

void GameControler::AddClient(eClient gameClient)
{
	assert(m_MainMap.GetDimensions() > 0 && "Map not initialized");

	switch (gameClient)
	{
		case GameControler::MCTS:
			m_GameClients.push_back(new MCTS_Controller(m_MainMap.GetDimensions(), m_WinCondition, 10000));
			break;

		case GameControler::MinMax:
			m_GameClients.push_back(new MinMax_Controler(m_MainMap.GetDimensions(), m_WinCondition, (char)(m_GameClients.size() + 49)));
			break;

		case GameControler::Player:
			m_GameClients.push_back(new PlayerClient());
			break;

		case GameControler::Random:
			m_GameClients.push_back(new RandomClient());
			break;

		case GameControler::None:
			break;

		default:
			break;
	}
}

void GameControler::Simulate(uint32_t numberOfSimulations, bool printPreciseMoves)
{
	std::unordered_map<int, int> PlayersWins;

	for (int i = 0; i < numberOfSimulations; i++)
	{
	    std::optional<int> GameResult;
	    uint32_t PlayerToMove = 1;
	    float TurnsNumber = 0.0;
	    uint32_t Y, X;
	    double TimeSpentOnMove = 0.0;

	
		m_MainMap.Clear();
	    do
	    {
			std::tie(Y, X, TimeSpentOnMove) = m_GameClients[PlayerToMove - 1]->NextMove(m_MainMap,PlayerToMove);
	        m_MainMap.At(Y, X, PlayerToMove);
			m_Statistics.LoadData({ {Y,X},{m_MainMap.GetDimensions(),m_MainMap.GetDimensions() },m_WinCondition,PlayerToMove,(uint32_t)TurnsNumber, TimeSpentOnMove });
			TurnsNumber += 0.5f;
			if (printPreciseMoves)
			{
				std::cout << "Player:" << PlayerToMove << " Move:" << Y << "," << X << std::endl;
				m_MainMap.Print();
			}
			PlayerToMove = PlayerToMove == m_GameClients.size() ? 1 : PlayerToMove + 1;

		} while ((GameResult = m_MainMap.EvaluateGame(m_WinCondition)).has_value() == false);
	
		PlayersWins[GameResult.value()]++;
		for (auto i : PlayersWins)
		{
			if (i.first != 0)
			{
				std::cout <<"'" << i.first << "' won " << i.second << " times ";
			}
			else
			{
				std::cout << i.second << " draws occured ";
			}
		}
		std::cout << std::endl;
	}
}
