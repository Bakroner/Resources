#include "player.h"

const int JOYSTICK_DEAD_ZONE = 8000;

Player::Player(SDL_Renderer*renderer, int pNum, string filePath, float x, float y)
{
	playerNum = pNum;

	speed = 500.0f;

	if(playerNum == 0){

		playerPath = filePath + "playerOne.png";
	}else{

		playerPath = filePath + "playerTwo.png";
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


	cursorPos.x = (int)(pos_X + 0.5f);
	cursorPos.y = (int)(pos_X + 0.5f);

	//update active position of cursor - collison box
	activePos.x = cursorPos.x;
	activePos.y = cursorPos.y;

	// off the screen in Y
	if(cursorPos.x < 0){
		cursorPos.x = 0;
		pos_X = cursorPos.x;
	}

	if(cursorPos.x > 1024 - cursorPos.w){
		cursorPos.x = 1024 - cursorPos.w;
		pos_X = cursorPos.x;
	}

	// off the screen in Y
	if(cursorPos.y < 0){
		cursorPos.y = 0;
		pos_Y = cursorPos.y;
	}

	if(cursorPos.y > 768 - cursorPos.h){
		cursorPos.y = 768 - cursorPos.h;
		pos_Y = cursorPos.y;

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









