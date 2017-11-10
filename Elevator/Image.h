#pragma once
#include <SDL/SDL_image.h>
#include <string>

class Image
{

public:

	// Constructors/Destructors
	Image();
	~Image();

	// Initializes the image
	void init(SDL_Renderer* r, int x, int y, int w, int h, const std::string path);

	// Changes the image file
	void changeImage(const std::string path);

	// Returns X, Y, W, H, respectively
	int getX();
	int getY();
	int getW();
	int getH();

	// Draws the image to renderer
	void draw();

private:

	// X Position, Y Position, Width, Height of the image
	int _x, _y;
	int _w, _h;

	// Image file path (Relative to the Object file)
	std::string image_path;

	// Renderer
	SDL_Renderer* _renderer;

	// Texture
	SDL_Texture* _texture;

};