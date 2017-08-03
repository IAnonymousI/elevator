#pragma once
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "Elevator.h"
#include "Floor.h"
#include "Image.h"

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

enum Mode {
	ACTIVATED, DEACTIVATED
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

	// Updates the SDL window
	void updateWindow();

	// Checks whether the cursor is within the boundaries of an image
	bool withinBoundaries(Image* i, int mX, int mY);

	// Returns an image of the number given
	std::string getNumberImagePath(int n);

	void updateEF();

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
	void randomPress();

	// Updates every elevator's destinations
	void updateDestinations();

	// Reports Error
	void reportError(std::string e);

	// Elevators
	Elevator elevator[N_ELEVATORS];
	
	// Floors
	Floor floor[MAX_HEIGHT];

	int screenWidth;
	int screenHeight;
	LoopState loopState;
	Mode mRandom;

	SDL_Window* window;
	SDL_Renderer* renderer;

	Image background;
	Image E1, E2, E3, E4;
	Image E1F, E2F, E3F, E4F;
	Image BStop, BRandom, BRandomUp, BRandomDown, BManual;
};