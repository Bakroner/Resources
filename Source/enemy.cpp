#include "enemy.h"

Enemy::Enemy(SDL_Renderer *renderer, string filePath)
{
	string enemyPath = filePath + "enemy.png";

	SDL_Surface *surface = IMG_Load(enemyPath.c_str());

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = w;
	posRect.h = h;

	Reset();

	xDir = 0;
	yDir = 1;

	angle = rand() % (360);


}
