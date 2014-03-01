#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <algorithm>

#include <string>
#include <vector>
#include <set>
#include <queue>
#include <stack>

extern SDL_Renderer* mainRenderer;
extern SDL_Window* mainWindow;

extern SDL_Rect camera;

const int MAX_ENTITIES = 50;
const int TILE_WIDTH = 64; //pixels


const int LEVEL_WIDTH = 16;
const int LEVEL_HEIGHT = 16;
const int SCREEN_HEIGHT = TILE_WIDTH*13;
const int SCREEN_WIDTH = TILE_WIDTH*13;

const int MINIMAP_WIDTH = 320;
const int MINIMAP_HEIGHT = 256;

const int MAX_TILE_TYPES = 3;
const int MAX_TILES = LEVEL_HEIGHT * LEVEL_WIDTH;

const int HERO_VEL = 1;
const int CAMERA_VEL = 1;

const int TILE_WATER = 0;
const int TILE_SAND = 1;
const int TILE_LAND = 2;

struct searchTile
{
	int index;
	int parentIndex;
	bool isInOpen;
	bool isInClosed;
	int exactCost;
	int estimatedCost;
};

typedef enum
{
	COMPONENT_NONE = 0,
	COMPONENT_POSITION = 1 << 0,
	COMPONENT_VELOCITY = 1 << 1,
	COMPONENT_SPRITE = 1 << 2,
	COMPONENT_CLICKABLE = 1 << 3,
	COMPONENT_DESTINATION = 1 << 4,
	COMPONENT_PATH = 1 << 5,
	COMPONENT_MINIMAP = 1 << 6

} Component;

const int MOVEMENT_MASK = (COMPONENT_POSITION | COMPONENT_VELOCITY);

typedef enum
{
	HERO,
	TREE,
} entityType;

typedef enum
{
	WATER,
	SAND,
	LAND
} terrainType;

struct entityData
{
	int index;
	terrainType terrain;
	std::set<entityType> types;
};

struct pathComparator
{
	bool operator() (const searchTile* one, const searchTile* two) const
	{
		if (one->exactCost + one->estimatedCost < 
			two->exactCost + two->estimatedCost) return true;
		else return false;
	}
};

bool checkCollision(SDL_Rect one, SDL_Rect two);
int manDist(SDL_Point one, SDL_Point two);
int diagDist(SDL_Point one, SDL_Point two);

typedef std::priority_queue<searchTile*, std::vector<searchTile*>, 
	pathComparator> TileQueue;


