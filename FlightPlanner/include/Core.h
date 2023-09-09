#ifndef CORE_H
#define CORE_H

#include "Map.h"

#include <SDL2/SDL.h>

#include <queue>

namespace Core
{
	inline SDL_Window* window{};
	inline SDL_Renderer* renderer{};
	inline SDL_Point windowSize{};
	inline bool running{};

	inline Map map{};
	inline std::queue<std::pair<int, std::pair<int, int>>> tiles{};

	void init();
	void loop();
	void quit();
}

#endif
