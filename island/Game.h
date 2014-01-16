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
	void updateCamera(SDL_Rect& camera);

	//Event handling
	void eventHandler(SDL_Event& event);
	void mouseHandler();

private:

	Texture tileTexture;
	Sprite mouseSprite;

	Tile* sector[MAX_TILES];
	SDL_Rect tileClips[MAX_TILE_TYPES];

	std::vector<int> componentMasks;
	std::vector<SDL_Point> componentPositions;
	std::vector<SDL_Point> componentCoordinates;
	std::vector<Velocity> componentVelocities;
	std::vector<Sprite> componentSprites;

	int heroNum;
	int mouseNum;

	int cameraVelX;
	int cameraVelY;


	int mouseX;
	int mouseY;


};

