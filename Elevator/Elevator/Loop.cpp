#include <iostream>
#include <stdlib.h>
#include <ctime>

#include "Loop.h"

Loop::Loop() {

	// Window is set to null pointer
	window = nullptr;

	// Screen Width
	screenWidth = 640;

	// Screen Height
	screenHeight = 480;

	// State of Loop
	loopState = ON;
}
Loop::~Loop() {}

// Initiates the simulation
void Loop::run() {

	// Initializes system
	init();

	// Start the Loop
	processLoop();
}

// Initializes Loop
void Loop::init() {

	// Initializes SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	// Creates a window
	window = SDL_CreateWindow("Elevator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
}

void Loop::processInput(){
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			loopState = LoopState::OFF;
			break;
		}
	}
}

void Loop::processLoop(){

	// Times when to update
	std::clock_t begin = clock();
	std::clock_t end;

	while (loopState != LoopState::OFF) {

		// Processes Input
		processInput();

		end = clock();

		if (double(end - begin) / CLOCKS_PER_SEC >= 1) {

			// Clears screen
			system("cls");

			// Updates everything
			update();

			// Prints the current state of elevators
			printCurrentState();

			// Renews begin
			begin = clock();
		}
	}
}

// Updates the current state of elevators
void Loop::update() {

	// Updates the directions of elevators
	updateDirections();

	// Moves elevators according to their direction and remove destinaions when they reach them
	moveElevators();

	// Updates (randomly presses) buttons
	updateButtons();

	// Checks whether elevators have reached their destinations and turns buttons off
	updateDestinations();
}

// Prints the current state of elevators (Console)
void Loop::printCurrentState() {
	std::cout << "Elevator: \t";
	for (int i = 1; i <= N_ELEVATORS; i++) {
		std::cout << "#" << i << "\t";
	}
	std::cout << "\nFloor:\t\t";
	for (int i = 0; i < N_ELEVATORS; i++) {
		std::cout << elevator[i].getLevel() << "\t";
	}
	std::cout << "\n";
	for (int i = 0; i < N_ELEVATORS; i++) {
		std::cout << "\nElevator " << i + 1 << " destinations: ";
		for (auto d : *(elevator[i].getPDestinations())) {
			std::cout << d << " ";
		}
	}
}

// Returns the closest elevator
int Loop::getClosest(int f) {
	int closest = 0;
	int distance = getDistance(0, f);
	for (int i = 1; i < N_ELEVATORS; i++) {
		if (getDistance(i, f) < distance) {
			distance = getDistance(i, f);
			closest = i;
		}
	}
	return closest;
}

// Returns the closest floor to an elevator
int Loop::getClosestFloor(int e)
{
	int tClosest = 0;
	int distance = MAX_HEIGHT;
	for (auto d : *(elevator[e].getPDestinations())) {
		if (getDistance(e, d) < distance) {
			tClosest = d;
			distance = getDistance(e, d);
		}
	}
	return tClosest;
}

////////////////////
//NO LONGER IN USE//
////////////////////
int Loop::getFarther(int f, int c) {
	int n_fElevator = N_ELEVATORS, tDistance;
	int cDistance = getDistance(c, f);
	int distance = cDistance + MAX_HEIGHT;
	for (int i = 0; i < N_ELEVATORS; i++) {
		tDistance = getDistance(i, f);
		if (tDistance > cDistance && tDistance < distance) {
			distance = tDistance;
			n_fElevator = i;
		}
	}
	return n_fElevator;
}

// Returns the most available elevator for a floor
int Loop::getAvailable(int f, Direction d) {
	int e_quickest = N_ELEVATORS;
	int t_quickest = MAX_HEIGHT * 2;
	for (int i = 0; i < N_ELEVATORS; i++) {
		int tArrival = t_arrival(i, f);
		if (tArrival < t_quickest) {
			t_quickest = tArrival;
			e_quickest = i;
		}
	}
	return e_quickest;
	/*
	Direction cDirection = elevator[tClosest].getDirection();
	int farther = getFarther(f, tClosest);
	
	// Case 1: Closest is stationary -> Available
	if (cDirection == STATIONARY) {
		return tClosest;
	}

	// Case 2: Closest is in the SAME direction -> Check whether it is moving towards or away
	// IF TOWARDS -> Available
	// IF AWAY -> Get another elevator
	else if (cDirection == d) {
		if (cDirection == UP) {
			if (f > elevator[tClosest].getLevel()) {
				return tClosest;
			}
			else {
				if (farther == N_ELEVATORS) {
					return getClosest(f);
				}
				return getAvailable(f, d, farther);
			}
		}
		else {
			if (f < elevator[tClosest].getLevel()) {
				return tClosest;
			}
			else {
				if (farther == N_ELEVATORS) {
					return getClosest(f);
				}
				return getAvailable(f, d, farther);
			}
		}
	}

	// Case 3: Closest is in the OPPOSITE direction -> Get another elevator
	else {
		if (farther == N_ELEVATORS) {
			return getClosest(f);
		}
		return getAvailable(f, d, farther);
	}
	*/
}

// Returns the distance between a elevator and a floor
int Loop::getDistance(int e, int f) {
	return abs(f - elevator[e].getLevel());
}

// Calculates the time the elevator needs to arrive at the given floor
int Loop::t_arrival(int e, int f)
{
	if (elevator[e].getDirection() == STATIONARY) {
		return getDistance(e, f);
	}
	else if (elevator[e].getDirection() == UP) {
		if (f < elevator[e].getLevel()) {
			return getDistance(e, elevator[e].getPDestinations()->back()) + (elevator[e].getPDestinations()->back() - f);
		}
		return getDistance(e, f);
	}
	else {
		if (f > elevator[e].getLevel()) {
			return getDistance(e, elevator[e].getPDestinations()->front()) + (f - elevator[e].getPDestinations()->front());
		}
		return getDistance(e, f);
	}
}

// Checks the destination and move accordingly
void Loop::moveElevators() {
	for (int i = 0; i < N_ELEVATORS; i++) {

		if (!(elevator[i].getPDestinations()->empty()) && elevator[i].getDirection() == STATIONARY) {
			if (elevator[i].getLevel() == elevator[i].getPDestinations()->front()) {
				elevator[i].getPDestinations()->pop_front();
			}
		}

		// Going up -> Move up by S_ELEVATORS -> Check if it reached front
		// Reached front -> Remove front
		if (elevator[i].getDirection() == UP) {
			elevator[i].setLevel(elevator[i].getLevel() + S_ELEVATORS);
			if (elevator[i].getLevel() == elevator[i].getPDestinations()->front()) {
				elevator[i].getPDestinations()->pop_front();
			}
		}

		// Going down -> Move down by S_ELEVATORS -> Check if it reached back
		// Reached back -> Remove back
		else if (elevator[i].getDirection() == DOWN) {
			elevator[i].setLevel(elevator[i].getLevel() - S_ELEVATORS);
			if (elevator[i].getLevel() == elevator[i].getPDestinations()->back()) {
				elevator[i].getPDestinations()->pop_back();
			}
		}
	}
}

// Updates directions
void Loop::updateDirections() {
	for (int i = 0; i < N_ELEVATORS; i++) {
		updateDirection(i);
	}
}

// Updates the directions of elevators
void Loop::updateDirection(int e) {
	if (elevator[e].getPDestinations()->empty()) {
		elevator[e].setDirection(STATIONARY);
	}
	else if (elevator[e].getDirection() == UP) {
		if (elevator[e].getLevel() > elevator[e].getPDestinations()->back()) {
			elevator[e].setDirection(DOWN);
		}
	}
	else if (elevator[e].getDirection() == DOWN) {
		if (elevator[e].getLevel() < elevator[e].getPDestinations()->front()) {
			elevator[e].setDirection(UP);
		}
	}
	else if (elevator[e].getDirection() == STATIONARY) {
		if (getClosestFloor(e) < elevator[e].getLevel()) {
			elevator[e].setDirection(DOWN);
		}
		else if (getClosestFloor(e) > elevator[e].getLevel()) {
			elevator[e].setDirection(UP);
		}
	}
}

// Updates (randomly presses) buttons
void Loop::updateButtons() {
	for (int i = 0; i < MAX_HEIGHT; i++) {
		floor[i].pressButtons();
	}
}

// Checks whether elevators have reached their destinations and turns buttons off
void Loop::updateDestinations() {
	for (int i = 1; i <= MAX_HEIGHT; i++) {
		if (floor[i - 1].getUpButton()) {
			elevator[getAvailable(i, UP)].addDestination(i);
			floor[i - 1].disableUpButton();
		}
		else if (floor[i - 1].getDownButton()) {
			elevator[getAvailable(i, DOWN)].addDestination(i);
			floor[i - 1].disableDownButton();
		}
	}
}