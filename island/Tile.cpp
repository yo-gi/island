#include "Tile.h"

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

SDL_Rect Tile::getBox()
{
	return tileBox;
}