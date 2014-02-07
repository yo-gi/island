#pragma once

#include "Main.h"
#include "Tile.h"
#include "Texture.h"

class Map
{
public:
	Map();

	bool loadSector(int sectorNumber);
	void generateSector();
	void loadRandomSector();
	void loadTileClips();


	void renderMap();

	int getType(int index);

private:
	Texture tileTexture;
	Tile* sector[MAX_TILES];
	SDL_Rect tileClips[MAX_TILE_TYPES];

	std::vector<SDL_Rect> rooms;


};