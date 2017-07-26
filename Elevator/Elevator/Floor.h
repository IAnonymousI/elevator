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

	// Randomly presses buttons
	void pressButtons();

	// Disables buttons
	void disableUpButton();
	void disableDownButton();

	// Returns whether upButton has been pushed or not
	bool getUpButton();

	// Returns whether downButton has been pushed or not
	bool getDownButton();

private:

	bool upButton;
	bool downButton;
};