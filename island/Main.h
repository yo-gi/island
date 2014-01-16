#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <fstream>

const int MAX_ENTITIES = 50;

<<<<<<< HEAD
const int LEVEL_WIDTH = 32;
const int LEVEL_HEIGHT = 32;
const int MAX_TILE_TYPES = 2;
const int MAX_TILES = LEVEL_HEIGHT * LEVEL_WIDTH;

const int TILE_WIDTH = 64;

const int SCREEN_HEIGHT = TILE_WIDTH*9;
const int SCREEN_WIDTH = TILE_WIDTH*12;
=======
const int LEVEL_WIDTH = 16;
const int LEVEL_HEIGHT = 16;
const int MAX_TILE_TYPES = 2;
const int MAX_TILES = LEVEL_HEIGHT * LEVEL_WIDTH;

const int SCREEN_HEIGHT = 64*LEVEL_HEIGHT/2;
const int SCREEN_WIDTH = 64*LEVEL_WIDTH/2;
>>>>>>> 8ed56953ad5890faa096d84d901d756093b2527b

const int HERO_VEL = 1;
const int CAMERA_VEL = 1;

extern SDL_Renderer* mainRenderer;
extern SDL_Window* mainWindow;

extern SDL_Rect camera;

<<<<<<< HEAD
=======
const int TILE_WIDTH = 64;

>>>>>>> 8ed56953ad5890faa096d84d901d756093b2527b
const int TILE_WATER = 0;
const int TILE_SAND = 1;

typedef enum
{
	COMPONENT_NONE = 0,
	COMPONENT_POSITION = 1 << 0,
	COMPONENT_VELOCITY = 1 << 1,
	COMPONENT_SPRITE = 1 << 2
} Component;

const int MOVEMENT_MASK = (COMPONENT_POSITION | COMPONENT_VELOCITY);



bool checkCollision(SDL_Rect one, SDL_Rect two);