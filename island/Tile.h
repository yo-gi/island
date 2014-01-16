#pragma once

#include "Main.h"
#include "Texture.h"

class Tile
{
public:
	Tile(int x, int y, int newTileType);
	void render(Texture* tileTexture, SDL_Rect* clip);
	int getType();
	SDL_Rect getBox();
private:
	SDL_Rect tileBox;
	int tileType;


};

