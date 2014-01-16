#include "Sprite.h"
#include "Texture.h"

Sprite::Sprite() 
{
	frame.x = 0;
	frame.y = 0;
	frame.w = 0;
	frame.h = 0;
	speed = 4;

	currentFrame = 0;
	numFrames = 0;
}

bool Sprite::initialize(std::string path, int width, 
	int height, int numberOfFrames, int animationSpeed) 
{
	if (!baseTexture.loadFrom(path)) return false;
	
	frame.x = 0;
	frame.y = 0;
	frame.w = width;
	frame.h = height;

	currentFrame = 0;
	numFrames = numberOfFrames;
	speed = animationSpeed;
	return true;
}

void Sprite::animate(int x, int y) 
{
	SDL_Rect srcRect;

	srcRect.x = frame.x + ((currentFrame / speed) * frame.w);
	srcRect.y = frame.y;

	srcRect.w = frame.w;
	srcRect.h = frame.h;

	++currentFrame;
	if (currentFrame == speed * numFrames) {
		currentFrame = 0;
	}

	baseTexture.render(x, y, &srcRect);
}

int Sprite::getWidth()
{
	return frame.w;
}

int Sprite::getHeight()
{
	return frame.h;
}

SDL_Rect Sprite::getBox()
{
	return frame;
}