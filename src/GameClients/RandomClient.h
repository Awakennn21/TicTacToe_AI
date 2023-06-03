#include "GameClient.h"
#include "Map.h"
#include "Timer.h"

class RandomClient : public GameClient
{
public:
	RandomClient() {}
	virtual ~RandomClient() {}
	virtual std::tuple<uint32_t, uint32_t, double> NextMove(Map map, int PlayerToMove) override
	{
		Timer Chrono;
		double TimeElapsed = 0.0;
		Chrono.Start();
		srand(time(NULL));
		uint32_t Y =0;
		uint32_t X =0;
		do
		{
			 Y = rand() % map.GetDimensions();
			 X = rand() % map.GetDimensions();
		} while (map.GetAt(Y, X) != 0);
		TimeElapsed = Chrono.Stop();
		return { Y,X,TimeElapsed };
	}
};