#include "Floor.h"

#include <cstdlib>

Floor::Floor() {
	r_buttons = 3000;
	upButton = false;
	downButton = false;
};
Floor::~Floor() {};

void Floor::setFloor(int f){
	floor = f;
}

void Floor::incRate(){
	r_buttons = r_buttons + 100;
}

void Floor::decRate(){
	r_buttons = r_buttons - 100;
}

int Floor::pressButtons() {
	int destination;
	for (int i = 0; i < N_RESIDENTS; i++) {
		int decider = rand() % r_buttons;

		// MAX_HEIGHT = 48 ... Fix later
		destination = (rand() % 48) + 1;
		if (decider == 0) {
			if (destination > floor) {
				upButton = true;
			}
			else if (destination < floor) {
				downButton = true;
			}
		}
	}
	return destination;
}

void Floor::disableUpButton(){
	upButton = false;
}

void Floor::disableDownButton(){
	downButton = false;
}

int Floor::getRValue()
{
	return r_buttons;
}

bool Floor::getUpButton()
{
	return upButton;
}

bool Floor::getDownButton()
{
	return downButton;
}
