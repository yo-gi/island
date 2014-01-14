#include "Texture.h"

using namespace std;
Texture::Texture()
{
	texture = NULL;
	width = 0;
	height = 0;
}

Texture::~Texture()
{
	destroy();
}

bool Texture::loadFrom(string path)
{
	destroy();
	SDL_Texture* newTexture = NULL;
	SDL_Surface* tempSurface = IMG_Load(path.c_str());

	if (tempSurface == NULL) 
	{
		cout << "Can't load image at " << path << "\n";
		return false;
	}
	else 
	{
		SDL_SetColorKey(tempSurface, SDL_TRUE, 
			SDL_MapRGB(tempSurface->format, 0x00, 0xFF, 0xFF));

		newTexture = SDL_CreateTextureFromSurface(mainRenderer, tempSurface);
		if (newTexture == NULL) 
		{
			cout << "Can't create texture at " << path << "\n";
			return false;
		}
		else 
		{
			width = tempSurface->w;
			height = tempSurface->h;
		}

		SDL_FreeSurface(tempSurface);
		texture = newTexture;
		return true;
	}
}

bool Texture::loadBlank(int w, int h)
{
	destroy();
	texture = SDL_CreateTexture(mainRenderer, SDL_PIXELFORMAT_RGBA8888, 
		SDL_TEXTUREACCESS_STREAMING, w, h);
	if (texture == NULL)
	{
		cout << "Couldn't create blank texture\n";
		return false;
	}

	width = w;
	height = h;
	return true;
}

bool Texture::loadRandomColor()
{
	destroy();
	int red = ((rand() % 256) + 2*0xFF)/3;
	int green = ((rand() % 256) + 2*0xFF)/3;
	int blue = ((rand() % 256) + 2*0xFF)/3;

	SDL_Texture* newTexture = NULL;
	SDL_Surface* tempSurface = IMG_Load("images/blank.png");

	SDL_FillRect(tempSurface, NULL, SDL_MapRGB(tempSurface->format, red,
		green, blue));

	if (tempSurface == NULL) 
	{
		cout << "Can't create randomly colored texture\n";
		return false;
	}
	else 
	{
		newTexture = SDL_CreateTextureFromSurface(mainRenderer, tempSurface);
		if (newTexture == NULL) 
		{
			cout << "Can't create randomly colored texture\n";
			return false;
		}
		else 
		{
			width = tempSurface->w;
			height = tempSurface->h;
		}

		SDL_FreeSurface(tempSurface);
		texture = newTexture;
		//SDL_SetTextureAlphaMod(texture, 127);
		return true;
	}

	return true;
}

void Texture::destroy()
{
	if (texture != NULL)
	{
		SDL_DestroyTexture(texture);
		texture = NULL;
		width = 0;
		height = 0;
	}
}

void Texture::render(int x, int y, SDL_Rect* srcRect)
{
	SDL_Rect destRect = {x, y, width, height};
	if (srcRect != NULL)
	{
		destRect.w = srcRect->w;
		destRect.h = srcRect->h;
	}
	SDL_RenderCopy(mainRenderer, texture, srcRect, &destRect);
}

void Texture::setAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(texture, alpha);
}

void Texture::setSelfAsRenderTarget()
{
	SDL_SetRenderTarget(mainRenderer, texture);
}

int Texture::getWidth()
{
	return width;
}

int Texture::getHeight()
{
	return height;
}