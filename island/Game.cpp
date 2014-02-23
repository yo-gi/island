#include "Game.h"

using namespace std;

Game::Game()
{
	mainWindow = NULL;
	mainRenderer = NULL;

	cMasks.resize(MAX_ENTITIES);
	cVelocities.resize(MAX_ENTITIES);
	cSprites.resize(MAX_ENTITIES);
	cPositions.resize(MAX_ENTITIES);
	cDestinations.resize(MAX_ENTITIES);
	cPaths.resize(MAX_ENTITIES);
	cCoordinates.resize(MAX_ENTITIES);
	cTypes.resize(MAX_ENTITIES);
	pathMap.resize(MAX_TILES);

	for (auto i : cMasks)
	{
		i = COMPONENT_NONE;
	}

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

	doneSelecting = true;
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

	if (!selectSprite.initialize("images/select.png", 
		TILE_WIDTH, TILE_WIDTH, 1, 1))
	{
		cout << "Couldn't load selecting texture\n";
		return false;
	}
	else selectSprite.setAlpha(127);

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

	for(int i = 0; i < MAX_TILES; ++i)
	{
		//entityMap[i].type = NONE;
		if (map.getType(i) == TILE_WATER) entityMap[i].terrain = WATER;
		if (map.getType(i) == TILE_SAND) entityMap[i].terrain = SAND;
		if (map.getType(i) == TILE_LAND) entityMap[i].terrain = LAND;

		entityMap[i].index = -1;
	}
}

//ECS
int Game::createEntity()
{
	for (int i = 0; i < MAX_ENTITIES; ++i)
	{
		if (cMasks[i] == COMPONENT_NONE)
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
	cMasks[i] = COMPONENT_NONE;
	cCoordinates[i].x = NULL;
	cCoordinates[i].y = NULL;
}

bool Game::findEntity(int index, entityType type)
{
	if ( entityMap[index].types.find(type) != entityMap[index].types.end() )
	{
		return true;
	}
	else return false;
}

void Game::eventHandler(SDL_Event& event)
{

	mouseHandler();

	if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
	{
		switch(event.key.keysym.sym)
		{
			case SDLK_SPACE:
				if(!selected.empty())
				{
					//make better choices than selected.front
					centerCamera(selected.front());
				}
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

			cout << "start: " << clickStart.x << ", " << clickStart.y << endl;
		}

		if (event.button.button == SDL_BUTTON_RIGHT)
		{
			assignDestinations(mouseCoordinate.x, mouseCoordinate.y);
		}
	}

	if (event.type == SDL_MOUSEBUTTONUP)
	{
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			doneSelecting = true;
			clickEnd.x = mouseCoordinate.x;
			clickEnd.y = mouseCoordinate.y;

			cout << "end: " << clickEnd.x << ", " << clickEnd.y << ", ";

			selectionSystem();
			cout << selected.size() << endl;

		}
	}
}

void Game::centerCamera(int componentIndex)
{
	camera.x = cCoordinates[componentIndex].x*TILE_WIDTH
	+ TILE_WIDTH/2 - SCREEN_WIDTH/2;
	camera.y = cCoordinates[componentIndex].y*TILE_WIDTH
	+ TILE_WIDTH/2 - SCREEN_HEIGHT/2;
}

void Game::mouseHandler()
{
	SDL_GetMouseState(&mouse.x, &mouse.y);

	mouseCoordinate.x = (mouse.x + camera.x) / TILE_WIDTH;
	mouseCoordinate.y = (mouse.y + camera.y) / TILE_WIDTH;
}

void Game::updateCamera()
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
	int mapIndex = cCoordinates[index].y * LEVEL_WIDTH
		+ cCoordinates[index].x;

	entityType tempType = cTypes[index];
	entityMap[mapIndex].types.erase(tempType);
	entityMap[mapIndex].index = -1;	

	cCoordinates[index].x = x;
	cCoordinates[index].y = y;
	cPositions[index].x = TILE_WIDTH/4 + x*TILE_WIDTH;
	cPositions[index].y = TILE_WIDTH/4 + y*TILE_WIDTH;

	mapIndex = cCoordinates[index].y * LEVEL_WIDTH
		+ cCoordinates[index].x;

	entityMap[mapIndex].types.insert(tempType);
	entityMap[mapIndex].index = index;
}

void Game::createHero(int x, int y)
{
	int heroNum = createEntity();
	heroNums.push_back(heroNum);

	cMasks[heroNum] = MOVEMENT_MASK | COMPONENT_SPRITE |
					  COMPONENT_CLICKABLE | COMPONENT_MINIMAP | COMPONENT_PATH;
	cTypes[heroNum] = HERO;

	cSprites[heroNum].initialize("images/hero.png", 
		32, 32, 1, 1);

	entityMap[y * LEVEL_WIDTH + x].types.insert(HERO);

	cDestinations[heroNum].x = x;
	cDestinations[heroNum].y = y;

	updatePosition(heroNum, x, y);

}

void Game::createTree(int x, int y)
{
	int treeNum = createEntity();
	treeNums.push_back(treeNum);

	cMasks[treeNums.back()] = COMPONENT_SPRITE | COMPONENT_POSITION 
		| COMPONENT_CLICKABLE | COMPONENT_MINIMAP;
	cTypes[treeNum] = TREE;

	cSprites[treeNum].initialize("images/tree.png", 
		32, 32, 1, 1);

	entityMap[y * LEVEL_WIDTH + x].types.insert(TREE);

	cDestinations[treeNum].x = x;
	cDestinations[treeNum].y = y;

	updatePosition(treeNum, x, y);
}

void Game::movementSystem()
{
	for (int i = 0; i < MAX_ENTITIES; ++i)
	{
		if (cMasks[i] & COMPONENT_DESTINATION == COMPONENT_DESTINATION
			&& !(cCoordinates[i].x == cDestinations[i].x
			&& cCoordinates[i].y == cDestinations[i].y))
		{
			if (collisionChecker(i, cDestinations[i].x, cDestinations[i].y))
			{
				updateDestination(i, cDestinations[i].x, cDestinations[i].y);
			}

			if (!cPaths[i].empty() 
				&& moveStep(i, cPaths[i].top().x, cPaths[i].top().y))
			{
				updatePosition(i, cPaths[i].top().x, cPaths[i].top().y);
				cPaths[i].pop();
			}
		}
	}
}

bool Game::moveStep(int index, int destX, int destY)
{
	int curX = cPositions[index].x - TILE_WIDTH/4;
	int curY = cPositions[index].y - TILE_WIDTH/4;

	destX *= TILE_WIDTH;
	destY *= TILE_WIDTH;

	if (curX > destX) cPositions[index].x -= HERO_VEL;
	if (curX < destX) cPositions[index].x += HERO_VEL;
	if (curY > destY) cPositions[index].y -= HERO_VEL;
	if (curY < destY) cPositions[index].y += HERO_VEL;

	if (curY == destY && curX == destX) return true;
	else return false;
}

void Game::animationSystem()
{
	for (int i = 0; i < MAX_ENTITIES; ++i)
	{
		if ((cMasks[i] & COMPONENT_SPRITE) == COMPONENT_SPRITE)
		{
			cSprites[i].animate(cPositions[i].x - camera.x,
				cPositions[i].y - camera.y);
		}
		if ((cMasks[i] & COMPONENT_MINIMAP) == COMPONENT_MINIMAP)
		{
			//render to minimap
		}
	}

	if (!doneSelecting)
	{
		for (int i = clickStart.x; i <= mouseCoordinate.x; ++i)
		{
			for (int j = clickStart.y; j <= mouseCoordinate.y; ++j)
			{
				selectSprite.animate(i * TILE_WIDTH - camera.x, 
					j * TILE_WIDTH - camera.y);
			}
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
			if (findEntity(j * LEVEL_WIDTH + i, HERO))
			{
				selected.push_back(entityMap[j * LEVEL_WIDTH + i].index);
			}
		}
	}
}

void Game::addToOpen(TileQueue* openNodes, int currentIndex, int index)
{
	if (entityMap[index].terrain != WATER && index > 0 && index < MAX_TILES - 1)
	{
		SDL_Point current;
		current.x = currentIndex % LEVEL_WIDTH;
		current.y = currentIndex / LEVEL_WIDTH;

		SDL_Point end;
		end.x = index % LEVEL_WIDTH;
		end.y = index / LEVEL_WIDTH;

		int cost = pathMap[currentIndex].exactCost + diagDist(current, end);
		if (pathMap[index].isInOpen && cost < pathMap[index].exactCost)
		{
			pathMap[index].isInOpen = false;
		}
		if (pathMap[index].isInClosed && cost < pathMap[index].exactCost)
		{
			pathMap[index].isInClosed = false;
		}
		if (!pathMap[index].isInOpen && !pathMap[index].isInClosed)
		{
			pathMap[index].exactCost = cost;
			pathMap[index].isInOpen = true;
			pathMap[index].parentIndex = currentIndex;
			openNodes->push(&pathMap[index]);
		}
	}
}

void Game::calclulatePath(int startIndex, int heroIndex)
{
	for (int i = 0; i < MAX_TILES; ++i)
	{
		pathMap[i].index = i;
		pathMap[i].parentIndex = 0;
		pathMap[i].isInOpen = false;
		pathMap[i].isInClosed = false;
		pathMap[i].exactCost = 0;
		pathMap[i].estimatedCost = 0;
	}

	TileQueue openNodes;
	int destination = cDestinations[heroIndex].y * LEVEL_WIDTH
					+ cDestinations[heroIndex].x;

	pathMap[startIndex].isInOpen = true;
	openNodes.push(&pathMap[startIndex]);

	while (!openNodes.empty() && openNodes.top()->index != destination)
	{
		while (openNodes.top()->isInOpen == false) openNodes.pop();

		searchTile* current = openNodes.top();
		int currentIndex = current->index;
		current->isInOpen = false;
		current->isInClosed = true;
		openNodes.pop();

		addToOpen(&openNodes, currentIndex, currentIndex + 1);
		addToOpen(&openNodes, currentIndex, currentIndex - 1);
		addToOpen(&openNodes, currentIndex, currentIndex + LEVEL_HEIGHT);
		addToOpen(&openNodes, currentIndex, currentIndex - LEVEL_HEIGHT);
		addToOpen(&openNodes, currentIndex, currentIndex + LEVEL_HEIGHT + 1);
		addToOpen(&openNodes, currentIndex, currentIndex - LEVEL_HEIGHT - 1);
		addToOpen(&openNodes, currentIndex, currentIndex + LEVEL_HEIGHT - 1);
		addToOpen(&openNodes, currentIndex, currentIndex - LEVEL_HEIGHT + 1);
	}

	int tempIndex = openNodes.top()->index;
	SDL_Point tempPoint;

	while (tempIndex != startIndex)
	{
		tempPoint.x = tempIndex % LEVEL_WIDTH;
		tempPoint.y = tempIndex / LEVEL_WIDTH;

		cPaths[heroIndex].push(tempPoint);
		tempIndex = pathMap[tempIndex].parentIndex;
	}

}

void Game::assignDestinations(int x, int y)
{
	for (int i = 0; i < selected.size(); ++i)
	{
		for (int i = 0; i < LEVEL_WIDTH - x; ++i)
		{
			if (!collisionChecker(selected[i], x, y))
			{
				setDestination(selected[i], x, y);
				break;
			}
			else if (!collisionChecker(selected[i], x - i, y))
			{
				setDestination(selected[i], x - i, y);
				break;
			}
			else if (!collisionChecker(selected[i], x + i, y))
			{
				setDestination(selected[i], x + i, y);
				break;
			}
			else if (!collisionChecker(selected[i], x, y - i))
			{
				setDestination(selected[i], x, y - i);
				break;
			}
			else if (!collisionChecker(selected[i], x, y + i))
			{
				setDestination(selected[i], x, y + i);
				break;
			}
			else if (!collisionChecker(selected[i], x + i, y + i))
			{
				setDestination(selected[i], x + i, y + i);
				break;
			}
			else if (!collisionChecker(selected[i], x - i, y - i))
			{
				setDestination(selected[i], x - i, y - i);
				break;
			}
		}

		int startIndex = cCoordinates[selected[i]].y * LEVEL_WIDTH + 
						 cCoordinates[selected[i]].x;
		calclulatePath(startIndex, selected[i]);
	}
}

void Game::updateDestination(int index, int x, int y)
{
	for (int i = 0; i < LEVEL_WIDTH - x; ++i)
	{
		if (!collisionChecker(index, x, y))
		{
			setDestination(index, x, y);
			break;
		}
		else if (!collisionChecker(index, x - i, y))
		{
			setDestination(index, x - i, y);
			break;
		}
		else if (!collisionChecker(index, x + i, y))
		{
			setDestination(index, x + i, y);
			break;
		}
		else if (!collisionChecker(index, x, y - i))
		{
			setDestination(index, x, y - i);
			break;
		}
		else if (!collisionChecker(index, x, y + i))
		{
			setDestination(index, x, y + i);
			break;
		}
		else if (!collisionChecker(index, x + i, y + i))
		{
			setDestination(index, x + i, y + i);
			break;
		}
		else if (!collisionChecker(index, x - i, y - i))
		{
			setDestination(index, x - i, y - i);
			break;
		}
	}

	int startIndex = cCoordinates[index].y * LEVEL_WIDTH + 
					 cCoordinates[index].x;
	calclulatePath(startIndex, index);
}

void Game::setDestination(int index, int x, int y)
{
	cDestinations[index].x = x;
	cDestinations[index].y = y;
}

bool Game::collisionChecker(int index, int x, int y)
{
	if (y < 0 || y >= LEVEL_HEIGHT) return true;
	if (x < 0 || x >= LEVEL_WIDTH) return true;

	int mapIndex = y * LEVEL_WIDTH + x;
	entityType type = cTypes[index];
	
	if (entityMap[mapIndex].index == index) return false;
	if (findEntity(mapIndex, type) || entityMap[mapIndex].terrain == WATER)
	{
		return true;
	} 
	else return false;
}

void Game::cutTrees(int x, int y)
{
	/*
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
	*/
}
