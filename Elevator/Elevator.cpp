#include "Elevator.h"

Elevator::Elevator()
{
	counter = 0;
	pausedDirection = STATIONARY;
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

std::list<int>* Elevator::getPAbove(){
	return &above;
}

std::list<int>* Elevator::getPBelow()
{
	return &below;
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

void Elevator::addAbove(int d){
	// Adds the destination to the front
	above.push_front(d);

	// Sorts the destinations
	above.sort();

	// Deletes repeats
	above.unique();
}

void Elevator::addBelow(int d) {
	// Adds the destination to the front
	below.push_front(d);

	// Sorts the destinations
	below.sort();

	// Deletes repeats
	below.unique();
}