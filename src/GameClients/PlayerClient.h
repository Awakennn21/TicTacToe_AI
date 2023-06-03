#include "GameClient.h"
#include "Map.h"
#include "Timer.h"

class PlayerClient : public GameClient
{
public:
	PlayerClient() {}
	virtual ~PlayerClient() {}
	virtual std::tuple<uint32_t, uint32_t, double> NextMove(Map map, int PlayerToMove) override
	{
		Timer Chrono;
		double TimeElapsed = 0.0;
		uint32_t Y, X;
		std::cout << "Input Move:";
		Chrono.Start();
		std::cin >> Y >> X;
		TimeElapsed = Chrono.Stop();
		std::cout << std::endl;
		return { Y,X,TimeElapsed };
	}
};