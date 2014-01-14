#pragma once

#include "Main.h"
#include "Texture.h"
#include "Velocity.h"
#include "Sprite.h"
#include "Tile.h"

class Game
{
public:
	Game();
	~Game();

	bool initialize();
	bool loadMedia();
	void destruct();

	void displayBackground();
	void reloadBackground(); //trash function, just for testing

	bool loadSector(int sectorNumber);
	void loadTileClips();
	void loadRandomSector();

	int createEntity();
	void destroyEntity(int i);

	void createHero();
	
	void movementSystem();
	void animationSystem();
	void centerCamera(SDL_Rect& camera);
	void eventHandler(SDL_Event& event);

private:

	Texture tileTexture;

	std::vector<int> componentMasks;
	std::vector<SDL_Point> componentPositions;
	std::vector<SDL_Point> componentCoordinates;
	std::vector<Velocity> componentVelocities;
	std::vector<Sprite> componentSprites;

	int heroNum;

	int cameraVelX;
	int cameraVelY;

	Tile* sector[MAX_TILES];
	SDL_Rect tileClips[MAX_TILE_TYPES];

};

