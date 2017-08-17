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

	int counter;
	Direction pausedDirection;

	// Gets/Sets level
	int getLevel();
	void setLevel(int l);

	// Gets destinations
	std::list<int>* getPDestinations();
	std::list<int>* getPAbove();
	std::list<int>* getPBelow();

	// Adds a destination
	void addDestination(int d);

	// Adds a destination to above/below
	void addAbove(int d);
	void addBelow(int d);

	// Gets/Sets direction
	Direction getDirection();
	void setDirection(Direction d);

private:

	// Level value
	int level;
	std::list<int> destinations;

	std::list<int> above;
	std::list<int> below;

	// Direction
	Direction dir;
};