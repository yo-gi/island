#include "Tile.h"

using namespace std;

Tile::Tile(int x, int y, int newTileType)
{
	tileBox.x = x;
	tileBox.y = y;

	tileBox.w = TILE_WIDTH;
	tileBox.h = TILE_WIDTH;

	tileType = newTileType;
}

void Tile::render(Texture* tileTexture, SDL_Rect* clip)
{
	tileTexture->render(tileBox.x - camera.x, tileBox.y - camera.y, clip);
}

int Tile::getType()
{
	return tileType;
}

void Tile::setType(int type)
{
	tileType = type;
}

SDL_Rect Tile::getBox()
{
	return tileBox;
}

int Tile::getX()
{
	return tileBox.x;
}

int Tile::getY()
{
	return tileBox.y;
}