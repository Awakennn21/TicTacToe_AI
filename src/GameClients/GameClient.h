#pragma once 
#include <iostream>

class GameClient
{
public:
	GameClient() {}
	virtual ~GameClient() {}
	virtual std::tuple<uint32_t, uint32_t, double> NextMove(class Map map, int PlayerToMove) = 0;
};