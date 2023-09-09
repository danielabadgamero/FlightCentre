#include "Core.h"

#include <SDL2/SDL_net.h>
#include <SDL2/SDL_image.h>

#include <iostream>

void Core::init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	SDLNet_Init();
	
	window = SDL_CreateWindow("Flight Planner", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
			960, 540, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	map.init();

	running = true;
}

void Core::loop()
{
	while (running)
	{
		SDL_Event e{};
		while (SDL_PollEvent(&e)) switch (e.type)
		{
		case SDL_QUIT:
			running = false;
			break;
		case SDL_WINDOWEVENT:
			SDL_GetWindowSize(window, &windowSize.x, &windowSize.y);
			break;
		case SDL_MOUSEMOTION:
			if (e.motion.state & SDL_BUTTON_LMASK)
			{
				map.getX() -= e.motion.xrel;
				map.getY() -= e.motion.yrel;
			}
			break;
		case SDL_MOUSEWHEEL:
			if (e.wheel.y > 0) if (map.getZ() + e.wheel.y > 12) { map.getZ() = 12; break; }
			if (e.wheel.y < 0) if (map.getZ() + e.wheel.y < 0) { map.getZ() = 0; break; }
			for (auto& tile : map.getTilesPtr()[map.getZ()])
				SDL_FreeSurface(tile.second);
			map.getTilesPtr()[map.getZ()].clear();
			map.getZ() += e.wheel.y;
			if (e.wheel.y > 0)
			{
				map.getX() *= 2;
				map.getY() *= 2;
			}
			else
			{
				map.getX() /= 2;
				map.getY() /= 2;
			}
			break;
		}

		SDL_SetRenderDrawColor(renderer, 0x10, 0x10, 0x10, 0xff);
		SDL_RenderClear(renderer);

		map.draw();

		SDL_RenderPresent(renderer);
	}
}

void Core::quit()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
