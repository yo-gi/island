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

	for (int i = 0; i < MAX_TILE_TYPES; ++i)
	{
		tileClips[i].w = TILE_WIDTH;
		tileClips[i].h = TILE_WIDTH;
	}

	camera.x = 0;
	camera.y = 0;

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

	mainWindow = SDL_CreateWindow("r/trees", 
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

	return true;
}

bool Game::loadMedia()
{
	if (!tileTexture.loadFrom("images/texture.png"))
	{
		cout << "Couldn't load tile texture\n";
		return false;
	}

	return true;
}

bool Game::loadSector(int sectorNumber)
{
	string path;
	path.append("sectors/sector_");
	path.append(to_string(sectorNumber));
	path.append(".txt");

	ifstream sectorFile(path);

	if (sector == NULL)
	{
		cout << "Couldn't load sector file at " << path << endl;
		return false;
	}
	else
	{
		int x, y, type;

		for (int i = 0; i < MAX_TILES; ++i)
		{
			x = (i % LEVEL_WIDTH) * TILE_WIDTH;
			y = (i / LEVEL_WIDTH) * TILE_WIDTH;
			sectorFile >> type;

			if (sectorFile.fail())
			{
				cout << "Error loading sector file at " << path << endl;
				return false;
			}

			sector[i] = new Tile(x, y, type);
		}
	}

	sectorFile.close();
	return true;
}

void Game::loadRandomSector()
{
	int x, y, type;

	for (int i = 0; i < MAX_TILES; ++i)
	{
		x = (i % LEVEL_WIDTH) * TILE_WIDTH;
		y = (i / LEVEL_WIDTH) * TILE_WIDTH;
		type = rand() % 2;

		sector[i] = new Tile(x, y, type);
	}
}

void Game::loadTileClips()
{
	tileClips[TILE_WATER].x = TILE_WIDTH;
	tileClips[TILE_WATER].y = 0;

	tileClips[TILE_SAND].x = 0;
	tileClips[TILE_SAND].y = 0;
}

void Game::reloadBackground()
{
	/*
	if (!background.loadRandomColor())
	{
		cout << "Couldn't load background texture\n";
	}
	*/
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
	for (int i = 0; i < MAX_TILES; ++i)
	{
		if (checkCollision(camera, sector[i]->getBox()))
		{
			sector[i]->render(&tileTexture, &tileClips[sector[i]->getType()]);
		}
	}
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
	componentMasks[i] = COMPONENT_NONE;
}

void Game::eventHandler(SDL_Event& event)
{
	if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
	{
		switch(event.key.keysym.sym)
		{
			/*case SDLK_UP:
				cameraVelY -= CAMERA_VEL;
				break;
			case SDLK_DOWN:
				cameraVelY += CAMERA_VEL;
				break;
			case SDLK_LEFT:
				cameraVelX -= CAMERA_VEL;
				break;
			case SDLK_RIGHT:
				cameraVelX += CAMERA_VEL;
				break;*/
			case SDLK_w:
				/*if(componentCoordinates[heroNum].y == 8)
				{
					cameraVelY -= CAMERA_VEL;
				}*/
				if(!collisionChecker(componentCoordinates[heroNum].x,
				                 componentCoordinates[heroNum].y - HERO_VEL))
				{
					componentCoordinates[heroNum].y -= HERO_VEL;
				}
				break;
			case SDLK_s:
				/*if(componentCoordinates[heroNum].y == 7)
				{
					cameraVelY += CAMERA_VEL;
				}*/
				if(!collisionChecker(componentCoordinates[heroNum].x,
				                 componentCoordinates[heroNum].y + HERO_VEL))
				{
					componentCoordinates[heroNum].y += HERO_VEL;
				}
				break;
			case SDLK_a:
				/*if(componentCoordinates[heroNum].x == 8)
				{
					cameraVelX -= CAMERA_VEL;
				}*/
				if(!collisionChecker(componentCoordinates[heroNum].x - HERO_VEL,
				                 componentCoordinates[heroNum].y))
				{
					componentCoordinates[heroNum].x -= HERO_VEL;
				}
				break;
			case SDLK_d:
				/*if(componentCoordinates[heroNum].x == 7)
				{
					cameraVelX += CAMERA_VEL;
				}*/
				if(!collisionChecker(componentCoordinates[heroNum].x + HERO_VEL,
				                 componentCoordinates[heroNum].y))
				{
					componentCoordinates[heroNum].x += HERO_VEL;
				}
				break;
			case SDLK_c:
				centerCamera(heroNum, camera);
				break;
			}
	}
	else if (event.type == SDL_KEYUP && event.key.repeat == 0)
	{
		switch(event.key.keysym.sym)
		{
			/*case SDLK_w:
				if(componentCoordinates[heroNum].y == 7)
				{
					cameraVelY += CAMERA_VEL;
				}
				break;
			case SDLK_s:
				if(componentCoordinates[heroNum].y == 8)
				{
					cameraVelY -= CAMERA_VEL;
				}
				break;
			case SDLK_a:
				if(componentCoordinates[heroNum].x == 7)
				{
					cameraVelX += CAMERA_VEL;
				}
				break;
			case SDLK_d:
				if(componentCoordinates[heroNum].x == 8)
				{
					cameraVelX -= CAMERA_VEL;
				}
				break;*/
		}
	}

	if (componentCoordinates[heroNum].x < 0) componentCoordinates[heroNum].x += HERO_VEL;
	if (componentCoordinates[heroNum].x > LEVEL_WIDTH - 1) componentCoordinates[heroNum].x -= HERO_VEL;
	if (componentCoordinates[heroNum].y < 0) componentCoordinates[heroNum].y += HERO_VEL;
	if (componentCoordinates[heroNum].y > LEVEL_HEIGHT - 1) componentCoordinates[heroNum].y -= HERO_VEL;

	cout << componentCoordinates[heroNum].x << ", " << componentCoordinates[heroNum].y << endl;
}

void Game::updateCamera(SDL_Rect& camera)
{
	camera.x += cameraVelX;
	camera.y += cameraVelY;

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

void Game::centerCamera(int componentIndex, SDL_Rect& camera)
{
	camera.x = componentCoordinates[componentIndex].x*TILE_WIDTH
	+ TILE_WIDTH/2 - SCREEN_WIDTH/2;
	camera.y = componentCoordinates[componentIndex].y*TILE_WIDTH
	+ TILE_WIDTH/2 - SCREEN_HEIGHT/2;

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

void Game::createHero()
{
	heroNum = createEntity();
	componentMasks[heroNum] = 
		COMPONENT_VELOCITY | COMPONENT_SPRITE | COMPONENT_POSITION;

	componentPositions[heroNum].x = TILE_WIDTH/4;
	componentPositions[heroNum].y = TILE_WIDTH/4;

	componentCoordinates[heroNum].x = 0;
	componentCoordinates[heroNum].y = 0;

	componentVelocities[heroNum].x = 0;
	componentVelocities[heroNum].y = 0;
	
	componentSprites[heroNum].initialize("images/hero.png", 
		32, 32, 1, 1);
}

void Game::createTree(int x, int y)
{
	treeNums.push_back(createEntity());
	componentMasks[treeNums.back()] = 
		COMPONENT_VELOCITY | COMPONENT_SPRITE | COMPONENT_POSITION;

	componentPositions[treeNums.back()].x = TILE_WIDTH/4;
	componentPositions[treeNums.back()].y = TILE_WIDTH/4;

	componentCoordinates[treeNums.back()].x = x;
	componentCoordinates[treeNums.back()].y = y;

	componentVelocities[treeNums.back()].x = 0;
	componentVelocities[treeNums.back()].y = 0;
	
	componentSprites[treeNums.back()].initialize("images/tree.png", 
		32, 32, 1, 1);
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
}

bool Game::collisionChecker(int x, int y)
{
	for (int i = 0; i < MAX_ENTITIES; ++i)
	{
		if (componentCoordinates[i].x == x && componentCoordinates[i].y == y)
		{
			return true;
		}
	}
	return false;
}
