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
	cMinimapSprites.resize(MAX_ENTITIES);
	cTimers.resize(MAX_ENTITIES);
	cDescriptions.resize(MAX_ENTITIES);

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
	selected = NULL;

	descriptionTextSprite.setSize(DESCRIPTION_WIDTH-20, 20);

	currentTime = SDL_GetTicks();
	timePassing = true;
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

	if( TTF_Init() == -1 )
	{
		printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
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

	if (!minimapSprite.initialize("images/minimap.png",
	    MINIMAP_WIDTH, MINIMAP_HEIGHT, 1, 1))
	{
		cout << "Couldn't load minimap texture\n";
		return false;
	}
	if (!descriptionSprite.initialize("images/description.png",
	    DESCRIPTION_WIDTH, DESCRIPTION_HEIGHT, 1, 1))
	{
		cout << "Couldn't load description texture\n";
		return false;
	}
	if (!descriptionTextSprite.initialize("images/description.png",
	    DESCRIPTION_WIDTH-20, DESCRIPTION_HEIGHT/2, 1, 1))
	{
		cout << "Couldn't load description texture\n";
		return false;
	}
	lazy28 = TTF_OpenFont( "Fonts/lazy.ttf", 28 );
	if (lazy28 == NULL)
	{
		cout << "Couldn't load lazy font texture\n";
		//return false;
	}
	return true;
}

void Game::destruct()
{
	SDL_DestroyWindow(mainWindow);
	SDL_DestroyRenderer(mainRenderer);
	mainWindow = NULL;
	mainRenderer = NULL;
	lazy28=NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Game::displayBackground()
{
	map.renderMap();
}

void Game::displayMinimap()
{
	minimapSprite.animate(SCREEN_WIDTH - MINIMAP_WIDTH, 0);
	for (int i = 0; i < MAX_ENTITIES; ++i)
	{
		if ((cMasks[i] & COMPONENT_MINIMAP) == COMPONENT_MINIMAP)
		{
			//render to minimap
			int xposition = cPositions[i].x*MINIMAP_WIDTH
			/(LEVEL_WIDTH*TILE_WIDTH);
			int yposition = cPositions[i].y*MINIMAP_HEIGHT
			/(LEVEL_HEIGHT*TILE_WIDTH);
			cMinimapSprites[i].animate
			(xposition + SCREEN_WIDTH - MINIMAP_WIDTH, yposition);
		}
	}
}

void Game::displayDescription()
{
	descriptionSprite.animate(SCREEN_WIDTH - DESCRIPTION_WIDTH, MINIMAP_HEIGHT);
	if(selected != NULL)
	{
		//call describe function
		descriptionTextSprite.animate(SCREEN_WIDTH - DESCRIPTION_WIDTH,
		                              MINIMAP_HEIGHT+DESCRIPTION_OFFSET);
	}
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

		entityMap[i].treeIndex = -1;
		entityMap[i].heroIndex = -1;
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
				if(selected != NULL)
				{
					centerCamera(selected);
				}
				break;
			case SDLK_c:
				for(int i = 0; i < int(heroNums.size()); ++i)
				{
					cTimers[heroNums[i]] = SDL_GetTicks() + 1000;
				}
				break;
		}
	}	

	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			selected = NULL;

			doneSelecting = false;
			clickStart.x = mouseCoordinate.x;
			clickStart.y = mouseCoordinate.y;

			cout << "start: " << clickStart.x << ", " << clickStart.y << endl;
		}

		if (event.button.button == SDL_BUTTON_RIGHT && selected != NULL
		    && cTypes[selected]==HERO)
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
			if(selected != NULL)
			{
				cout << cDescriptions[selected] << endl;
				descriptionTextSprite.textRender(cDescriptions[selected], lazy28, 0,0,0);
			}
			else
			{
				descriptionTextSprite.setSize(DESCRIPTION_WIDTH-20, 20);
			}
		}
	}
	if(timePassing)
	{
		for(int i = 0; i < MAX_ENTITIES; ++i)
		{
			if(SDL_GetTicks() == cTimers[i])
			{
				cutTrees(cCoordinates[heroNums[i]].x,
				cCoordinates[heroNums[i]].y);
			}
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
	entityMap[mapIndex].heroIndex = -1;	

	cCoordinates[index].x = x;
	cCoordinates[index].y = y;
	cPositions[index].x = TILE_WIDTH/4 + x*TILE_WIDTH;
	cPositions[index].y = TILE_WIDTH/4 + y*TILE_WIDTH;

	mapIndex = cCoordinates[index].y * LEVEL_WIDTH
		+ cCoordinates[index].x;

	entityMap[mapIndex].types.insert(tempType);
	entityMap[mapIndex].heroIndex = index;
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

	cMinimapSprites[heroNum].initialize
	("images/minimap_hero.png", 16, 16, 1, 1);

	cDescriptions[heroNum] = "This is a hero!";

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

	cDescriptions[treeNum] = "This is a lowly plant";

	entityMap[y * LEVEL_WIDTH + x].types.insert(TREE);
	entityMap[y * LEVEL_WIDTH + x].treeIndex = treeNum;

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
	selected = NULL;
	for (int i = clickStart.x; i <= clickEnd.x; ++i)
	{
		for (int j = clickStart.y; j <= clickEnd.y; ++j)
		{
			if (findEntity(j * LEVEL_WIDTH + i, HERO))
			{
				selected = entityMap[j * LEVEL_WIDTH + i].heroIndex;
			}
			if (findEntity(j * LEVEL_WIDTH + i, TREE))
			{
				selected = entityMap[j * LEVEL_WIDTH + i].treeIndex;
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
		for (int i = 0; i < LEVEL_WIDTH - x; ++i)
		{
			if (!collisionChecker(selected, x, y))
			{
				setDestination(selected, x, y);
				break;
			}
			else if (!collisionChecker(selected, x - i, y))
			{
				setDestination(selected, x - i, y);
				break;
			}
			else if (!collisionChecker(selected, x + i, y))
			{
				setDestination(selected, x + i, y);
				break;
			}
			else if (!collisionChecker(selected, x, y - i))
			{
				setDestination(selected, x, y - i);
				break;
			}
			else if (!collisionChecker(selected, x, y + i))
			{
				setDestination(selected, x, y + i);
				break;
			}
			else if (!collisionChecker(selected, x + i, y + i))
			{
				setDestination(selected, x + i, y + i);
				break;
			}
			else if (!collisionChecker(selected, x - i, y - i))
			{
				setDestination(selected, x - i, y - i);
				break;
			}
		}

		int startIndex = cCoordinates[selected].y * LEVEL_WIDTH + 
						 cCoordinates[selected].x;
		calclulatePath(startIndex, selected);
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
	
	if (entityMap[mapIndex].heroIndex == index) return false;
	if (findEntity(mapIndex, type) || entityMap[mapIndex].terrain == WATER)
	{
		return true;
	} 
	else return false;
}

void Game::cutTrees(int x, int y)
{
	cout << "-------" << endl;
	cout << "1" << endl;
	int i = y*LEVEL_WIDTH+x+1;
	if(i < MAX_TILES)
	{
		if(entityMap[i].types.find(TREE) != entityMap[i].types.end())
		{
			destroyEntity(entityMap[i].treeIndex);
		}
	}
	cout << "1" << endl;
	i = (y+1)*LEVEL_WIDTH+x+1;
	if(i < MAX_TILES)
	{
		if(entityMap[i].types.find(TREE) != entityMap[i].types.end())
		{
			destroyEntity(entityMap[i].treeIndex);
		}
	}
	cout << "1" << endl;
	i = (y-1)*LEVEL_WIDTH+x+1;
	if(i < MAX_TILES)
	{
		if(entityMap[i].types.find(TREE) != entityMap[i].types.end())
		{
			destroyEntity(entityMap[i].treeIndex);
		}
	}
	cout << "1" << endl;
	i = y*LEVEL_WIDTH+x-1;
	if(i < MAX_TILES)
	{
		if(entityMap[i].types.find(TREE) != entityMap[i].types.end())
		{
			destroyEntity(entityMap[i].treeIndex);
		}
	}
	cout << "1" << endl;
	i = (y+1)*LEVEL_WIDTH+x-1;
	if(i < MAX_TILES)
	{
		if(entityMap[i].types.find(TREE) != entityMap[i].types.end())
		{
			destroyEntity(entityMap[i].treeIndex);
		}
	}
	cout << "1" << endl;
	i = (y-1)*LEVEL_WIDTH+x-1;
	if(i < MAX_TILES)
	{
		if(entityMap[i].types.find(TREE) != entityMap[i].types.end())
		{
			destroyEntity(entityMap[i].treeIndex);
		}
	}
	cout << "1" << endl;
	i = (y+1)*LEVEL_WIDTH+x;
	if(i < MAX_TILES)
	{
		if(entityMap[i].types.find(TREE) != entityMap[i].types.end())
		{
			destroyEntity(entityMap[i].treeIndex);
		}
	}
	cout << "1" << endl;
	i = (y-1)*LEVEL_WIDTH+x;
	if(i < MAX_TILES)
	{
		if(entityMap[i].types.find(TREE) != entityMap[i].types.end())
		{
			destroyEntity(entityMap[i].treeIndex);
		}
	}
	cout << "done" << endl;
}
