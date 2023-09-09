#ifndef NET_H
#define NET_H

#include <SDL2/SDL_net.h>

#include <vector>
#include <string>
#include <unordered_map>

namespace Net
{
	inline std::string host{};
	inline std::unordered_map<std::string, std::string> headers{};
	inline std::string url{};
	inline std::vector<char> res{};

	inline TCPsocket server{};
	inline IPaddress ip{};
	inline SDLNet_SocketSet set{};

	void open();
	void send();
	void close();
}

#endif
