/*
    PCH.hpp
    Declan Hopkins
    10/18/2015
*/

// TODO Make this an actual precompiled header
// The imports for this entire project are hinky. Fix this.

#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <sstream>
#include <time.h>
#include <ctime>
#include <stdlib.h>

#if WINDOWS
    #include <SDL.h>
    #include <SDL_ttf.h>
    #include <SDL_mixer.h>
#else
    #include "SDL2/SDL.h"
    #include "SDL2_ttf/SDL_ttf.h"
    #include "SDL2_mixer/SDL_mixer.h"
#endif
