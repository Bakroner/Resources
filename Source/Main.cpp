
#if defined(_WIN32) || (_WIN64)

#include "SDL.h"
#include "SDL_image.h"

#endif

#if defined(__APPLE__)

#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"

#endif

#if defined(__linux__)

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#endif

#if defined(_WIN32) || (_WIN64)
#include <direct.h>
#define getcwd _getcwd
#endif

#if defined(__linux__)
#include <unistd.h>
#endif

#include <stdio.h>
#include <iostream>
using namespace std;

// code for frame rate independence
float deltaTime = 0.0;
int thisTime = 0;
int lastTime = 0;

// create the SDL_Rectangle for the texture's position and size -x,y,w,h
SDL_Rect bkgd1Pos;

// create the SDL_Rectangle for the texture's position and size -x,y,w,h
SDL_Rect bkgd2Pos;

// set speed for background
int bkgdSpeed = 100;

// set temp variables to hold movement - background 1
float BG1pos_X = 0,BG1pos_Y = 0;

// set temp variables to hold movement - background 2
float BG2pos_X = 0,BG2pos_Y = -768;

// move the background
void UpdateBackground(float deltaTime){
	// Update Background 1
	BG1pos_Y += (bkgdSpeed * 1) * deltaTime;

	// set the new bkgd1 position
	bkgd1Pos.y = (int)(BG1pos_Y + 0.5f);
	// reset when off the bottom of the screen
	if(bkgd1Pos.y >= 768){

		bkgd1Pos.y = -768;
		BG1pos_Y = bkgd1Pos.y;

	}

	// Update Background 2
	BG2pos_Y += (bkgdSpeed * 1) * deltaTime;

	// set the new bkgd1 position
	bkgd2Pos.y = (int)(BG2pos_Y + 0.5f);

	// reset when off the bottom of the screen
	if(bkgd2Pos.y >= 768){

		bkgd2Pos.y = -768;
		BG2pos_Y = bkgd2Pos.y;
	}
}

//new joystick vars
const int JOYSTICK_DEAD_ZONE = 8000;

//joystick direction vars
float xDir, yDir;

//cursor float vars for movement
float pos_X,pos_Y;

//create SDL Rectangle for the 2 player graphics
SDL_Rect cursorPos, activePos;

//var for cursor speed
int cursorSpeed = 400;

void moveCursor(const SDL_ControllerAxisEvent event) {
	//check joystick 0 - player one
	if(event.which == 0){
		if(event.axis == 0){

			if(event.value < -JOYSTICK_DEAD_ZONE){
				xDir = -1.0f;
			}else if (event.value > JOYSTICK_DEAD_ZONE){
				xDir = 1.0f;
			}else{
				xDir = 0.0f;
			}
		}
		if(event.axis == 1){
			if(event.value < -JOYSTICK_DEAD_ZONE){
				yDir = -1.0f;
			}else if (event.value > JOYSTICK_DEAD_ZONE){
				yDir = 1.0f;
			}else{
				yDir = 0.0f;
			}
		}
	}
}

// update cursor on screen
void UpdateCursor(float deltaTime){

	pos_X += (cursorSpeed * xDir) * deltaTime;
	pos_Y += (cursorSpeed * yDir) * deltaTime;


	cursorPos.x = (int)(pos_X + 0.5f);
	cursorPos.y = (int)(pos_Y + 0.5f);

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

// variable for all Menu button overs
bool players1Over = false, players2Over = false, instructionsOver = false, quitOver = false, menuOver = false, playOver = false;

#include "player.h"

int main(int argc, char* argv[]) {

#if defined(_WIN32) || (_WIN64)

	cout << "Running on Windows" << endl;

	//get the current working directory
	string currentWorkingDirectory(getcwd(NULL, 0));

	//create a string linking to the mac's images folder
	string images_dir = currentWorkingDirectory + "\\Resources\\Images\\";

#endif

#if defined(__APPLE__)

	cout << "Running on Apple" << endl;
	//get the current working directory
	string currentWorkingDirectory(getcwd(NULL,0));

	//create a string linking to the mac's images folder
	string images_dir = currentWorkingDirectory + "/Resources/Images/";

#endif

#if defined(__linux__)

	cout << "Running on Linux" << endl;

	cout << "Added on Linux" << endl;

	//get the current working directory
	string currentWorkingDirectory(getcwd(NULL,0));

	//create a string linking to the mac's images folder
	string images_dir = currentWorkingDirectory + "/Resources/Images/";

#endif

	SDL_Window *window;                    // Declare a pointer

	SDL_Init(SDL_INIT_EVERYTHING);              // Initialize SDL2

	// Create an application window with the following settings:
	window = SDL_CreateWindow(
			"An SDL2 window",                  // window title
			SDL_WINDOWPOS_UNDEFINED,           // initial x position
			SDL_WINDOWPOS_UNDEFINED,           // initial y position
			1024,                               // width, in pixels
			768,                               // height, in pixels
			SDL_WINDOW_OPENGL                  // flags - see below
	);

	// Check that the window was successfully created
	if (window == NULL) {
		// In the case that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	// Declare renderer
	SDL_Renderer*renderer = NULL;

	// create the renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//**************************** create players **************************
	Player player1 = Player(renderer, 0, images_dir.c_str(), 250.0,500.0);
	Player player2 = Player(renderer, 1, images_dir.c_str(), 750.0,500.0);

	//*********************** Create Background *********************************
	SDL_Surface *surface = IMG_Load((images_dir + "Background Test.png").c_str());

	// create a SDL texture
	SDL_Texture *bkgd1;

	// place surface into the texture bkgd1
	bkgd1 = SDL_CreateTextureFromSurface(renderer,surface);

	// create a SDL texture - background 2
	SDL_Texture *bkgd2;

	// place surface into the texture bkgd1
	bkgd2 = SDL_CreateTextureFromSurface(renderer,surface);

	// free the SDL surface
	SDL_FreeSurface(surface);

	// set the X,Y,W, and H for the Rectangle
	bkgd1Pos.x = 0;
	bkgd1Pos.y = 0;
	bkgd1Pos.w = 1024;
	bkgd1Pos.h = 768;

	// set the X,Y,W, and H for the Rectangle
	bkgd2Pos.x = 0;
	bkgd2Pos.y = -768;
	bkgd2Pos.w = 1024;
	bkgd2Pos.h = 768;




	//********************** Create Main Menu ***************************

	//************** GAME TITLE *****************
	surface = IMG_Load((images_dir + "Game Title.png").c_str());

	// create a SDL texture
	SDL_Texture *title;

	// place surface into the texture bkgd1
	title = SDL_CreateTextureFromSurface(renderer,surface);

	// free the SDL surface
	SDL_FreeSurface(surface);

	// create SDL Rectangle for the title graphic

	SDL_Rect titlePos;

	// set the X,Y,W, and H for the Rectangle
	titlePos.x = 406;
	titlePos.y = 58;
	titlePos.w = 205;
	titlePos.h = 73;
	//************** GAME TITLE *****************

	//************** ONE PLAYER *****************
	surface = IMG_Load((images_dir + "One Player Game.png").c_str());

	// create a SDL texture
	SDL_Texture *onePlayer;

	// place surface into the texture bkgd1
	onePlayer = SDL_CreateTextureFromSurface(renderer,surface);

	// free the SDL surface
	SDL_FreeSurface(surface);

	surface = IMG_Load((images_dir + "One Player Game Pressed.png").c_str());

	// create a SDL texture
	SDL_Texture *onePlayerO;

	// place surface into the texture bkgd1
	onePlayerO = SDL_CreateTextureFromSurface(renderer,surface);

	// free the SDL surface
	SDL_FreeSurface(surface);

	// create SDL Rectangle for the title graphic

	SDL_Rect onePlayerPos;

	// set the X,Y,W, and H for the Rectangle
	onePlayerPos.x = 186;
	onePlayerPos.y = 214;
	onePlayerPos.w = 648;
	onePlayerPos.h = 72;
	//************** ONE PLAYER *****************

	//************** TWO PLAYER *****************
	surface = IMG_Load((images_dir + "Two Player Game.png").c_str());

	// create a SDL texture
	SDL_Texture *twoPlayer;

	// place surface into the texture bkgd1
	twoPlayer = SDL_CreateTextureFromSurface(renderer,surface);

	// free the SDL surface
	SDL_FreeSurface(surface);

	surface = IMG_Load((images_dir + "Two Player Game Pressed.png").c_str());

	// create a SDL texture
	SDL_Texture *twoPlayerO;

	// place surface into the texture bkgd1
	twoPlayerO = SDL_CreateTextureFromSurface(renderer,surface);

	// free the SDL surface
	SDL_FreeSurface(surface);

	// create SDL Rectangle for the title graphic

	SDL_Rect twoPlayerPos;

	// set the X,Y,W, and H for the Rectangle
	twoPlayerPos.x = 186;
	twoPlayerPos.y = 322;
	twoPlayerPos.w = 648;
	twoPlayerPos.h = 72;

	//************** TWO PLAYER *****************

	//************** INSTRUCTIONS *****************
	surface = IMG_Load((images_dir + "Instructions Button.png").c_str());

	// create a SDL texture
	SDL_Texture *instructionsB;

	// place surface into the texture bkgd1
	instructionsB = SDL_CreateTextureFromSurface(renderer,surface);

	// free the SDL surface
	SDL_FreeSurface(surface);

	surface = IMG_Load((images_dir + "Instructions Button Pressed.png").c_str());

	// create a SDL texture
	SDL_Texture *instructionsBO;

	// place surface into the texture bkgd1
	instructionsBO = SDL_CreateTextureFromSurface(renderer,surface);

	// free the SDL surface
	SDL_FreeSurface(surface);

	// create SDL Rectangle for the title graphic

	SDL_Rect instructionsBPos;

	// set the X,Y,W, and H for the Rectangle
	instructionsBPos.x = 284;
	instructionsBPos.y = 432;
	instructionsBPos.w = 467;
	instructionsBPos.h = 72;

	//************** INSTRUCTIONS *****************

	//************** QUIT GAME *****************
	surface = IMG_Load((images_dir + "Quit Button.png").c_str());

	// create a SDL texture
	SDL_Texture *quitB;

	// place surface into the texture bkgd1
	quitB = SDL_CreateTextureFromSurface(renderer,surface);

	// free the SDL surface
	SDL_FreeSurface(surface);

	surface = IMG_Load((images_dir + "Quit Button Pressed.png").c_str());

	// create a SDL texture
	SDL_Texture *quitBO;

	// place surface into the texture bkgd1
	quitBO = SDL_CreateTextureFromSurface(renderer,surface);

	// free the SDL surface
	SDL_FreeSurface(surface);

	// create SDL Rectangle for the title graphic

	SDL_Rect quitBPos;

	// set the X,Y,W, and H for the Rectangle
	quitBPos.x = 284;
	quitBPos.y = 545;
	quitBPos.w = 467;
	quitBPos.h = 72;

	//************** QUIT GAME *****************

	//********************** Create Main Menu ***************************

	//**************** CREATE CURSOR ***************
	surface = IMG_Load((images_dir + "Cursor.png").c_str());

	// create a SDL texture
	SDL_Texture *cursor;

	// place surface into the texture cursor
	cursor = SDL_CreateTextureFromSurface(renderer,surface);

	SDL_FreeSurface(surface);

	// create the SDL_Rectangle for the texture's position and size -x,y,w,h
	//SDL_Rect cursorPos, activePos;

	// set the X,Y,W, and H for the Rectangle
	cursorPos.x = 0;
	cursorPos.y = 0;
	cursorPos.w = 37;
	cursorPos.h = 36;

	// set the X,Y,W, and H for the Rectangle
	activePos.x = 10;
	activePos.y = 10;
	activePos.w = 10;
	activePos.h = 10;

	// cursor speed
	//int cursorSpeed = 400;

	//**************** CREATE CURSOR ***************

	//************** INSTRUCTIONS MENU *****************

	//************** INSTRUCTIONS TEXT *****************
	surface = IMG_Load((images_dir + "Instructions Text.png").c_str());
	// create a SDL texture
	SDL_Texture *instructionsT;

	// place surface into the texture bkgd1
	instructionsT = SDL_CreateTextureFromSurface(renderer,surface);

	// free the SDL surface
	SDL_FreeSurface(surface);

	// create SDL Rectangle for the title graphic

	SDL_Rect instructionsTPos;

	// set the X,Y,W, and H for the Rectangle
	instructionsTPos.x = 80;
	instructionsTPos.y = 160;
	instructionsTPos.w = 888;
	instructionsTPos.h = 364;

	//************** INSTRUCTIONS TEXT *****************

	//************** INSTRUCTIONS RETURN MAIN MENU *****************
	surface = IMG_Load((images_dir + "Return To Main Menu.png").c_str());

	// create a SDL texture
	SDL_Texture *returnToMainMenuI;

	// place surface into the texture bkgd1
	returnToMainMenuI = SDL_CreateTextureFromSurface(renderer,surface);

	// free the SDL surface
	SDL_FreeSurface(surface);

	// create SDL Rectangle for the title graphic

	SDL_Rect returnToMainMenuIPos;

	surface = IMG_Load((images_dir + "Return To Main Menu Pressed.png").c_str());

	// create a SDL texture
	SDL_Texture *returnToMainMenuIO;

	// place surface into the texture bkgd1
	returnToMainMenuIO = SDL_CreateTextureFromSurface(renderer,surface);

	// free the SDL surface
	SDL_FreeSurface(surface);

	// set the X,Y,W, and H for the Rectangle
	returnToMainMenuIPos.x = 100;
	returnToMainMenuIPos.y = 610;
	returnToMainMenuIPos.w = 848;
	returnToMainMenuIPos.h = 72;

	//************** INSTRUCTIONS RETURN MAIN MENU *****************

	//************** INSTRUCTIONS MENU *****************


	//************** ONE PLAYER GAME *****************

	//************** ONE PLAYER GRAPHIC *****************
	surface = IMG_Load((images_dir + "One Player Game.png").c_str());

	// create a SDL texture
	SDL_Texture *onePlayerGameG;

	// place surface into the texture bkgd1
	onePlayerGameG = SDL_CreateTextureFromSurface(renderer,surface);

	// free the SDL surface
	SDL_FreeSurface(surface);

	// create SDL Rectangle for the title graphic

	SDL_Rect onePlayerGameGPos;

	// set the X,Y,W, and H for the Rectangle
	onePlayerGameGPos.x = 200;
	onePlayerGameGPos.y = 160;
	onePlayerGameGPos.w = 648;
	onePlayerGameGPos.h = 72;
	//************** ONE PLAYER GRAPHIC *****************

	//************** ONE PLAYER GAME *****************


	//************** TWO PLAYER GAME *****************

	//************** TWO PLAYER GRAPHIC *****************
	surface = IMG_Load((images_dir + "Two Player Game.png").c_str());

	// create a SDL texture
	SDL_Texture *twoPlayerGameG;

	// place surface into the texture bkgd1
	twoPlayerGameG = SDL_CreateTextureFromSurface(renderer,surface);

	// free the SDL surface
	SDL_FreeSurface(surface);

	// create SDL Rectangle for the title graphic

	SDL_Rect twoPlayerGameGPos;

	// set the X,Y,W, and H for the Rectangle
	twoPlayerGameGPos.x = 200;
	twoPlayerGameGPos.y = 160;
	twoPlayerGameGPos.w = 648;
	twoPlayerGameGPos.h = 72;
	//************** TWO PLAYER GRAPHIC *****************

	//************** TWO PLAYER GAME *****************


	//************** WIN SCREEN *****************

	//************** WIN TEXT *****************
	surface = IMG_Load((images_dir + "Win Text.png").c_str());

	// create a SDL texture
	SDL_Texture *winT;

	// place surface into the texture bkgd1
	winT = SDL_CreateTextureFromSurface(renderer,surface);

	// free the SDL surface
	SDL_FreeSurface(surface);

	// create SDL Rectangle for the title graphic

	SDL_Rect winTPos;

	// set the X,Y,W, and H for the Rectangle
	winTPos.x = 200;
	winTPos.y = 188;
	winTPos.w = 667;
	winTPos.h = 119;

	//************** WIN TEXT *****************

	//************** WIN RETURN MAIN MENU *****************
	surface = IMG_Load((images_dir + "Return To Main Menu.png").c_str());

	// create a SDL texture
	SDL_Texture *returnToMainMenuW;

	// place surface into the texture bkgd1
	returnToMainMenuW = SDL_CreateTextureFromSurface(renderer,surface);

	// free the SDL surface
	SDL_FreeSurface(surface);

	// create SDL Rectangle for the title graphic

	SDL_Rect returnToMainMenuWPos;

	surface = IMG_Load((images_dir + "Return To Main Menu Pressed.png").c_str());

	// create a SDL texture
	SDL_Texture *returnToMainMenuWO;

	// place surface into the texture bkgd1
	returnToMainMenuWO = SDL_CreateTextureFromSurface(renderer,surface);

	// free the SDL surface
	SDL_FreeSurface(surface);

	// set the X,Y,W, and H for the Rectangle
	returnToMainMenuWPos.x = 88;
	returnToMainMenuWPos.y = 404;
	returnToMainMenuWPos.w = 848;
	returnToMainMenuWPos.h = 72;

	//************** WIN RETURN MAIN MENU *****************

	//************** WIN PLAY AGAIN *****************
	surface = IMG_Load((images_dir + "Play Again Button.png").c_str());

	// create a SDL texture
	SDL_Texture *playAgainBW;

	// place surface into the texture bkgd1
	playAgainBW = SDL_CreateTextureFromSurface(renderer,surface);

	// free the SDL surface
	SDL_FreeSurface(surface);

	// create SDL Rectangle for the title graphic

	SDL_Rect playAgainBWPos;

	surface = IMG_Load((images_dir + "Play Again Button Pressed.png").c_str());

	// create a SDL texture
	SDL_Texture *playAgainBWO;

	// place surface into the texture bkgd1
	playAgainBWO = SDL_CreateTextureFromSurface(renderer,surface);

	// free the SDL surface
	SDL_FreeSurface(surface);

	// set the X,Y,W, and H for the Rectangle
	playAgainBWPos.x = 302;
	playAgainBWPos.y = 574;
	playAgainBWPos.w = 459;
	playAgainBWPos.h = 72;

	//************** WIN PLAY AGAIN *****************

	//************** WIN SCREEN *****************


	//************** LOSE SCREEN *****************

	//************** LOSE TEXT *****************
	surface = IMG_Load((images_dir + "Lose Text.png").c_str());

	// create a SDL texture
	SDL_Texture *loseT;

	// place surface into the texture bkgd1
	loseT = SDL_CreateTextureFromSurface(renderer,surface);

	// free the SDL surface
	SDL_FreeSurface(surface);

	// create SDL Rectangle for the title graphic

	SDL_Rect loseTPos;

	// set the X,Y,W, and H for the Rectangle
	loseTPos.x = 259;
	loseTPos.y = 100;
	loseTPos.w = 547;
	loseTPos.h = 119;

	//************** LOSE TEXT *****************

	//************** LOSE RETURN MAIN MENU *****************
	surface = IMG_Load((images_dir + "Return To Main Menu.png").c_str());

	// create a SDL texture
	SDL_Texture *returnToMainMenuL;

	// place surface into the texture bkgd1
	returnToMainMenuL = SDL_CreateTextureFromSurface(renderer,surface);

	// free the SDL surface
	SDL_FreeSurface(surface);

	// create SDL Rectangle for the title graphic

	SDL_Rect returnToMainMenuLPos;

	surface = IMG_Load((images_dir + "Return To Main Menu Pressed.png").c_str());

	// create a SDL texture
	SDL_Texture *returnToMainMenuLO;

	// place surface into the texture bkgd1
	returnToMainMenuLO = SDL_CreateTextureFromSurface(renderer,surface);

	// free the SDL surface
	SDL_FreeSurface(surface);

	// set the X,Y,W, and H for the Rectangle
	returnToMainMenuLPos.x = 88;
	returnToMainMenuLPos.y = 404;
	returnToMainMenuLPos.w = 848;
	returnToMainMenuLPos.h = 72;

	//************** LOSE RETURN MAIN MENU *****************

	//************** LOSE PLAY AGAIN *****************
	surface = IMG_Load((images_dir + "Play Again Button.png").c_str());

	// create a SDL texture
	SDL_Texture *playAgainBL;

	// place surface into the texture bkgd1
	playAgainBL = SDL_CreateTextureFromSurface(renderer,surface);

	// free the SDL surface
	SDL_FreeSurface(surface);

	// create SDL Rectangle for the title graphic

	SDL_Rect playAgainBLPos;

	surface = IMG_Load((images_dir + "Play Again Button Pressed.png").c_str());

	// create a SDL texture
	SDL_Texture *playAgainBLO;

	// place surface into the texture bkgd1
	playAgainBLO = SDL_CreateTextureFromSurface(renderer,surface);

	// free the SDL surface
	SDL_FreeSurface(surface);

	// set the X,Y,W, and H for the Rectangle
	playAgainBLPos.x = 302;
	playAgainBLPos.y = 574;
	playAgainBLPos.w = 459;
	playAgainBLPos.h = 72;

	//************** LOSE PLAY AGAIN *****************

	//************** LOSE SCREEN *****************


	/*
	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	//Get window surface
	screenSurface = SDL_GetWindowSurface( window );

	//Fill the surface white
	SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0, 42, 254 ) );

	//Update the surface
	SDL_UpdateWindowSurface( window );
	 */
	//********************* CREATE CONTROLLERS ***********************************

	//***** Turn on Game Controller Events*****
	SDL_GameControllerEventState(SDL_ENABLE);

	//***** set up a Game Controller variable*****
	SDL_GameController* gGameController0 = NULL;

	//***** Open Game Controller*****
	gGameController0 = SDL_GameControllerOpen(0);

	//***** set up a Game Controller variable*****
	SDL_GameController* gGameController1 = NULL;

	//***** Open Game Controller*****
	gGameController1 = SDL_GameControllerOpen(1);

	//********************* CREATE CONTROLLERS ***********************************

	//***** SDL Event to handle input*****
	SDL_Event event;

	// set up variables for the game state
	enum GameState {MENU,INSTRUCTIONS,PLAYERS1,PLAYERS2, WIN,LOSE};

	//set up the initial state
	GameState gameState = MENU;

	//boolean values to control movement through the states
	bool menu, instructions,players1,players2,win,lose,quit;
	quit = false;


	// The window is open: could enter program loop here (see SDL_PollEvent())

	while(!quit)
	{
		switch(gameState)
		{
		case MENU:
			menu = true;
			while(menu)
			{
				// set up frame rate for the section, or CASE
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime)/1000;
				lastTime = thisTime;

				//Check for input events
				if(SDL_PollEvent(&event))
				{
					//check to see of the SDL Window is closed - player clicks the X in the window
					if(event.type == SDL_QUIT)
					{
						quit = true;
						menu = false;
						break;
					}
					switch(event.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:

						if(event.cdevice.which == 0)
						{
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								if (players1Over){
									menu = false;
									gameState = PLAYERS1;
									players1Over = false;
								}
								if (players2Over){
									menu = false;
									gameState = PLAYERS2;
									players2Over = false;
								}
								if (instructionsOver){
									menu = false;
									gameState = INSTRUCTIONS;
									instructionsOver = false;
								}
								if (quitOver){
									menu = false;
									quit = true;
									quitOver = false;
								}
							}
						}
						break;
						// ********************** Move cursor **************************
					case SDL_CONTROLLERAXISMOTION:
						moveCursor(event.caxis);
						break;
					}
				}
				//Update

				UpdateBackground(deltaTime);

				// Update Cursor
				UpdateCursor(deltaTime);

				players1Over = SDL_HasIntersection(&activePos, &onePlayerPos);
				players2Over = SDL_HasIntersection(&activePos, &twoPlayerPos);
				instructionsOver = SDL_HasIntersection(&activePos, &instructionsBPos);
				quitOver = SDL_HasIntersection(&activePos, &quitBPos);

				// Start Drawing

				//Clear SDL renderer
				SDL_RenderClear(renderer);

				// Draw the bkgd1 image
				SDL_RenderCopy(renderer,bkgd1,NULL,&bkgd1Pos);

				// Draw the bkgd2 image
				SDL_RenderCopy(renderer,bkgd2,NULL,&bkgd2Pos);

				// Draw the Game Title image
				SDL_RenderCopy(renderer,title,NULL,&titlePos);

				// *************************** Button Over states **************************
				// Draw the One Player image
				if(players1Over){
					SDL_RenderCopy(renderer,onePlayerO,NULL,&onePlayerPos);
				}else{
					SDL_RenderCopy(renderer,onePlayer,NULL,&onePlayerPos);
				}
				// Draw the Two Player image
				if(players2Over){
					SDL_RenderCopy(renderer,twoPlayerO,NULL,&twoPlayerPos);
				}else{
					SDL_RenderCopy(renderer,twoPlayer,NULL,&twoPlayerPos);
				}
				// Draw the Instructions image
				if(instructionsOver){
					SDL_RenderCopy(renderer,instructionsBO,NULL,&instructionsBPos);
				}else{
					SDL_RenderCopy(renderer,instructionsB,NULL,&instructionsBPos);
				}
				// Draw the Quit image
				if(quitOver){
					SDL_RenderCopy(renderer,quitBO,NULL,&quitBPos);
				}else{
					SDL_RenderCopy(renderer,quitB,NULL,&quitBPos);
				}
				// *************************** Button Over states **************************

				// Draw the cursor image
				SDL_RenderCopy(renderer,cursor,NULL,&cursorPos);

				//SDL Render present
				SDL_RenderPresent(renderer);
			}
			break; // end menu case

		case INSTRUCTIONS:
			instructions = true;
			while(instructions)
			{
				// set up frame rate for the section, or CASE
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime)/1000;
				lastTime = thisTime;

				//Check for input events
				if(SDL_PollEvent(&event))
				{
					//check to see of the SDL Window is closed - player clicks the X in the window
					if(event.type == SDL_QUIT)
					{
						quit = true;
						menu = false;
						break;
					}
					switch(event.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:
						if(event.cdevice.which == 0)
						{
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								if(menuOver) {
									instructions = false;
									gameState = MENU;
									menuOver = false;
								}
							}
						}
						break;
					case SDL_CONTROLLERAXISMOTION:
						moveCursor(event.caxis);
						break;
					}
				}

				//Update

				UpdateBackground(deltaTime);

				UpdateCursor(deltaTime);

				menuOver = SDL_HasIntersection(&activePos, &returnToMainMenuIPos);

				// Start Drawing

				//Clear SDL renderer
				SDL_RenderClear(renderer);

				// Draw the bkgd1 image
				SDL_RenderCopy(renderer,bkgd1,NULL,&bkgd1Pos);

				// Draw the bkgd2 image
				SDL_RenderCopy(renderer,bkgd2,NULL,&bkgd2Pos);

				// Draw the Game Title image
				SDL_RenderCopy(renderer,title,NULL,&titlePos);

				// Draw the Game Instructions Text image
				SDL_RenderCopy(renderer,instructionsT,NULL,&instructionsTPos);

				// Draw the Return to Main Menu button
				if(menuOver){
					SDL_RenderCopy(renderer, returnToMainMenuIO, NULL, &returnToMainMenuIPos);
				}else{
					SDL_RenderCopy(renderer, returnToMainMenuI, NULL, &returnToMainMenuIPos);
				}

				// Draw the cursor image
				SDL_RenderCopy(renderer,cursor,NULL,&cursorPos);

				//SDL Render present
				SDL_RenderPresent(renderer);

			}
			break; // end instruction case

		case PLAYERS1:
			players1 = true;
			while(players1)
			{
				// set up frame rate for the section, or CASE
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime)/1000;
				lastTime = thisTime;
				//Check for input events
				if(SDL_PollEvent(&event))
				{
					//check to see of the SDL Window is closed - player clicks the X in the window
					if(event.type == SDL_QUIT)
					{
						quit = true;
						players1 = false;
						break;
					}

					switch(event.type)
					{

					case SDL_CONTROLLERBUTTONDOWN:

						if(event.cdevice.which == 0 ||event.cdevice.which == 1)
						{
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
							{
								players1 = false;
								gameState = WIN;
							}

							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
							{
								players1 = false;
								gameState = LOSE;
							}
							player1.OnControllerButton(event.cbutton);
						}
						break;
					case SDL_CONTROLLERAXISMOTION:
						player1.OnControllerAxis(event.caxis);
						break;
					}
				}

				//Update

				UpdateBackground(deltaTime);

				// Update Player
				player1.Update(deltaTime);

				// Start Drawing

				//Clear SDL renderer
				SDL_RenderClear(renderer);

				// Draw the bkgd1 image
				SDL_RenderCopy(renderer,bkgd1,NULL,&bkgd1Pos);

				// Draw the bkgd2 image
				SDL_RenderCopy(renderer,bkgd2,NULL,&bkgd2Pos);

				// Draw the title image
				//SDL_RenderCopy(renderer,title,NULL,&titlePos);

				// Draw the one player graphic image image
				//SDL_RenderCopy(renderer,onePlayerGameG,NULL,&onePlayerGameGPos);

				//Draw Player1
				player1.Draw(renderer);

				// Draw the cursor image
				//SDL_RenderCopy(renderer,cursor,NULL,&cursorPos);

				//SDL Render present
				SDL_RenderPresent(renderer);

			}
			break; // end players1 case

		case PLAYERS2:
			players2 = true;
			while(players2)
			{
				// set up frame rate for the section, or CASE
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime)/1000;
				lastTime = thisTime;
				//Check for input events
				if(SDL_PollEvent(&event))
				{
					//check to see of the SDL Window is closed - player clicks the X in the window
					if(event.type == SDL_QUIT)
					{
						quit = true;
						players2 = false;
						break;
					}

					switch(event.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:

						if(event.cdevice.which == 0)
						{
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
							{
								players2 = false;
								gameState = WIN;
							}

							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
							{
								players2 = false;
								gameState = LOSE;
							}
						}
						player1.OnControllerButton(event.cbutton);
						player2.OnControllerButton(event.cbutton);

						break;

					case SDL_CONTROLLERAXISMOTION:
						player1.OnControllerAxis(event.caxis);
						player2.OnControllerAxis(event.caxis);
						break;
					}
				}

				//Update

				UpdateBackground(deltaTime);

				player1.Update(deltaTime);

				player2.Update(deltaTime);

				// Start Drawing

				//Clear SDL renderer
				SDL_RenderClear(renderer);

				// Draw the bkgd1 image
				SDL_RenderCopy(renderer,bkgd1,NULL,&bkgd1Pos);

				// Draw the bkgd2 image
				SDL_RenderCopy(renderer,bkgd2,NULL,&bkgd2Pos);

				// Draw the title image
				//SDL_RenderCopy(renderer,title,NULL,&titlePos);

				// Draw the two player graphic image image
				//SDL_RenderCopy(renderer,twoPlayerGameG,NULL,&twoPlayerGameGPos);

				player1.Draw(renderer);

				player2.Draw(renderer);

				// Draw the cursor image
				//SDL_RenderCopy(renderer,cursor,NULL,&cursorPos);

				//SDL Render present
				SDL_RenderPresent(renderer);


			}
			break; // end players2 case

		case WIN:
			win = true;
			while(win)
			{

				// set up frame rate for the section, or CASE
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime)/1000;
				lastTime = thisTime;

				//Check for input events
				if(SDL_PollEvent(&event))
				{
					//check to see of the SDL Window is closed - player clicks the X in the window
					if(event.type == SDL_QUIT)
					{
						quit = true;
						win = false;
						break;

					}

					switch(event.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:
						if(event.cdevice.which == 0)
						{
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								if(menuOver){
									win = false;
									gameState = MENU;
									menuOver = false;
								}
								if(playOver){
									win = false;
									gameState = PLAYERS1;
									playOver = false;
								}
							}
						}
						break;
					case SDL_CONTROLLERAXISMOTION:
						moveCursor(event.caxis);
						break;
					}
				}

				//Update

				UpdateBackground(deltaTime);

				UpdateCursor(deltaTime);

				menuOver = SDL_HasIntersection(&activePos, &returnToMainMenuWPos);
				playOver = SDL_HasIntersection(&activePos, &playAgainBWPos);

				// Start Drawing

				//Clear SDL renderer
				SDL_RenderClear(renderer);

				// Draw the bkgd1 image
				SDL_RenderCopy(renderer,bkgd1,NULL,&bkgd1Pos);

				// Draw the bkgd2 image
				SDL_RenderCopy(renderer,bkgd2,NULL,&bkgd2Pos);

				// Draw the Game Win Text image
				SDL_RenderCopy(renderer,winT,NULL,&winTPos);

				// Draw the Return to Main Menu button
				if(menuOver){
					SDL_RenderCopy(renderer,returnToMainMenuWO,NULL,&returnToMainMenuWPos);
				}else{
					SDL_RenderCopy(renderer,returnToMainMenuW,NULL,&returnToMainMenuWPos);
				}

				if(playOver){
					SDL_RenderCopy(renderer,playAgainBWO,NULL,&playAgainBWPos);
				}else{
					SDL_RenderCopy(renderer,playAgainBW,NULL,&playAgainBWPos);
				}

				// Draw the cursor image
				SDL_RenderCopy(renderer,cursor,NULL,&cursorPos);

				//SDL Render present
				SDL_RenderPresent(renderer);

			}
			break; // end WIN case

		case LOSE:
			lose = true;
			while(lose)
			{
				// set up frame rate for the section, or CASE
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime)/1000;
				lastTime = thisTime;

				//Check for input events
				if(SDL_PollEvent(&event))
				{
					//check to see of the SDL Window is closed - player clicks the X in the window
					if(event.type == SDL_QUIT)
					{
						quit = true;
						lose = false;
						break;
					}

					switch(event.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:

						if(event.cdevice.which == 0)
						{
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								if(menuOver){
									lose = false;
									gameState = MENU;
									menuOver = false;
								}
								if(playOver){
									lose = false;
									gameState = PLAYERS1;
									playOver = false;
								}
							}
						}
						break;
					case SDL_CONTROLLERAXISMOTION:
						moveCursor(event.caxis);
						break;
					}
				}

				//Update

				UpdateBackground(deltaTime);

				UpdateCursor(deltaTime);

				menuOver = SDL_HasIntersection(&activePos, &returnToMainMenuLPos);
				playOver = SDL_HasIntersection(&activePos, &playAgainBLPos);

				// Start Drawing

				//Clear SDL renderer
				SDL_RenderClear(renderer);

				// Draw the bkgd1 image
				SDL_RenderCopy(renderer,bkgd1,NULL,&bkgd1Pos);

				// Draw the bkgd2 image
				SDL_RenderCopy(renderer,bkgd2,NULL,&bkgd2Pos);

				// Draw the Game Lose Text image
				SDL_RenderCopy(renderer,loseT,NULL,&loseTPos);

				// Draw the Return to Main Menu button
				if(menuOver){
					SDL_RenderCopy(renderer,returnToMainMenuLO,NULL,&returnToMainMenuLPos);
				}else{
					SDL_RenderCopy(renderer,returnToMainMenuL,NULL,&returnToMainMenuLPos);
				}

				if(playOver){
					SDL_RenderCopy(renderer,playAgainBLO,NULL,&playAgainBLPos);
				}else{
					SDL_RenderCopy(renderer,playAgainBL,NULL,&playAgainBLPos);
				}

				// Draw the cursor image
				SDL_RenderCopy(renderer,cursor,NULL,&cursorPos);

				//SDL Render present
				SDL_RenderPresent(renderer);
			}
			break; // end LOSE case
		}
	}





	//SDL_Delay(3000);  // Pause execution for 3000 milliseconds, for example

	// Close and destroy the window
	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit();
	return 0;
}

