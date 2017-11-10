#include <iostream>
#include <string>
#include <stdlib.h>
#include <ctime>
#include <time.h>

#include "Loop.h"

// Constructor
Loop::Loop() {

	// Window is set to null pointer
	window = nullptr;

	// Screen Width
	screenWidth = 512;

	// Screen Height
	screenHeight = 512;

	// State of Loop
	loopState = ON;

	// Random Mode Flag
	mRandom = DEACTIVATED;
}

// Destructor
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
	
	// Pseudo-random
	srand((unsigned int)time(NULL));

	// Sets every floor
	for (int i = 0; i < MAX_HEIGHT; i++) {
		floor[i].setFloor(i + 1);
	}

	// Sets every Elevator at level 1
	for (int i = 0; i < N_ELEVATORS; i++) {
		elevator[i].setLevel(1);
	}

	// Initializes SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		reportError("SDL could not be initialized...");
	}

	// Initializes IMG
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
		reportError("SDL_image could not be initialized...");
	}

	// Creates window
	window = SDL_CreateWindow("Elevator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
	if (window == nullptr) {
		reportError("SDL Window could not be created...");
	}

	// Creates renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	background.init(renderer, 0, 0, 512, 512, "Textures/background.png");
	background.draw();

	for (int i = 0; i < MAX_HEIGHT; i++) {
		int tY = 486 - 10 * i;
		E1[i].init(renderer, 25, tY, 10, 10, "Textures/blank.png");
		E2[i].init(renderer, 45, tY, 10, 10, "Textures/blank.png");
		E3[i].init(renderer, 65, tY, 10, 10, "Textures/blank.png");
		E4[i].init(renderer, 85, tY, 10, 10, "Textures/blank.png");
		BFloorUp[i].init(renderer, 120, tY, 10, 10, "Textures/button_floor_up_off.png");
		BFloorDown[i].init(renderer, 134, tY, 10, 10, "Textures/button_floor_down_off.png");
		E1[i].draw();
		E2[i].draw();
		E3[i].draw();
		E4[i].draw();
		BFloorUp[i].draw();
		BFloorDown[i].draw();
	}

	BStop.init(renderer, 175, 20, 128, 64, "Textures/button_stop.png");
	BRandom.init(renderer, 313, 20, 94, 64, "Textures/button_random_short.png");
	BStop.draw();
	BRandom.draw();

	/* Redesigning...
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
	*/
	/*
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
	*/
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
			/*
			else if (withinBoundaries(&BRandomUp, e.motion.x, e.motion.y)) {
				BRandomUp.changeImage("Textures/button_random_up_pressed.png");
				BRandomUp.draw();
			}
			else if (withinBoundaries(&BRandomDown, e.motion.x, e.motion.y)) {
				BRandomDown.changeImage("Textures/button_random_down_pressed.png");
				BRandomDown.draw();
			}
			*/
			else if(withinBoundaries(&BStop, e.motion.x, e.motion.y)){
				BStop.changeImage("Textures/button_stop_pressed.png");
				BStop.draw();
			}
			/*
			else if (withinBoundaries(&BManual, e.motion.x, e.motion.y)) {
				BManual.changeImage("Textures/button_manual_pressed.png");
				BManual.draw();
			}
			*/
			break;
		case SDL_MOUSEBUTTONUP:
			if (withinBoundaries(&BRandom, e.motion.x, e.motion.y)) {
				mRandom = ACTIVATED;
				BRandom.changeImage("Textures/button_random_short.png");
				BRandom.draw();
			}
			/*
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
			*/
			else if (withinBoundaries(&BStop, e.motion.x, e.motion.y)) {
				mRandom = DEACTIVATED;
				BStop.changeImage("Textures/button_stop.png");
				BStop.draw();
			}
			/*
			else if (withinBoundaries(&BManual, e.motion.x, e.motion.y)) {
				BManual.changeImage("Textures/button_manual.png");
				BManual.draw();
			}
			*/
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
	//updateDirections();
	for (int i = 0; i < N_ELEVATORS; i++) {
		if (elevator[i].getDirection() == STATIONARY) {
			updateDirection(i);
		}
	}

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
		for (auto d : *(elevator[i].getPBelow())) {
			std::cout << d << " ";
		}
		std::cout << "[] ";
		for (auto d : *(elevator[i].getPAbove())) {
			std::cout << d << " ";
		}
	}
}

void Loop::updateWindow(){
	updateEI();
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

std::string Loop::getElevatorImageFile(Direction d){
	switch (d) {
	case STATIONARY:
		return "Textures/elevator_stationary.png";
	case UP:
		return "Textures/elevator_up.png";
	case DOWN:
		return "Textures/elevator_down.png";
	}
}

// Updates Elevator images
void Loop::updateEI(){
	
	E1[elevator[0].getLevel() - 1].changeImage(getElevatorImageFile(elevator[0].getDirection()));
	E2[elevator[1].getLevel() - 1].changeImage(getElevatorImageFile(elevator[1].getDirection()));
	E3[elevator[2].getLevel() - 1].changeImage(getElevatorImageFile(elevator[2].getDirection()));
	E4[elevator[3].getLevel() - 1].changeImage(getElevatorImageFile(elevator[3].getDirection()));
	E1[elevator[0].getLevel() - 1].draw();
	E2[elevator[1].getLevel() - 1].draw();
	E3[elevator[2].getLevel() - 1].draw();
	E4[elevator[3].getLevel() - 1].draw();
}

void Loop::openDoor(int e){
	elevator[e].pausedDirection = elevator[e].getDirection();
	switch (e) {
	case 0:
		E1[elevator[0].getLevel() - 1].changeImage("Textures/elevator_open.png");
		E1[elevator[0].getLevel() - 1].draw();
		break;
	case 1:
		E2[elevator[1].getLevel() - 1].changeImage("Textures/elevator_open.png");
		E2[elevator[1].getLevel() - 1].draw();
		break;
	case 2:
		E3[elevator[2].getLevel() - 1].changeImage("Textures/elevator_open.png");
		E3[elevator[2].getLevel() - 1].draw();
		break;
	case 3:
		E4[elevator[3].getLevel() - 1].changeImage("Textures/elevator_open.png");
		E4[elevator[3].getLevel() - 1].draw();
		break;
	}
	elevator[e].setDirection(STATIONARY); 
	elevator[e].counter++;
}

void Loop::closeDoor(int e) {
	switch (e) {
	case 0:
		E1[elevator[0].getLevel() - 1].changeImage(getElevatorImageFile(elevator[e].pausedDirection));
		E1[elevator[0].getLevel() - 1].draw();
		break;
	case 1:
		E2[elevator[1].getLevel() - 1].changeImage(getElevatorImageFile(elevator[e].pausedDirection));
		E2[elevator[1].getLevel() - 1].draw();
		break;
	case 2:
		E3[elevator[2].getLevel() - 1].changeImage(getElevatorImageFile(elevator[e].pausedDirection));
		E3[elevator[2].getLevel() - 1].draw();
		break;
	case 3:
		E4[elevator[3].getLevel() - 1].changeImage(getElevatorImageFile(elevator[e].pausedDirection));
		E4[elevator[3].getLevel() - 1].draw();
		break;
	}
	elevator[e].setDirection(elevator[e].pausedDirection);
	elevator[e].counter = 0;
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
			return getDistance(e, elevator[e].getPAbove()->back()) + (elevator[e].getPAbove()->back() - f);
		}
		return getDistance(e, f);
	}
	else {
		if (f > elevator[e].getLevel()) {
			return getDistance(e, elevator[e].getPBelow()->front()) + (f - elevator[e].getPBelow()->front());
		}
		return getDistance(e, f);
	}
}

// Checks the destination and move accordingly
void Loop::moveElevators() {
	for (int i = 0; i < N_ELEVATORS; i++) {

		// IF counter is 0 -> Not stopped, in motion
		// IF counter is 1 ~ 3 -> Stopped, door stays opened
		// IF counter is 4 -> Reached time limit, closes door
		if (elevator[i].counter > 0) {
			if (elevator[i].counter < 4) {
				elevator[i].counter++;
			}
			else {
				closeDoor(i);
			}
		}

		// Going up -> Remove the elevator image -> Move up by S_ELEVATORS -> Check if it reached front
		// Reached front -> Remove front
		if (elevator[i].getDirection() == UP) {

			// Removes the elevator image
			switch (i) {
			case 0:
				E1[elevator[0].getLevel() - 1].changeImage("Textures/blank.png");
				E1[elevator[0].getLevel() - 1].draw();
				break;
			case 1:
				E2[elevator[1].getLevel() - 1].changeImage("Textures/blank.png");
				E2[elevator[1].getLevel() - 1].draw();
				break;
			case 2:
				E3[elevator[2].getLevel() - 1].changeImage("Textures/blank.png");
				E3[elevator[2].getLevel() - 1].draw();
				break;
			case 3:
				E4[elevator[3].getLevel() - 1].changeImage("Textures/blank.png");
				E4[elevator[3].getLevel() - 1].draw();
				break;
			}

			elevator[i].setLevel(elevator[i].getLevel() + S_ELEVATORS);
			if (elevator[i].getLevel() == elevator[i].getPAbove()->front()) {
				elevator[i].getPAbove()->pop_front();
				elevator[i].pausedDirection = elevator[i].getDirection();
				openDoor(i);

				// Adds a random destination...
				/*
				int eb = floor[elevator[i].getLevel() - 1].pressElevatorButtons();
				if (eb > elevator[i].getLevel()) {
				elevator[i].addAbove(eb);
				}
				else if (eb < elevator[i].getLevel()) {
				elevator[i].addBelow(eb);
				}*/

				// Checks whether direction needs to be changed
				updateDirection(i);
			}
		}

		// Going down -> Remove the elevator image -> Move down by S_ELEVATORS -> Check if it reached back
		// Reached back -> Remove back
		else if (elevator[i].getDirection() == DOWN) {

			// Removes the elevator image
			switch (i) {
			case 0:
				E1[elevator[0].getLevel() - 1].changeImage("Textures/blank.png");
				E1[elevator[0].getLevel() - 1].draw();
				break;
			case 1:
				E2[elevator[1].getLevel() - 1].changeImage("Textures/blank.png");
				E2[elevator[1].getLevel() - 1].draw();
				break;
			case 2:
				E3[elevator[2].getLevel() - 1].changeImage("Textures/blank.png");
				E3[elevator[2].getLevel() - 1].draw();
				break;
			case 3:
				E4[elevator[3].getLevel() - 1].changeImage("Textures/blank.png");
				E4[elevator[3].getLevel() - 1].draw();
				break;
			}

			elevator[i].setLevel(elevator[i].getLevel() - S_ELEVATORS);
			if (elevator[i].getLevel() == elevator[i].getPBelow()->back()) {
				elevator[i].getPBelow()->pop_back();
				elevator[i].pausedDirection = elevator[i].getDirection();
				openDoor(i);

				// Adds a random destination...
				/*
				int eb = floor[elevator[i].getLevel() - 1].pressElevatorButtons();
				if (eb > elevator[i].getLevel()) {
				elevator[i].addAbove(eb);
				}
				else if (eb < elevator[i].getLevel()) {
				elevator[i].addBelow(eb);
				}*/

				updateDirection(i);
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
	/*
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
	*/
	if (elevator[e].getPAbove()->empty()) {
		if (elevator[e].getPBelow()->empty()) {
			elevator[e].setDirection(STATIONARY);
		}
		else {
			elevator[e].setDirection(DOWN);
		}
	}
	else {
		if (elevator[e].getPBelow()->empty()) {
			elevator[e].setDirection(UP);
		}
	}
}

// Updates (randomly presses) buttons
void Loop::randomPress() {
	for (int i = 0; i < MAX_HEIGHT; i++) {
		floor[i].pressFloorButtons();
	}
}

// Checks whether elevators have reached their destinations and turns buttons off
// Calculates the most available elevator and assigns it to the floor
void Loop::updateDestinations() {
	for (int i = 1; i <= MAX_HEIGHT; i++) {
		if (floor[i - 1].getUpButton()) {
			int a = getAvailable(i, UP);
			if ((i - 1) > elevator[a].getLevel()) {
				elevator[a].addAbove(i);
				floor[i - 1].disableUpButton();
			}
			else if ((i - 1) < elevator[a].getLevel()) {
				elevator[a].addBelow(i);
				floor[i - 1].disableUpButton();
			}
			else {
				floor[i - 1].disableUpButton();
			}
		}
		else if (floor[i - 1].getDownButton()) {
			int a = getAvailable(i, DOWN);
			if ((i - 1) > elevator[a].getLevel()) {
				elevator[a].addAbove(i);
				floor[i - 1].disableDownButton();
			}
			else if ((i - 1) < elevator[a].getLevel()) {
				elevator[a].addBelow(i);
				floor[i - 1].disableDownButton();
			}
			else {
				floor[i - 1].disableDownButton();
			}
		}
	}
}

// Reports error
void Loop::reportError(std::string e){
	int pause;
	std::cout << e << std::endl << "Press any key to continue...";
	std::cin >> pause;
}