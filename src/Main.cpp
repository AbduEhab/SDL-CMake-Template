#define SDL_MAIN_HANDLED

#include <Constants.hpp>
#include <SDL.h>
#include <stdio.h>

#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_sdlrenderer2.h>
#include <imgui.h>

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

	// SDL Renderer setup
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == nullptr)
	{
		printf("Unable to create renderer. Error %s\n", SDL_GetError());
		return 1;
	}

	SDL_GL_SetSwapInterval(1); // Enable vsync

	// init imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer2_Init(renderer);

	// vars for the bouncing rectangle
	int x = 60;
	int y = 60;
	int x_vel = 300;
	int y_vel = 300;

	TimePoint end_time_of_last_frame = Clock::now();
	float delta_time = 0.0f;
	while (true)
	{
		// Start the ImGui frame
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Hello, world!");
		ImGui::Text("Template Created by: %s", "AbduEhab");
		ImGui::End();

		// Clear the window to black
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Draw a rectangle
		SDL_Rect rect(x, y, 60, 60);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		// have rectangle bounce around the screen

		x += x_vel * delta_time;
		y += y_vel * delta_time;

		if (x >= 580 || x <= 0)
		{
			x_vel *= -1;
		}

		if (y >= 420 || y <= 0)
		{
			y_vel *= -1;
		}

		rect.x = x;
		rect.y = y;

		SDL_RenderFillRect(renderer, &rect);

		// Render the ImGui frame
		ImGui::Render();
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);

		// Update the window
		SDL_RenderPresent(renderer);

		// Check for quit event
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL2_ProcessEvent(&event);

			if (event.type == SDL_QUIT)
			{
				break;
			}
		}

		delta_time = (Clock::now() - end_time_of_last_frame).count() * 1e-9;
		end_time_of_last_frame = Clock::now();

		print(delta_time);
	}

	// Destroy the window and shutdown the program
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}