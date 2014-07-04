#pragma once

#include "Main.h"

class Texture
{
public:
	Texture();
	~Texture();

	bool loadFrom(std::string path);
	bool loadBlank(int w, int h);
	bool loadRandomColor();
	void destroy();
	void render(int x, int y, SDL_Rect* srcRect);
	void setAlpha(Uint8 alpha);
	void setSelfAsRenderTarget();
	bool textRender(std::string text, TTF_Font* font, int r, int g, int b);

	int getWidth();
	int getHeight();

private:
	SDL_Texture* texture;
	int width;
	int height;
};

