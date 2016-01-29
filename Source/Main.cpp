
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
void UpdateBackground(){
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
	SDL_Rect cursorPos, activePos;

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

	int cursorSpeed = 400;

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

	//***** set up a Game Controller variable*****
	SDL_GameController*gGameController = NULL;

	//***** Open Game Controller*****
	gGameController = SDL_GameControllerOpen(0);

	//***** Turn on Game Controller Events*****
	SDL_GameControllerEventState(SDL_ENABLE);

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
			cout << "The Game State is Menu" << endl;
			cout << "Press the A Button for Instructions" << endl;
			cout << "Press the B Button for 1 Player Game" << endl;
			cout << "Press the X Button for 2 Player Game" << endl;
			cout << "Press the Y Button to Quit Game" << endl;
			cout << endl;

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

								menu = false;
								gameState = INSTRUCTIONS;

							}

							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
							{

								menu = false;
								gameState = PLAYERS1;

							}
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
							{

								menu = false;
								gameState = PLAYERS2;

							}
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
							{

								menu = false;
								quit = true;

							}
						}
						break;
					}

				}
				//Update

				UpdateBackground();

				// Start Drawing

				//Clear SDL renderer
				SDL_RenderClear(renderer);

				// Draw the bkgd1 image
				SDL_RenderCopy(renderer,bkgd1,NULL,&bkgd1Pos);

				// Draw the bkgd2 image
				SDL_RenderCopy(renderer,bkgd2,NULL,&bkgd2Pos);

				// Draw the Game Title image
				SDL_RenderCopy(renderer,title,NULL,&titlePos);

				// Draw the One Player image
				SDL_RenderCopy(renderer,onePlayer,NULL,&onePlayerPos);

				// Draw the Two Player image
				SDL_RenderCopy(renderer,twoPlayer,NULL,&twoPlayerPos);

				// Draw the Instructions image
				SDL_RenderCopy(renderer,instructionsB,NULL,&instructionsBPos);

				// Draw the Quit image
				SDL_RenderCopy(renderer,quitB,NULL,&quitBPos);

				// Draw the cursor image
				SDL_RenderCopy(renderer,cursor,NULL,&cursorPos);

				//SDL Render present
				SDL_RenderPresent(renderer);



			}
			break; // end menu case

		case INSTRUCTIONS:
			instructions = true;
			cout << "The Game State is Instructions" << endl;
			cout << "Press the A Button for Main Menu" << endl;
			cout << endl;

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
						instructions = false;
						break;

					}

					switch(event.type)
					{

					case SDL_CONTROLLERBUTTONDOWN:

						if(event.cdevice.which == 0)
						{

							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{

								instructions = false;
								gameState = MENU;

							}
						}
						break;
					}
				}

				//Update

				UpdateBackground();

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
				SDL_RenderCopy(renderer,returnToMainMenuI,NULL,&returnToMainMenuIPos);

				// Draw the cursor image
				SDL_RenderCopy(renderer,cursor,NULL,&cursorPos);

				//SDL Render present
				SDL_RenderPresent(renderer);

			}
			break; // end instruction case

		case PLAYERS1:
			players1 = true;
			cout << "The Game State is 1 Player Game" << endl;
			cout << "Press the A Button for Win Screen" << endl;
			cout << "Press the B Button for Lose Screen" << endl;
			cout << endl;

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

						if(event.cdevice.which == 0)
						{

							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{

								players1 = false;
								gameState = WIN;

							}

							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
							{

								players1 = false;
								gameState = LOSE;

							}
						}
						break;
					}

				}

				//Update

				UpdateBackground();

				// Start Drawing

				//Clear SDL renderer
				SDL_RenderClear(renderer);

				// Draw the bkgd1 image
				SDL_RenderCopy(renderer,bkgd1,NULL,&bkgd1Pos);

				// Draw the bkgd2 image
				SDL_RenderCopy(renderer,bkgd2,NULL,&bkgd2Pos);

				// Draw the title image
				SDL_RenderCopy(renderer,title,NULL,&titlePos);

				// Draw the one player graphic image image
				SDL_RenderCopy(renderer,onePlayerGameG,NULL,&onePlayerGameGPos);

				// Draw the cursor image
				SDL_RenderCopy(renderer,cursor,NULL,&cursorPos);

				//SDL Render present
				SDL_RenderPresent(renderer);

			}
			break; // end players1 case

		case PLAYERS2:
			players2 = true;
			cout << "The Game State is 2 Player Game" << endl;
			cout << "Press the A Button for Win Screen" << endl;
			cout << "Press the B Button for Lose Screen" << endl;
			cout << endl;

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
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{

								players2 = false;
								gameState = WIN;

							}

							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
							{

								players2 = false;
								gameState = LOSE;

							}
						}
						break;
					}
				}

				//Update

				UpdateBackground();

				// Start Drawing

				//Clear SDL renderer
				SDL_RenderClear(renderer);

				// Draw the bkgd1 image
				SDL_RenderCopy(renderer,bkgd1,NULL,&bkgd1Pos);

				// Draw the bkgd2 image
				SDL_RenderCopy(renderer,bkgd2,NULL,&bkgd2Pos);

				// Draw the title image
				SDL_RenderCopy(renderer,title,NULL,&titlePos);

				// Draw the two player graphic image image
				SDL_RenderCopy(renderer,twoPlayerGameG,NULL,&twoPlayerGameGPos);

				// Draw the cursor image
				SDL_RenderCopy(renderer,cursor,NULL,&cursorPos);

				//SDL Render present
				SDL_RenderPresent(renderer);


			}
			break; // end players2 case

		case WIN:
			win = true;
			cout << "The Game State is WIN" << endl;
			cout << "Press the A Button for Main Menu" << endl;
			cout << "Press the B Button to Quit" << endl;
			cout << endl;

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

								win = false;
								gameState = MENU;

							}

							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
							{

								win = false;
								quit = true;

							}
						}
						break;
					}
				}

				//Update

				UpdateBackground();

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
				SDL_RenderCopy(renderer,returnToMainMenuW,NULL,&returnToMainMenuWPos);

				// Draw the Play Again button
				SDL_RenderCopy(renderer,playAgainBW,NULL,&playAgainBWPos);

				// Draw the cursor image
				SDL_RenderCopy(renderer,cursor,NULL,&cursorPos);

				//SDL Render present
				SDL_RenderPresent(renderer);

			}
			break; // end WIN case

		case LOSE:
			lose = true;
			cout << "The Game State is LOSE" << endl;
			cout << "Press the A Button for Main Menu" << endl;
			cout << "Press the B Button to Quit" << endl;
			cout << endl;

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

								lose = false;
								gameState = MENU;

							}

							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
							{

								lose = false;
								quit = true;

							}
						}
						break;
					}

				}

				//Update

				UpdateBackground();

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
				SDL_RenderCopy(renderer,returnToMainMenuL,NULL,&returnToMainMenuLPos);

				// Draw the Play Again button
				SDL_RenderCopy(renderer,playAgainBL,NULL,&playAgainBLPos);

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

