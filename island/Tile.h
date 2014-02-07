#pragma once

#include "Main.h"
#include "Texture.h"

class Tile
{
public:
	Tile(int x, int y, int newTileType);
	void render(Texture* tileTexture, SDL_Rect* clip);
	
	int getType();
	void setType(int type);

	SDL_Rect getBox();
	int getX();
	int getY();
private:
	SDL_Rect tileBox;
	int tileType;

};
