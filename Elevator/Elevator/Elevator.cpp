#include "Elevator.h"

Elevator::Elevator()
{
	level = 1;
	dir = STATIONARY;
}

Elevator::~Elevator(){}

// Sets floor value
void Elevator::setLevel(int l) {
	level = l;
}


// Returns floor value
int Elevator::getLevel() {
	return level;
}

std::list<int>* Elevator::getPDestinations()
{
	return &destinations;
}

Direction Elevator::getDirection() {
	return dir;
}

void Elevator::setDirection(Direction d) {
	dir = d;
}

void Elevator::addDestination(int d) {

	// Adds the destination to the front
	destinations.push_front(d);

	// Sorts the destinations
	destinations.sort();

	// Deletes repeats
	destinations.unique();
}