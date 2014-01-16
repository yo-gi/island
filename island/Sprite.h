#pragma once

#include "Main.h"
#include "Texture.h"

#ifndef SPRITE_H
#define SPRITE_H

class Texture;

class Sprite
{
public:
	Sprite();

	bool initialize(std::string path, int width, 
		int height, int numberOfFrames, int animationSpeed);
	
	void animate(int x, int y);

	int getWidth();
	int getHeight();
<<<<<<< HEAD
=======
	SDL_Rect getBox();
>>>>>>> 8ed56953ad5890faa096d84d901d756093b2527b

private:
	Texture baseTexture;
	SDL_Rect frame;

	int currentFrame;
	int numFrames;
	int speed;
};

#endif