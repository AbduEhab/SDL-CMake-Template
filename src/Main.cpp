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

enum class Cycle
{
	UP,
	DOWN
};

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

	// SDL Renderer setup
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == nullptr)
	{
		printf("Unable to create renderer. Error %s\n", SDL_GetError());
		return 1;
	}

	uint8_t i = 0, j = 0, k = 0;
	Cycle cycle = Cycle::UP;

	// cycle rainbow colors to show that the window is working
	while (true)
	{
		// Clear the window to black
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Draw a rainbow rectangle
		SDL_Rect rect;
		rect.x = 0;
		rect.y = 0;
		rect.w = 640;
		rect.h = 480;

		SDL_SetRenderDrawColor(renderer, i, j, k, 255);
		SDL_RenderFillRect(renderer, &rect);

		// Update the window
		SDL_RenderPresent(renderer);

		// Cycle the colors
		switch (cycle)
		{
		case Cycle::UP:
			if (++i == 255)
			{
				j++;
				i = 254;
			}
			if (j == 255)
			{
				k++;
				j = 254;
			}
			if (k == 255)
			{
				cycle = Cycle::DOWN;
			}
			break;
		case Cycle::DOWN:
			if (--i == 0)
			{
				j--;
				i = 1;
			}
			if (j == 0)
			{
				k--;
				j = 1;
			}
			if (k == 0)
			{
				cycle = Cycle::UP;
			}

		default:
			break;
		}

		// Check for quit event
		SDL_Event event;
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				break;
			}
		}

		SDL_Delay(5);
	}

	// Destroy the window and shutdown the program
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}