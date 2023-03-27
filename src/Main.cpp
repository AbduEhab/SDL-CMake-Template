#pragma once
#define SDL_MAIN_HANDLED

#include <Constants.hpp>
#include <SDL.h>
#include <stdio.h>

// uncomment the following lines if you want to use the SDL libraries (must be enabled in the CMakeLists.txt!!!)
// #include <SDL_image.h>
// #include <SDL_mixer.h>
// #include <SDL_ttf.h>
// #include <SDL_net.h>

int main([[maybe_unused]] int argc, [[maybe_unused]] char *arg[])
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("SDL could not initialize: Error %s\n", SDL_GetError());
		return 1;
	}

	// SDL Window setup
	SDL_Window *window = SDL_CreateWindow("SDL Template",
										  SDL_WINDOWPOS_UNDEFINED,
										  SDL_WINDOWPOS_UNDEFINED,
										  640, 480, 0);

	if (window == nullptr)
	{
		printf("Unable to create window. Error %s\n", SDL_GetError());
		return 1;
	}

	// Wait for 2,000 miliseconds (2 seconds)
	SDL_Delay(2000);

	// Destroy the window and shutdown the program
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}