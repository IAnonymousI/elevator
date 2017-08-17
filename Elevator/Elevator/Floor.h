#pragma once

// Residents per floor
const int N_RESIDENTS = 9;

class Floor
{
public:
	Floor();
	~Floor();

	// Set floor
	void setFloor(int f);

	// Increases/Decreases r_buttons by 100
	void incRate();
	void decRate();

	// Residents request to go up/down
	void pressFloorButtons();

	// After residents get onto the elevator, they request a certain floor that they want to go to
	int pressElevatorButtons();

	// Disables buttons
	void disableUpButton();
	void disableDownButton();

	// Returns r_buttons
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

	// Assigned Floor
	int floor;
};