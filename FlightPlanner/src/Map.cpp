#include "Map.h"
#include "Core.h"
#include "Net.h"

#include <SDL2/SDL_image.h>

#include <fstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

static int mod(int a, int b)
{
	return (a % b + b) % b;
}

static void getTile(int x, int y, int z)
{
	std::filesystem::path url{ "/" + std::to_string(z) + "/" + std::to_string(x) +
		"/" + std::to_string(y) + ".png" };
	std::filesystem::path tile{ cache.string() + url.string() };
	if (std::filesystem::exists(tile))
	{
		SDL_Surface* texture{ IMG_Load(tile.c_str()) };
		if (Core::map.getTilesPtr()[z].contains(std::pair<int, int>{ x, y }))
			Core::map.getTilesPtr()[z].at(std::pair<int, int>{ x, y }) = texture;
	}
	else
	{
		Net::url = url;
		Net::send();
		std::vector<char> res{ Net::res };
		do
		{
			if (res.empty())
			{
				return;
			}
			res.erase(res.begin());
		}
		while (static_cast<unsigned char>(res.front()) != 137);
		SDL_RWops* data{ SDL_RWFromConstMem(res.data(), res.size()) };
		SDL_Surface* texture{ IMG_Load_RW(data, 1) };
		if (z == Core::map.getZ())
			Core::map.getTilesPtr()[z].at(std::pair<int, int>{ x, y }) = texture;
		std::filesystem::path dirs{ tile };
		dirs.remove_filename();
		std::filesystem::create_directories(dirs);
		std::ofstream out{ tile.string(), std::ios::binary };
		out.write(res.data(), res.size());
	}
}

static int getTiles(void*)
{
	while (!Core::running);
	while (Core::running)
		if (!Core::tiles.empty())
		{
			if (Core::tiles.front().first == Core::map.getZ())
				getTile(Core::tiles.front().second.first, Core::tiles.front().second.second, Core::tiles.front().first);
			Core::tiles.pop();
		}

	return 0;
}

Map::Map()
{
	if (!std::filesystem::exists(cache))
		std::filesystem::create_directory(cache);
}

Map::~Map()
{
	for (auto& level : tiles)
	{
		for (auto& tile : level)
			SDL_FreeSurface(tile.second);
		level.clear();
	}
	SDL_WaitThread(thread, NULL);
}

void Map::init()
{
	Net::host = "tile.openstreetmap.org";
	Net::headers["User-Agent"] = "FlightPlanner";
	Net::open();
	thread = SDL_CreateThread(getTiles, "get_tiles", NULL);
}

void Map::draw()
{
	int startx{ x / 256 - 1 };
	int starty{ y / 256 - 1 };
	if (starty < 0) starty = 0;

	int endx{ Core::windowSize.x / 256 + 2 + x / 256 };
	int endy{ Core::windowSize.y / 256 + 2 + y / 256 };
	int maxHeight{ static_cast<int>(std::pow(2, z)) };
	if (endy > maxHeight) endy = maxHeight;

	for (int i{ starty }; i != endy; i++)
		for (int j{ startx }; j != endx; j++)
		{
			std::pair<int, int> pos{ mod(j, maxHeight), i };
			if (!tiles[z].contains(pos))
			{
				Core::tiles.push(std::pair<int, std::pair<int, int>>{ z, pos });
				tiles[z][pos] = NULL;
			}
			else if (tiles[z].at(pos))
			{
				SDL_Rect rect{ j * 256 - x, i * 256 - y, 256, 256 };
				SDL_Texture* texture{ SDL_CreateTextureFromSurface(Core::renderer, tiles[z].at(pos)) };
				SDL_RenderCopy(Core::renderer, texture, NULL, &rect);
				SDL_DestroyTexture(texture);
			}
		}
}

int& Map::getX(){ return x; }
int& Map::getY(){ return y; }
int& Map::getZ(){ return z; }
std::unordered_map<std::pair<int, int>, SDL_Surface*, Hash>* Map::getTilesPtr(){ return tiles; }
