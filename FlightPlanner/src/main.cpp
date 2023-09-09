#include "Core.h"

#include <SDL2/SDL.h>

int main(int, char**)
{
	Core::init();
	Core::loop();
	Core::quit();

	return 0;
}
