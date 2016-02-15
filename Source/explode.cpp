#include "explode.h"

Explode::Explode(SDL_Renderer *renderer, string filePath, float x, float y)
{
	active = false;
	string explodePath = filePath + "explosion.png";
	frameCounter = 0.0f;
	SDL_Surface *surface = IMG_Load(explodePath.c_str());
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	textureWidth = w;
	textureHeight = h;
	posRect.w = 128;
	posRect.h = 128;
	posRect.x = x;
	posRect.y = y;
	frameWidth = 128;
	frameHeight = 128;
	drawRect.x = 128;
	drawRect.y = 0;
	drawRect.w = frameWidth;
	drawRect.h = frameHeight;

}

	void Explode::Reset() {

		active = false;
	}

	void Explode::Update(float deltaTime)
	{
		if (active) {

			frameCounter += deltaTime;

			if (frameCounter >= .035f) {

				frameCounter = 0.0f;

				drawRect.x += frameWidth;

				if (drawRect.x >= textureWidth)
				{
					drawRect.x = 0;
					active = false;
					posRect.x = -1000;
					posRect.y = -1000;
				}
			}
		}

	}

	void Explode::Draw(SDL_Renderer *renderer) {

		SDL_RenderCopy(renderer, texture, &drawRect, &posRect);

	}

	Explode::~Explode()
	{
		//SDL_DestroyTexture(texture);
	}
