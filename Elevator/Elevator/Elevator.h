#pragma once

#include <list>

enum Direction {
	UP, STATIONARY, DOWN
};

// Elevator object
class Elevator
{

public:

	// Constructor/Deconstructor
	Elevator();
	~Elevator();

	// Gets/Sets level
	int getLevel();
	void setLevel(int l);

	// Gets destinations
	std::list<int>* getPDestinations();

	// Adds a destination
	void addDestination(int d);

	// Gets/Sets direction
	Direction getDirection();
	void setDirection(Direction d);

private:

	// Level value
	int level;
	std::list<int> destinations;

	Direction dir;
};