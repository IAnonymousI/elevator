#include <iostream>
#include <string>
#include <stdlib.h>
#include <ctime>
#include <time.h>

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
	mRandom = DEACTIVATED;
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
	
	srand((unsigned int)time(NULL));

	// Sets every floor
	for (int i = 0; i < MAX_HEIGHT; i++) {
		floor[i].setFloor(i + 1);
	}

	for (int i = 0; i < N_ELEVATORS; i++) {
		elevator[i].setLevel((i + 1) * 10);
	}

	// Initializes SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		reportError("SDL could not be initialized...");
	}

	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
		reportError("SDL_image could not be initialized...");
	}

	// Creates a window
	window = SDL_CreateWindow("Elevator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
	if (window == nullptr) {
		reportError("SDL Window could not be created...");
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	background.init(renderer, 0, 0, 640, 480, "Textures/background.jpg");
	E1.init(renderer, 58, 5, 64, 25, "Textures/E1.png");
	E2.init(renderer, 211, 5, 64, 25, "Textures/E2.png");
	E3.init(renderer, 365, 5, 64, 25, "Textures/E3.png");
	E4.init(renderer, 518, 5, 64, 25, "Textures/E4.png");
	E1F.init(renderer, 26, 35, 128, 128, "Textures/Numbers/--.png");
	E2F.init(renderer, 179, 35, 128, 128, "Textures/Numbers/--.png");
	E3F.init(renderer, 333, 35, 128, 128, "Textures/Numbers/--.png");
	E4F.init(renderer, 486, 35, 128, 128, "Textures/Numbers/--.png");
	BStop.init(renderer, 26, 188, 128, 64, "Textures/button_stop.png");
	BRandom.init(renderer, 179, 188, 94, 64, "Textures/button_random_short.png");
	BRandomUp.init(renderer, 277, 188, 30, 30, "Textures/button_random_up.png");
	BRandomDown.init(renderer, 277, 222, 30, 30, "Textures/button_random_down.png");
	BManual.init(renderer, 333, 188, 128, 64, "Textures/button_manual.png");

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	std::cout << SDL_GetError();

	background.draw();
	E1.draw();
	E2.draw();
	E3.draw();
	E4.draw();
	E1F.draw();
	E2F.draw();
	E3F.draw();
	E4F.draw();
	BStop.draw();
	BRandom.draw();
	BRandomUp.draw();
	BRandomDown.draw();
	BManual.draw();
}

void Loop::processInput(){
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_MOUSEBUTTONDOWN:
			if (withinBoundaries(&BRandom, e.motion.x, e.motion.y)) {
				BRandom.changeImage("Textures/button_random_short_pressed.png");
				BRandom.draw();
			}
			else if (withinBoundaries(&BRandomUp, e.motion.x, e.motion.y)) {
				BRandomUp.changeImage("Textures/button_random_up_pressed.png");
				BRandomUp.draw();
			}
			else if (withinBoundaries(&BRandomDown, e.motion.x, e.motion.y)) {
				BRandomDown.changeImage("Textures/button_random_down_pressed.png");
				BRandomDown.draw();
			}
			else if(withinBoundaries(&BStop, e.motion.x, e.motion.y)){
				BStop.changeImage("Textures/button_stop_pressed.png");
				BStop.draw();
			}
			else if (withinBoundaries(&BManual, e.motion.x, e.motion.y)) {
				BManual.changeImage("Textures/button_manual_pressed.png");
				BManual.draw();
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (withinBoundaries(&BRandom, e.motion.x, e.motion.y)) {
				mRandom = ACTIVATED;
				BRandom.changeImage("Textures/button_random_short.png");
				BRandom.draw();
			}
			else if (withinBoundaries(&BRandomUp, e.motion.x, e.motion.y)) {
				for (int i = 0; i < MAX_HEIGHT; i++) {
					floor[i].incRate();
				}
				BRandomUp.changeImage("Textures/button_random_up.png");
				BRandomUp.draw();
			}
			else if (withinBoundaries(&BRandomDown, e.motion.x, e.motion.y)) {
				for (int i = 0; i < MAX_HEIGHT; i++) {
					floor[i].decRate();
				}
				BRandomDown.changeImage("Textures/button_random_down.png");
				BRandomDown.draw();
			}
			else if (withinBoundaries(&BStop, e.motion.x, e.motion.y)) {
				mRandom = DEACTIVATED;
				BStop.changeImage("Textures/button_stop.png");
				BStop.draw();
			}
			else if (withinBoundaries(&BManual, e.motion.x, e.motion.y)) {
				BManual.changeImage("Textures/button_manual.png");
				BManual.draw();
			}
			break;
		case SDL_QUIT:
			loopState = LoopState::OFF;
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			IMG_Quit();
			SDL_Quit();
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

		updateWindow();

		end = clock();

		// Times when to update (1 sec)
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

	if (mRandom == ACTIVATED) {
		randomPress();
	}

	// Checks whether elevators have reached their destinations and turns buttons off
	updateDestinations();
}

// Prints the current state of elevators (Console)
void Loop::printCurrentState() {
	std::cout << "Random Mode: ";
	if (mRandom == ACTIVATED) {
		std::cout << "ACTIVATED";
	}
	else if(mRandom == DEACTIVATED){
		std::cout << "DEACTIVATED";
	}
	std::cout << "\nRandomness Value: " << floor[0].getRValue();
	for (int i = 0; i < N_ELEVATORS; i++) {
		std::cout << "\nElevator " << i + 1 << " destinations: ";
		for (auto d : *(elevator[i].getPDestinations())) {
			std::cout << d << " ";
		}
	}
}

void Loop::updateWindow(){
	updateEF();
	SDL_RenderPresent(renderer);
}

bool Loop::withinBoundaries(Image* i, int mX, int mY)
{
	if (mX >= i->getX() && mX <= (i->getX() + i->getW()) && mY >= i->getY() && mY <= (i->getY() + i->getH())) {
		return true;
	}
	return false;
}

std::string Loop::getNumberImagePath(int n)
{
	switch (n) {
	case 1:
		return "Textures/Numbers/1.png";
	case 2:
		return "Textures/Numbers/2.png";
	case 3:
		return "Textures/Numbers/3.png";
	case 4:
		return "Textures/Numbers/4.png";
	case 5:
		return "Textures/Numbers/5.png";
	case 6:
		return "Textures/Numbers/6.png";
	case 7:
		return "Textures/Numbers/7.png";
	case 8:
		return "Textures/Numbers/8.png";
	case 9:
		return "Textures/Numbers/9.png";
	case 10:
		return "Textures/Numbers/10.png";
	case 11:
		return "Textures/Numbers/11.png";
	case 12:
		return "Textures/Numbers/12.png";
	case 13:
		return "Textures/Numbers/13.png";
	case 14:
		return "Textures/Numbers/14.png";
	case 15:
		return "Textures/Numbers/15.png";
	case 16:
		return "Textures/Numbers/16.png";
	case 17:
		return "Textures/Numbers/17.png";
	case 18:
		return "Textures/Numbers/18.png";
	case 19:
		return "Textures/Numbers/19.png";
	case 20:
		return "Textures/Numbers/20.png";
	case 21:
		return "Textures/Numbers/21.png";
	case 22:
		return "Textures/Numbers/22.png";
	case 23:
		return "Textures/Numbers/23.png";
	case 24:
		return "Textures/Numbers/24.png";
	case 25:
		return "Textures/Numbers/25.png";
	case 26:
		return "Textures/Numbers/26.png";
	case 27:
		return "Textures/Numbers/27.png";
	case 28:
		return "Textures/Numbers/28.png";
	case 29:
		return "Textures/Numbers/29.png";
	case 30:
		return "Textures/Numbers/30.png";
	case 31:
		return "Textures/Numbers/31.png";
	case 32:
		return "Textures/Numbers/32.png";
	case 33:
		return "Textures/Numbers/33.png";
	case 34:
		return "Textures/Numbers/34.png";
	case 35:
		return "Textures/Numbers/35.png";
	case 36:
		return "Textures/Numbers/36.png";
	case 37:
		return "Textures/Numbers/37.png";
	case 38:
		return "Textures/Numbers/38.png";
	case 39:
		return "Textures/Numbers/39.png";
	case 40:
		return "Textures/Numbers/40.png";
	case 41:
		return "Textures/Numbers/41.png";
	case 42:
		return "Textures/Numbers/42.png";
	case 43:
		return "Textures/Numbers/43.png";
	case 44:
		return "Textures/Numbers/44.png";
	case 45:
		return "Textures/Numbers/45.png";
	case 46:
		return "Textures/Numbers/46.png";
	case 47:
		return "Textures/Numbers/47.png";
	case 48:
		return "Textures/Numbers/48.png";
	case 49:
		return "Textures/Numbers/49.png";
	case 50:
		return "Textures/Numbers/50.png";
	default:
		return "Textures/Numbers/--.png";
	}
}

void Loop::updateEF(){
	E1F.changeImage(getNumberImagePath(elevator[0].getLevel()));
	E2F.changeImage(getNumberImagePath(elevator[1].getLevel()));
	E3F.changeImage(getNumberImagePath(elevator[2].getLevel()));
	E4F.changeImage(getNumberImagePath(elevator[3].getLevel()));
	E1F.draw();
	E2F.draw();
	E3F.draw();
	E4F.draw();
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
void Loop::randomPress() {
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

void Loop::reportError(std::string e){
	int pause;
	std::cout << e << std::endl << "Press any key to continue...";
	std::cin >> pause;
}