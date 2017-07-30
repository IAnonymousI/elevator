#pragma once

// Residents per floor
const int N_RESIDENTS = 9;

// Rate of button pushes
const int R_BUTTONS = 5000;

class Floor
{
public:
	Floor();
	~Floor();

	void setFloor(int f);

	// Residents request to go up/down
	// Returns their desired destination
	int pressButtons();

	// Disables buttons
	void disableUpButton();
	void disableDownButton();

	// Returns whether upButton has been pushed or not
	bool getUpButton();

	// Returns whether downButton has been pushed or not
	bool getDownButton();

private:

	// State of the buttons
	bool upButton;
	bool downButton;

	int floor;
};