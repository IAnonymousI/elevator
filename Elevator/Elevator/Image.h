#pragma once
#include <SDL/SDL_image.h>
#include <string>

class Image
{

public:
	Image();
	~Image();

	void init(SDL_Renderer* r, int x, int y, int w, int h, const std::string path);
	void changeImage(const std::string path);

	int getX();
	int getY();
	int getW();
	int getH();

	void draw();

private:

	int _x, _y;
	int _w, _h;

	std::string image_path;
	SDL_Renderer* _renderer;
	SDL_Texture* _texture;

};