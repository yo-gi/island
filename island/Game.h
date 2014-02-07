#pragma once

#include "Main.h"
#include "Texture.h"
#include "Velocity.h"
#include "Sprite.h"
#include "Tile.h"
#include "Map.h"

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

	void loadMap();

	int createEntity();
	void destroyEntity(int i);

	void createHero(int x, int y);
	void createTree(int x, int y);

	void movementSystem();
	void animationSystem();
	void selectionSystem();

	void updateCamera(SDL_Rect& camera);
	void centerCamera(int componentIndex, SDL_Rect& camera);

	//Event handling
	void eventHandler(SDL_Event& event);
	void mouseHandler();


private:

	bool collisionChecker(int x, int y);
	void cutTrees(int x, int y);
	void updatePosition(int index, int x, int y);

	Texture tileTexture;
	Sprite mouseSprite;

	Map map;

	entityData entityMap[MAX_TILES];

	std::vector<int> componentMasks;
	std::vector<SDL_Point> componentPositions;
	std::vector<SDL_Point> componentCoordinates;
	std::vector<Velocity> componentVelocities;
	std::vector<Sprite> componentSprites;

	std::vector<int> selected;

	int heroNum;

	std::vector<int> treeNums;
	
	int cameraVelX;
	int cameraVelY;

	SDL_Point mouse;
	SDL_Point mouseCoordinate;
	SDL_Point clickStart;
	SDL_Point clickEnd;

	bool doneSelecting;
};

