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
	void setAlpha(Uint8 alpha);
	bool textRender(std::string text, TTF_Font* font, int r, int g, int b);
	void setSize(int width, int height);

	int getWidth();
	int getHeight();

	SDL_Rect getBox();


private:
	Texture baseTexture;
	SDL_Rect frame;

	int currentFrame;
	int numFrames;
	int speed;
};

#endif