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
	xDir = 0;
	yDir = 0;

	string bulletPath;
	if(playerNum == 0){
		bulletPath = filePath + "Bullet 1.png";
	}else{
		bulletPath = filePath + "Bullet 2.png";
	}

	for (int i = 0; i < 10; i++)
	{
		Bullet tmpBullet(renderer, bulletPath, -1000, -1000);
		bulletlist.push_back(tmpBullet);
	}
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
	for (int i = 0; i < bulletlist.size(); i++)
	{
		if(bulletlist[i].active){

			bulletlist[i].Update(deltaTime);
		}
	}
}

void Player::Draw(SDL_Renderer*renderer)
{
	SDL_RenderCopy(renderer, texture, NULL, &posRect);

	for (int i = 0; i < bulletlist.size(); i++)
	{
		if(bulletlist[i].active){

			bulletlist[i].Draw(renderer);
		}
	}
}

Player::~Player()
{
	SDL_DestroyTexture(texture);
}

void Player::CreateBullet(){
	for (int i = 0; i < bulletlist.size(); i++)
	{
		if(bulletlist[i].active == false){

			bulletlist[i].active = true;

			bulletlist[i].posRect.x = (pos_X + (posRect.w/2));

			bulletlist[i].posRect.x = (bulletlist[i].posRect.x - (bulletlist[i].posRect.w/2));
			bulletlist[i].posRect.y = posRect.y;

			bulletlist[i].pos_X = pos_X;
			bulletlist[i].pos_Y = pos_Y;

			break;
		}
	}
}

void Player::OnControllerButton(const SDL_ControllerButtonEvent event)
{
	if(event.which == 0 && playerNum == 0)
	{
		if(event.button == 0)
		{
			cout << "Player 1 - Button A" << endl;

			CreateBullet();
		}
	}
	if(event.which == 1 && playerNum == 1)
	{
		if(event.button == 1)
		{
			cout << "Player 2 - Button A" << endl;

			CreateBullet();
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

	if(event.which == 1 && playerNum == 1)
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










