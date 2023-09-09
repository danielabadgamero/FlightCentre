#ifndef MAP_H
#define MAP_H

#include <filesystem>
#include <unordered_map>
#include <SDL2/SDL.h>

struct Hash
{
	size_t operator()(const std::pair<int, int>& p) const
	{
		return static_cast<size_t>(p.first) << 32 | p.second;
	}
};

inline const std::filesystem::path cache{ "./cache" };
inline SDL_Thread* thread{};

class Map
{
private:
	int z{};
	int x{};
	int y{};
	std::unordered_map<std::pair<int, int>, SDL_Surface*, Hash> tiles[20]{};
public:
	Map();
	~Map();
	int& getX();
	int& getY();
	int& getZ();
	std::unordered_map<std::pair<int, int>, SDL_Surface*, Hash>* getTilesPtr();
	void init();
	void draw();
};

#endif
