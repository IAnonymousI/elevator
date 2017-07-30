#include "Image.h"
#include <iostream>

Image::Image(SDL_Renderer* r, int x, int y, int w, int h, const std::string path) :
_renderer(r), _x(x), _y(y), _w(w), _h(h){
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

Image::~Image(){
	SDL_DestroyTexture(_texture);
}

void Image::draw() {
	SDL_Rect rect = { _x, _y, _w, _h };
	SDL_RenderCopy(_renderer, _texture, nullptr, &rect);
}