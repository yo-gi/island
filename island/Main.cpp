#include <SDL.h>
#include <SDL_image.h>

#include <iostream>

#include "Game.h"

using namespace std;

SDL_Window* mainWindow;
SDL_Renderer* mainRenderer;
SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

int main (int argc, char* args[]) {

	srand(time(0));

	Game game;
	if (!game.initialize())
	{
		cout << "Game couldn't initialize\n";
		return 1;
	}

	if (!game.loadMedia())
	{
		cout << "Game couldn't load media\n";
		return 1;
	}
	SDL_SetRenderTarget(mainRenderer, NULL);


	/*
	if (!game.loadSector(1))
	{
		cout << "Game couldn't load sector 1\n";
		return 1;
	}
	*/
	//game.loadSector(1);
	game.loadMap();
	
	game.createTree(2,3);
	game.createTree(5,6);
	game.createTree(2,12);
	game.createTree(5,9);
	game.createTree(8,3);
	game.createTree(13,5);
	game.createTree(12,7);
	game.createTree(9,13);

	game.createHero(3, 3);
	game.createHero(3, 4);

	bool quit = false;
	SDL_Event event;

	while (!quit)
	{
		while(SDL_PollEvent(&event) != 0)
		{
			switch(event.type)
			{
				case SDL_QUIT:
					quit = true;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							quit = true;
							break;
					}
			}
			game.eventHandler(event);
		}

		game.updateCamera();

		SDL_RenderClear(mainRenderer);
		game.displayBackground();
		game.animationSystem();
		game.movementSystem();
		game.displayMinimap();

		SDL_RenderPresent(mainRenderer);

	}

	game.destruct();

	return 0;
}


bool checkCollision(SDL_Rect one, SDL_Rect two)
{
	int leftOne, leftTwo;
	int rightOne, rightTwo;
	int topOne, topTwo;
	int bottomOne, bottomTwo;

	leftOne = one.x;
	rightOne = one.x + one.w;
	topOne = one.y;
	bottomOne = one.y + one.h;

	leftTwo = two.w;
	rightTwo = two.x + two.w;
	topTwo = two.y;
	bottomTwo = two.y + two.h;

	if (bottomOne <= topTwo) return false;
	if (topOne >= bottomTwo) return false;
	if (rightOne <= leftTwo) return false;
	if (leftOne >= rightTwo) return false;

	return true;
}

int manDist(SDL_Point one, SDL_Point two)
{
	return abs(one.x - two.x) + abs(one.y - two.y);
}

int diagDist(SDL_Point one, SDL_Point two)
{
	int dx = abs(one.x - two.x);
	int dy = abs(one.y - two.y);

	return 1 * max(dx, dy);
}