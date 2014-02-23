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

extern SDL_Renderer* mainRenderer;
extern SDL_Window* mainWindow;

extern SDL_Rect camera;

const int MAX_ENTITIES = 50;

const int LEVEL_WIDTH = 16;
const int LEVEL_HEIGHT = 16;
const int SCREEN_HEIGHT = TILE_WIDTH*13;
const int SCREEN_WIDTH = TILE_WIDTH*13;

const int TILE_WIDTH = 64; //pixels

const int MAX_TILE_TYPES = 3;
const int MAX_TILES = LEVEL_HEIGHT * LEVEL_WIDTH;

const int HERO_VEL = 1;
const int CAMERA_VEL = 1;

const int TILE_WATER = 0;
const int TILE_SAND = 1;
const int TILE_LAND = 2;

typedef enum
{
	COMPONENT_NONE = 0,
	COMPONENT_POSITION = 1 << 0,
	COMPONENT_VELOCITY = 1 << 1,
	COMPONENT_SPRITE = 1 << 2,
	COMPONENT_CLICKABLE = 1 << 3,
	COMPONENT_DESTINATION = 1 << 4,
	COMPONENT_TEST = 1 << 5

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

bool checkCollision(SDL_Rect one, SDL_Rect two);
int manDist(SDL_Point one, SDL_Point two);


