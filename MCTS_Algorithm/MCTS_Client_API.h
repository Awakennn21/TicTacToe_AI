#pragma once
#include "MCTS_Controller.h"

class MCTS_Client_API
{
private:
	MCTS_Controller* m_Controller;
public:
	MCTS_Client_API(uint32_t mapSize, uint32_t winCondition, uint32_t iterations)
	{
		m_Controller = new MCTS_Controller(mapSize, winCondition, iterations);
	}

	~MCTS_Client_API()
	{
		delete m_Controller;
	}


    void FeedInMove(int move, int player)
    {
        m_Controller->FeedInMove(move, player);
    }

    std::tuple<int, int> NextMove(int PlayerToMove)
    {
        return m_Controller->NextMove(PlayerToMove);
    }

    void NewGame()
    {
        m_Controller->NewGame();
    }

    std::optional<double> EvaluateGame()
    {
        return m_Controller->EvaluateGame();
    }
};