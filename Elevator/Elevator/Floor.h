#pragma once

// Residents per floor
const int N_RESIDENTS = 9;

class Floor
{
public:
	Floor();
	~Floor();

	void setFloor(int f);

	void incRate();
	void decRate();

	// Residents request to go up/down
	// Returns their desired destination
	int pressButtons();

	// Disables buttons
	void disableUpButton();
	void disableDownButton();

	int getRValue();

	// Returns whether upButton has been pushed or not
	bool getUpButton();

	// Returns whether downButton has been pushed or not
	bool getDownButton();

private:

	// Rate of button pushes
	int r_buttons;

	// State of the buttons
	bool upButton;
	bool downButton;

	int floor;
};