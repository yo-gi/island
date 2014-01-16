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
<<<<<<< HEAD
	void createTree(int x, int y);

	void movementSystem();
	void animationSystem();
	void updateCamera(SDL_Rect& camera);
	void centerCamera(int componentIndex, SDL_Rect& camera);
	void eventHandler(SDL_Event& event);
=======
		
	void movementSystem();
	void animationSystem();
	void updateCamera(SDL_Rect& camera);

	//Event handling
	void eventHandler(SDL_Event& event);
	void mouseHandler();
>>>>>>> 8ed56953ad5890faa096d84d901d756093b2527b

private:

	Texture tileTexture;
<<<<<<< HEAD
=======
	Sprite mouseSprite;

	Tile* sector[MAX_TILES];
	SDL_Rect tileClips[MAX_TILE_TYPES];
>>>>>>> 8ed56953ad5890faa096d84d901d756093b2527b

	std::vector<int> componentMasks;
	std::vector<SDL_Point> componentPositions;
	std::vector<SDL_Point> componentCoordinates;
	std::vector<Velocity> componentVelocities;
	std::vector<Sprite> componentSprites;

	int heroNum;
<<<<<<< HEAD
	std::vector<int> treeNums;
	
	int cameraVelX;
	int cameraVelY;

	Tile* sector[MAX_TILES];
	SDL_Rect tileClips[MAX_TILE_TYPES];
	bool collisionChecker(int x, int y);
=======
	int mouseNum;

	int cameraVelX;
	int cameraVelY;


	int mouseX;
	int mouseY;


>>>>>>> 8ed56953ad5890faa096d84d901d756093b2527b
};

