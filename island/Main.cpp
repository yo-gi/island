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

	game.createHero();

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

		game.updateCamera(camera);


		SDL_RenderClear(mainRenderer);
		game.displayBackground();
		game.animationSystem();
		game.movementSystem();
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