#include "Image.h"
#include <iostream>

// Constructor
Image::Image() {}

// Destructor, destroys texture
Image::~Image(){
	SDL_DestroyTexture(_texture);
}

// Initializes the image
void Image::init(SDL_Renderer * r, int x, int y, int w, int h, const std::string path)
{
	_renderer = r;
	_x = x;
	_y = y;
	_w = w;
	_h = h;
	SDL_Surface * surface = IMG_Load(path.c_str());
	if (!surface) {
		std::cerr << "Surface could not be created...\n";
	}
	_texture = SDL_CreateTextureFromSurface(_renderer, surface);
	if (!_texture) {
		std::cerr << "Texture could not be created...\n";
	}
	std::cout << SDL_GetError();
	SDL_FreeSurface(surface);
}

// Changes the image file
void Image::changeImage(const std::string path){
	SDL_Surface * surface = IMG_Load(path.c_str());
	if (!surface) {
		std::cerr << "Surface could not be created...\n";
	}
	SDL_DestroyTexture(_texture);
	_texture = SDL_CreateTextureFromSurface(_renderer, surface);
	if (!_texture) {
		std::cerr << "Texture could not be created...\n";
	}
	std::cout << SDL_GetError();
	SDL_FreeSurface(surface);
}

// Returns X
int Image::getX()
{
	return _x;
}

// Returns Y
int Image::getY()
{
	return _y;
}

// Returns W
int Image::getW()
{
	return _w;
}

// Returns H
int Image::getH()
{
	return _h;
}

// Draws the image
void Image::draw() {
	SDL_Rect rect = { _x, _y, _w, _h };
	SDL_RenderCopy(_renderer, _texture, nullptr, &rect);
}