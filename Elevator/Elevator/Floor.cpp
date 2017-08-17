#include "Floor.h"
#include <cstdlib>

// Initializes values
Floor::Floor() {
	r_buttons = 3000;
	upButton = false;
	downButton = false;
};
Floor::~Floor() {};

// Sets floor to f
void Floor::setFloor(int f){
	floor = f;
}

// Increases r_buttons by 100
void Floor::incRate(){
	r_buttons = r_buttons + 100;
}

// Decreases r_buttons by 100
void Floor::decRate(){
	r_buttons = r_buttons - 100;
}

// Randomly presses floor buttons
void Floor::pressFloorButtons() {
	for (int i = 0; i < N_RESIDENTS; i++) {
		int decider = rand() % r_buttons;
		if (decider == 0) {
			upButton = true;
			return;
		}
		else if (decider == 1) {
			downButton = true;
			return;
		}
	}
}

// Randomly presses elevator buttons
int Floor::pressElevatorButtons() {
	return (rand() % 48) + 1;
}

// Disables up button
void Floor::disableUpButton(){
	upButton = false;
}

// Disables down button
void Floor::disableDownButton(){
	downButton = false;
}

// Returns r_buttons
int Floor::getRValue(){
	return r_buttons;
}

// Returns up button
bool Floor::getUpButton(){
	return upButton;
}

// Returns down button
bool Floor::getDownButton(){
	return downButton;
}