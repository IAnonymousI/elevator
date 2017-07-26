#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>

#include "Elevator.h"
#include "Floor.h"

// MAXIMUM HEIGHT OF BUILDING
const int MAX_HEIGHT = 48;

// MINIMUM HEIGHT OF BUILDING
const int MIN_HEIGHT = 1;

// NUMBER OF ELEVATORS
const int N_ELEVATORS = 4;

// SPEED OF ELEVATORS
const int S_ELEVATORS = 1;

enum LoopState {
	ON, OFF
};

// Starts the simulation
class Loop {

public:

	Loop();
	~Loop();

	// Starts the simulation
	void run();

private:

	// Initializes elevators
	void init();

	// Processes input
	void processInput();

	// Processes loop
	void processLoop();

	// Updates the current state of elevators
	void update();

	// Prints the current state of elevators (Console)
	void printCurrentState();

	// Returns the closest elevator
	int getClosest(int f);

	// Returns the closest floor
	int getClosestFloor(int e);

	// Returns the elevator that is farther than the closest, but closer than the others
	int getFarther(int f, int c);

	// Returns an available elevator
	int getAvailable(int f, Direction d);

	// Returns the distance between the current floor of an elevator and the desired floor
	int getDistance(int e, int f);

	// Returns the approximate time the elevator needs to arrive f
	int t_arrival(int e, int f);

	// Moves elevators according to their directions, and update their direction
	void moveElevators();

	// Updates directions
	void updateDirections();
	void updateDirection(int e);

	// Updates every floor's buttons
	void updateButtons();

	// Updates every elevator's destinations
	void updateDestinations();

	// Elevators
	Elevator elevator[N_ELEVATORS];
	
	// Floors
	Floor floor[MAX_HEIGHT];

	SDL_Window* window;
	int screenWidth;
	int screenHeight;
	LoopState loopState;
};