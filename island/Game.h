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
	void reloadBackground(); //trash function, just for testing	//way to junk up the code man

	bool loadSector(int sectorNumber);
	void loadTileClips();
	void loadRandomSector();

	int createEntity();
	void destroyEntity(int i);

	void createHero(int x, int y);
	void createTree(int x, int y);

	void movementSystem();
	void animationSystem();
	void updateCamera(SDL_Rect& camera);
	void centerCamera(int componentIndex, SDL_Rect& camera);

	//Event handling
	void eventHandler(SDL_Event& event);
	void mouseHandler();


private:

	bool collisionChecker(int x, int y);
	void cutTrees(int x, int y);

	Texture tileTexture;
	Sprite mouseSprite;

	Tile* sector[MAX_TILES];
	entityData entityMap[MAX_TILES];

	SDL_Rect tileClips[MAX_TILE_TYPES];

	std::vector<int> componentMasks;
	std::vector<SDL_Point> componentPositions;
	std::vector<SDL_Point> componentCoordinates;
	std::vector<Velocity> componentVelocities;
	std::vector<Sprite> componentSprites;

	int heroNum;

	std::vector<int> treeNums;
	
	int cameraVelX;
	int cameraVelY;

	int mouseX;
	int mouseY;



};

