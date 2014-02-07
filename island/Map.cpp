#include "Map.h"

using namespace std;

Map::Map()
{
	int x, y;

	/*for (int i = 0; i < MAX_TILES; ++i)
	{
		x = (i % LEVEL_WIDTH) * TILE_WIDTH;
		y = (i / LEVEL_HEIGHT) * TILE_WIDTH;

		sector[i] = new Tile(x, y, 0);
	}*/

	for (int i = 0; i < MAX_TILE_TYPES; ++i)
	{
		tileClips[i].w = TILE_WIDTH;
		tileClips[i].h = TILE_WIDTH;
	}

	loadTileClips();
	rooms.resize(16*16);
}

bool Map::loadSector(int sectorNumber)
{
	if (!tileTexture.loadFrom("images/texture.png"))
	{
		cout << "Couldn't load tile texture\n";
	}

	string path;
	path.append("sectors/sector_");
	path.append(to_string(sectorNumber));
	path.append(".txt");

	ifstream sectorFile(path);

	if (sector == NULL)
	{
		cout << "Couldn't load sector file at " << path << endl;
		return false;
	}
	else
	{
		int x, y, type;

		for (int i = 0; i < MAX_TILES; ++i)
		{
			x = (i % LEVEL_WIDTH) * TILE_WIDTH;
			y = (i / LEVEL_WIDTH) * TILE_WIDTH;
			sectorFile >> type;

			//cout << "type: " << type << endl;
			sector[i] = new Tile(x, y, type);
		}
	}

	sectorFile.close();
	return true;
}

void Map::loadRandomSector()
{
	int x, y, type;

	for (int i = 0; i < MAX_TILES; ++i)
	{
		x = (i % LEVEL_WIDTH) * TILE_WIDTH;
		y = (i / LEVEL_HEIGHT) * TILE_WIDTH;
		type = rand() % 2;

		sector[i] = new Tile(x, y, type);
	}
}

void Map::renderMap()
{
	for (int i = 0; i < MAX_TILES; ++i)
	{
		sector[i]->render(&tileTexture, &tileClips[sector[i]->getType()]);
	}
}

void Map::loadTileClips()
{
	tileClips[TILE_WATER].x = TILE_WIDTH;
	tileClips[TILE_WATER].y = 0;

	tileClips[TILE_SAND].x = 0;
	tileClips[TILE_SAND].y = 0;

	tileClips[TILE_LAND].x = 2*TILE_WIDTH;
	tileClips[TILE_LAND].y = 0;
}

int Map::getType(int index)
{
	return sector[index]->getType();
}

void Map::generateSector()
{
	int x, y;

	for (int i = 0; i < MAX_TILES; ++i)
	{
		x = (i % LEVEL_WIDTH) * TILE_WIDTH;
		y = (i / LEVEL_WIDTH) * TILE_WIDTH;

		if (rand() % 100 < 10)
		{ 
			sector[i] = new Tile(x, y, 1);
			cout << "1 " << sector[i]->getType() << endl;
		}
		else 
		{
			sector[i] = new Tile(x, y, 0);
			cout << "0 " << sector[i]->getType() << endl;
		}

		//cout << sector[i]->getX() << ", " << sector[i]->getY() << endl << endl;
	}
	
	//SDL_RenderClear(mainRenderer);


}