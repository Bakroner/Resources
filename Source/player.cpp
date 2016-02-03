#include "player.h"

const int JOYSTICK_DEAD_ZONE = 8000;

Player::Player(SDL_Renderer*renderer, int pNum, string filePath, float x, float y)
{
	playerNum = pNum;
	speed = 500.0f;

	if(playerNum == 0){
		playerPath = filePath + "Player1.png";
	}else{
		playerPath = filePath + "Player2.png";
	}
	surface = IMG_Load(playerPath.c_str());
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	posRect.x = x;
	posRect.y = y;
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = w;
	posRect.h = h;
	pos_X = x;
	pos_Y = y;
	xDir = x;
	yDir = y;
}

void Player::Update(float deltaTime)
{
	pos_X += (speed * xDir) * deltaTime;
	pos_Y += (speed * yDir) * deltaTime;


	posRect.x = (int)(pos_X + 0.5f);
	posRect.y = (int)(pos_Y + 0.5f);

	// off the screen in X
	if(posRect.x < 0){
		posRect.x = 0;
		pos_X = posRect.x;
	}

	if(posRect.x > 1024 - posRect.w){
		posRect.x = 1024 - posRect.w;
		pos_X = posRect.x;
	}

	// off the screen in Y
	if(posRect.y < 0){
		posRect.y = 0;
		pos_Y = posRect.y;
	}

	if(posRect.y > 768 - posRect.h){
		posRect.y = 768 - posRect.h;
		pos_Y = posRect.y;
	}
}

void Player::Draw(SDL_Renderer*renderer)
{
	SDL_RenderCopy(renderer, texture, NULL, &posRect);
}

void Player::OnControllerButton(const SDL_ControllerButtonEvent event)
{
	if(event.which == 0 && playerNum == 0)
	{
		if(event.button == 0)
		{
			cout << "Player 1 - Button A" << endl;
		}
	}
	if(event.which == 1 && playerNum == 1)
	{
		if(event.button == 1)
		{
			cout << "Player 2 - Button A" << endl;
		}
	}
}

void Player::OnControllerAxis(const SDL_ControllerAxisEvent event)
{
	if(event.which == 0 && playerNum == 0)
	{
		if(event.axis == 0)
		{
			if(event.value < -JOYSTICK_DEAD_ZONE)
			{
				xDir = -1.0f;

			}
			else if (event.value > JOYSTICK_DEAD_ZONE)
			{
				xDir = 1.0f;
			}
			else
			{
				xDir = 0.0f;
			}
		}

		if(event.axis == 1)
		{
			if(event.value < -JOYSTICK_DEAD_ZONE)
			{
				yDir = -1.0f;

			}
			else if (event.value > JOYSTICK_DEAD_ZONE)
			{
				yDir = 1.0f;
			}
			else
			{
				yDir = 0.0f;
			}
		}
	}
}









