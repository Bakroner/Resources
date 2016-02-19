#if defined(__APPLE__)

#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include "SDL2_mixer/SDL_mixer.h"
#include "SDL2_ttf/SDL_ttf.h"

#endif

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <stdint.h>

using namespace std;

#if defined(__APPLE__)
string currentWorkingDirectory(getcwd(NULL, 0));

string images_dir = currentWorkingDirectory + "/src/";

string audio_dir = currentWorkingDirectory + "/src/";
#endif

float deltaTime = 0.0;
int thisTime = 0;
int lastTime = 0;

int Main(){


	return 0;
}
