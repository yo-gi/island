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

	//Initialize
	bool initialize();
	bool loadMedia();
	void destruct();
	void loadMap();

	//Background
	void displayBackground();
	void updateCamera();
	void centerCamera(int componentIndex);
	
	//Entity management
	int createEntity();
	void destroyEntity(int i);

	void createHero(int x, int y);
	void createTree(int x, int y);

	//_Systems
	void movementSystem();
	void animationSystem();
	void selectionSystem();

	//Event handling
	void eventHandler(SDL_Event& event);
	void mouseHandler();


private:

	void cutTrees(int x, int y);

	//Movement
	bool collisionChecker(int index, int x, int y);

	void updatePosition(int index, int x, int y);
	void setDestination(int index, int x, int y);

	void assignDestinations(int x, int y);
	void updateDestination(int index, int x, int y);
	bool moveStep(int index, int destX, int destY);

	//Textures and Sprites
	Texture tileTexture;
	Sprite selectSprite;
	Sprite mouseSprite;

	//Maps
	Map map;
	entityData entityMap[MAX_TILES];

	//Component parts
	std::vector<int> cMasks;
	
	std::vector<SDL_Point> cPositions;
	std::vector<SDL_Point> cCoordinates;
	std::vector<SDL_Point> cDestinations;
	std::vector<Velocity> cVelocities;
	std::vector<Sprite> cSprites;
	std::vector<entityType> cTypes;

	bool findEntity(int index, entityType type);

	//Selection
	std::vector<int> selected;
	bool doneSelecting;
	SDL_Point mouse;
	SDL_Point mouseCoordinate;
	SDL_Point clickStart;
	SDL_Point clickEnd;

	//Component access
	std::vector<int> treeNums;
	std::vector<int> heroNums;
	
	//Camera
	int cameraVelX;
	int cameraVelY;

	

};

