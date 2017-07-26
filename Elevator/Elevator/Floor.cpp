#include "Floor.h"

#include <cstdlib>
#include <time.h>

Floor::Floor() {
	upButton = false;
	downButton = false;
};
Floor::~Floor() {};

void Floor::pressButtons() {
	for (int i = 0; i < N_RESIDENTS; i++) {
		int decider = rand() % R_BUTTONS;
		if (decider == 0) {
			upButton = true;
		}
		else if (decider == 1) {
			downButton = true;
		}
	}
}

void Floor::disableUpButton(){
	upButton = false;
}

void Floor::disableDownButton(){
	downButton = false;
}

bool Floor::getUpButton()
{
	return upButton;
}

bool Floor::getDownButton()
{
	return downButton;
}
