#include "Game.h"

using namespace std;

Game::Game()
{
	mainWindow = NULL;
	mainRenderer = NULL;

	componentMasks.resize(MAX_ENTITIES);
	componentVelocities.resize(MAX_ENTITIES);
	componentSprites.resize(MAX_ENTITIES);
	componentPositions.resize(MAX_ENTITIES);
	componentCoordinates.resize(MAX_ENTITIES);

	for (auto i : componentMasks)
	{
		i = COMPONENT_NONE;
	}

	heroNum = 0;
	treeNums.push_back(1);
	treeNums.push_back(2);

	cameraVelX = 0;
	cameraVelY = 0;

	mouse.x = 0;
	mouse.y = 0;

	mouseCoordinate.x = 0;
	mouseCoordinate.y = 0;

	clickStart.x = 0;
	clickStart.y = 0;

	clickEnd.x = 0;
	clickEnd.y = 0;

	for(int i = 0; i < MAX_TILES; ++i)
	{
		entityMap[i].type = NONE;
	}

	doneSelecting = false;
}

Game::~Game()
{
	destruct();
}

bool Game::initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "Couldn't initialize SDL: " << SDL_GetError() << "\n";
		return false;
	}

	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		cout << "Render scale quality not set" << "\n";
	}

	mainWindow = SDL_CreateWindow("island", 
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (mainWindow == NULL)
	{
		cout << "Couldn't initialize window: " << SDL_GetError() << "\n";
		return false; 
	}
	
	mainRenderer = SDL_CreateRenderer(mainWindow, -1, 
		SDL_RENDERER_ACCELERATED);

	if (mainRenderer == NULL)
	{
		cout << "Couldn't initialize renderer: " << SDL_GetError() << "\n";
		return false;
	}

	SDL_SetRenderDrawColor(mainRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		cout << "Couldn't initialize SDL_Image: " << SDL_GetError() << "\n";
		return false;
	}

	SDL_SetRelativeMouseMode((SDL_bool)true);

	return true;
}

bool Game::loadMedia()
{
	if (!mouseSprite.initialize("images/cursor.png", 32, 32, 1, 1))
	{
		cout << "Couldn't load mouse texture\n";
		return false;
	}

	return true;
}

void Game::destruct()
{
	SDL_DestroyWindow(mainWindow);
	SDL_DestroyRenderer(mainRenderer);
	mainWindow = NULL;
	mainRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
}

void Game::displayBackground()
{
	map.renderMap();
}

void Game::loadMap()
{
	if (!map.loadSector(1))
	{
		cout << "Couldn't load sector 1" << endl;
		exit(1);
	}
	//map.generateSector();
}

//ECS
int Game::createEntity()
{
	for (int i = 0; i < MAX_ENTITIES; ++i)
	{
		if (componentMasks[i] == COMPONENT_NONE)
		{
			return i;
		}
	}

	return -1;
}

void Game::destroyEntity(int i)
{
	//need to handle other component* stuff.
	//also my solution is probs bad
	componentMasks[i] = COMPONENT_NONE;
	componentCoordinates[i].x = NULL;
	componentCoordinates[i].y = NULL;
}

void Game::eventHandler(SDL_Event& event)
{

	mouseHandler();

	if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
	{
		switch(event.key.keysym.sym)
		{
			case SDLK_w:
				if(!collisionChecker(componentCoordinates[heroNum].x,
				                 componentCoordinates[heroNum].y - HERO_VEL))
				{
					entityMap[componentCoordinates[heroNum].y*LEVEL_WIDTH
					          +componentCoordinates[heroNum].x].type = NONE;

					componentCoordinates[heroNum].y -= HERO_VEL;
					entityMap[componentCoordinates[heroNum].y*LEVEL_WIDTH
					          +componentCoordinates[heroNum].x].type = HERO;
				}
				break;
			case SDLK_s:
				if(!collisionChecker(componentCoordinates[heroNum].x,
				                 componentCoordinates[heroNum].y + HERO_VEL))
				{
					entityMap[componentCoordinates[heroNum].y*LEVEL_WIDTH
					          +componentCoordinates[heroNum].x].type = NONE;
					componentCoordinates[heroNum].y += HERO_VEL;
					entityMap[componentCoordinates[heroNum].y*LEVEL_WIDTH
					          +componentCoordinates[heroNum].x].type = HERO;
				}
				break;
			case SDLK_a:
				if(!collisionChecker(componentCoordinates[heroNum].x - HERO_VEL,
				                 componentCoordinates[heroNum].y))
				{
					entityMap[componentCoordinates[heroNum].y*LEVEL_WIDTH
					          +componentCoordinates[heroNum].x].type = NONE;
					componentCoordinates[heroNum].x -= HERO_VEL;
					entityMap[componentCoordinates[heroNum].y*LEVEL_WIDTH
					          +componentCoordinates[heroNum].x].type = HERO;
				}
				break;
			case SDLK_d:
				if(!collisionChecker(componentCoordinates[heroNum].x + HERO_VEL,
				                 componentCoordinates[heroNum].y))
				{
					entityMap[componentCoordinates[heroNum].y*LEVEL_WIDTH
					          +componentCoordinates[heroNum].x].type = NONE;
					componentCoordinates[heroNum].x += HERO_VEL;
					entityMap[componentCoordinates[heroNum].y*LEVEL_WIDTH
					          +componentCoordinates[heroNum].x].type = HERO;
				}
				break;
			case SDLK_SPACE:
				centerCamera(heroNum, camera);
				break;
			case SDLK_c:
				cutTrees(componentCoordinates[heroNum].x,
				         componentCoordinates[heroNum].y);
				break;
		}
	}

	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			selected.clear();

			doneSelecting = false;
			clickStart.x = mouseCoordinate.x;
			clickStart.y = mouseCoordinate.y;

			cout << clickStart.x << ", " << clickStart.y << endl;
		}

		if (event.button.button == SDL_BUTTON_RIGHT)
		{
			if (!selected.empty())
			{
				for (int i = 0; i < selected.size(); ++i)
				{
					cout << selected[i] << ", " << heroNum << endl;
					updatePosition(selected[i], mouseCoordinate.x, 
						mouseCoordinate.y);	
				}	
			}
			
		}
	}

	if (event.type == SDL_MOUSEBUTTONUP)
	{
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			doneSelecting = true;
			clickEnd.x = mouseCoordinate.x;
			clickEnd.y = mouseCoordinate.y;

			cout << clickEnd.x << ", " << clickEnd.y << ", ";

			selectionSystem();
			cout << selected.size() << endl;

		}
	}

	if (componentCoordinates[heroNum].x < 0) componentCoordinates[heroNum].x += HERO_VEL;
	if (componentCoordinates[heroNum].x > LEVEL_WIDTH - 1) componentCoordinates[heroNum].x -= HERO_VEL;
	if (componentCoordinates[heroNum].y < 0) componentCoordinates[heroNum].y += HERO_VEL;
	if (componentCoordinates[heroNum].y > LEVEL_HEIGHT - 1) componentCoordinates[heroNum].y -= HERO_VEL;
}

void Game::centerCamera(int componentIndex, SDL_Rect& camera)
{
	camera.x = componentCoordinates[componentIndex].x*TILE_WIDTH
	+ TILE_WIDTH/2 - SCREEN_WIDTH/2;
	camera.y = componentCoordinates[componentIndex].y*TILE_WIDTH
	+ TILE_WIDTH/2 - SCREEN_HEIGHT/2;

	if (componentCoordinates[heroNum].x < 0) 
		componentCoordinates[heroNum].x += HERO_VEL;

	if (componentCoordinates[heroNum].x > LEVEL_WIDTH - 1) 
		componentCoordinates[heroNum].x -= HERO_VEL;
	
	if (componentCoordinates[heroNum].y < 0) 
		componentCoordinates[heroNum].y += HERO_VEL;
	
	if (componentCoordinates[heroNum].y > LEVEL_HEIGHT - 1) 
		componentCoordinates[heroNum].y -= HERO_VEL;

	//cout << componentCoordinates[heroNum].x << ", " << componentCoordinates[heroNum].y << endl;
	cout << mouse.x << ", " << mouse.y << ", " << mouseCoordinate.x << ", ";
	cout << mouseCoordinate.y << endl;
}

void Game::mouseHandler()
{
	SDL_GetMouseState(&mouse.x, &mouse.y);

	mouseCoordinate.x = (mouse.x + camera.x) / TILE_WIDTH;
	mouseCoordinate.y = (mouse.y + camera.y) / TILE_WIDTH;
}

void Game::updateCamera(SDL_Rect& camera)
{

	if (mouse.x < 10) 				camera.x -= CAMERA_VEL;
	if (mouse.x > SCREEN_WIDTH - 10) camera.x += CAMERA_VEL;
	if (mouse.y < 10) 				camera.y -= CAMERA_VEL;
	if (mouse.y > SCREEN_HEIGHT - 10) camera.y += CAMERA_VEL;

	if (camera.x < 0) camera.x = 0;
	if (camera.y < 0) camera.y = 0;

	if (camera.x + SCREEN_WIDTH > (LEVEL_WIDTH * TILE_WIDTH))
	{
		camera.x = (LEVEL_WIDTH * TILE_WIDTH) - SCREEN_WIDTH;
	}
	if (camera.y + SCREEN_HEIGHT > (LEVEL_HEIGHT * TILE_WIDTH))
	{
		camera.y = (LEVEL_HEIGHT * TILE_WIDTH) - SCREEN_HEIGHT;
	}

	if (camera.x < 0) camera.x = 0;
	if (camera.y < 0) camera.y = 0;
}

void Game::updatePosition(int index, int x, int y)
{
	componentCoordinates[index].x = x;
	componentCoordinates[index].y = y;

	componentPositions[index].x = TILE_WIDTH/4 + x*TILE_WIDTH;
	componentPositions[index].y = TILE_WIDTH/4 + y*TILE_WIDTH;
}

void Game::createHero(int x, int y)
{
	heroNum = createEntity();
	componentMasks[heroNum] = 
		COMPONENT_VELOCITY | COMPONENT_SPRITE | COMPONENT_POSITION
		| COMPONENT_CLICKABLE;

	componentCoordinates[heroNum].x = x;
	componentCoordinates[heroNum].y = y;

	componentPositions[heroNum].x = TILE_WIDTH/4 + x*TILE_WIDTH;
	componentPositions[heroNum].y = TILE_WIDTH/4 + y*TILE_WIDTH;

	componentVelocities[heroNum].x = 0;
	componentVelocities[heroNum].y = 0;

	componentSprites[heroNum].initialize("images/hero.png", 
		32, 32, 1, 1);

	entityMap[y * LEVEL_WIDTH + x].type  = HERO;
	entityMap[y * LEVEL_WIDTH + x].index = heroNum;
}

void Game::createTree(int x, int y)
{
	treeNums.push_back(createEntity());
	componentMasks[treeNums.back()] = 
		COMPONENT_VELOCITY | COMPONENT_SPRITE | COMPONENT_POSITION
		| COMPONENT_CLICKABLE;

	componentPositions[treeNums.back()].x = TILE_WIDTH/4;
	componentPositions[treeNums.back()].y = TILE_WIDTH/4;

	componentCoordinates[treeNums.back()].x = x;
	componentCoordinates[treeNums.back()].y = y;

	componentVelocities[treeNums.back()].x = 0;
	componentVelocities[treeNums.back()].y = 0;
	
	componentSprites[treeNums.back()].initialize("images/tree.png", 
		32, 32, 1, 1);

	entityMap[y * LEVEL_WIDTH+x].type  = TREE;
	entityMap[y * LEVEL_WIDTH+x].index = treeNums.back();
}

void Game::movementSystem()
{
	for (int i = 0; i < MAX_ENTITIES; ++i)
	{
		if ((componentMasks[i] & MOVEMENT_MASK) == MOVEMENT_MASK)
		{
			componentPositions[i].x += componentVelocities[i].x;
			componentPositions[i].y += componentVelocities[i].y;
		}

		if (componentPositions[i].y < 0 || componentPositions[i].y > 
			(LEVEL_HEIGHT*TILE_WIDTH - componentSprites[i].getHeight()))
		{
			componentPositions[i].y -= componentVelocities[i].y;
		}

		if (componentPositions[i].x < 0 || componentPositions[i].x >
			(LEVEL_WIDTH*TILE_WIDTH - componentSprites[i].getHeight()))
		{
			componentPositions[i].x -= componentVelocities[i].x;
		}

		componentPositions[i].x = TILE_WIDTH * componentCoordinates[i].x + TILE_WIDTH/4;
		componentPositions[i].y = TILE_WIDTH * componentCoordinates[i].y + TILE_WIDTH/4;
		
	}
}

void Game::animationSystem()
{
	for (int i = 0; i < MAX_ENTITIES; ++i)
	{
		if (componentMasks[i] & COMPONENT_SPRITE == COMPONENT_SPRITE)
		{
			componentSprites[i].animate(componentPositions[i].x - camera.x, 
				componentPositions[i].y - camera.y);
		}
	}

	mouseSprite.animate(mouse.x, mouse.y);
}

void Game::selectionSystem()
{
	for (int i = clickStart.x; i <= clickEnd.x; ++i)
	{
		for (int j = clickStart.y; j <= clickEnd.y; ++j)
		{
			if (entityMap[j * LEVEL_WIDTH + i].type == HERO)
			{
				selected.push_back(entityMap[j * LEVEL_WIDTH + i].index);
			}
		}
	}

}

bool Game::collisionChecker(int x, int y)
{
	for (int i = 0; i < MAX_ENTITIES; ++i)
	{
		if ((componentCoordinates[i].x == x && componentCoordinates[i].y == y)
		    || map.getType(y*LEVEL_WIDTH + x) == 0)
		{
			return true;
		}
	}
	return false;
}

void Game::cutTrees(int x, int y)
{
	int i = y*LEVEL_WIDTH+x+1;
	if(i < MAX_TILES)
	{
		if(entityMap[i].type == TREE)
		{
			destroyEntity(entityMap[i].index);
		}
	}
	i = (y+1)*LEVEL_WIDTH+x+1;
	if(i < MAX_TILES)
	{
		if(entityMap[i].type == TREE)
		{
			destroyEntity(entityMap[i].index);
		}
	}
	i = (y-1)*LEVEL_WIDTH+x+1;
	if(i < MAX_TILES)
	{
		if(entityMap[i].type == TREE)
		{
			destroyEntity(entityMap[i].index);
		}
	}
	i = y*LEVEL_WIDTH+x-1;
	if(i < MAX_TILES)
	{
		if(entityMap[i].type == TREE)
		{
			destroyEntity(entityMap[i].index);
		}
	}
	i = (y+1)*LEVEL_WIDTH+x-1;
	if(i < MAX_TILES)
	{
		if(entityMap[i].type == TREE)
		{
			destroyEntity(entityMap[i].index);
		}
	}
	i = (y-1)*LEVEL_WIDTH+x-1;
	if(i < MAX_TILES)
	{
		if(entityMap[i].type == TREE)
		{
			destroyEntity(entityMap[i].index);
		}
	}
	i = (y+1)*LEVEL_WIDTH+x;
	if(i < MAX_TILES)
	{
		if(entityMap[i].type == TREE)
		{
			destroyEntity(entityMap[i].index);
		}
	}
	i = (y-1)*LEVEL_WIDTH+x;
	if(i < MAX_TILES)
	{
		if(entityMap[i].type == TREE)
		{
			destroyEntity(entityMap[i].index);
		}
	}
}
