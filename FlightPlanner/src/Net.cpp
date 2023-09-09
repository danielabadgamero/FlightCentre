#include "Net.h"

#include <vector>
#include <iostream>

void Net::open()
{
	SDLNet_ResolveHost(&ip, host.c_str(), 80);
	server = SDLNet_TCP_Open(&ip);
	set = SDLNet_AllocSocketSet(1);
	SDLNet_TCP_AddSocket(set, server);
}

void Net::send()
{
	std::string msg{ "GET " + url + " HTTP/1.1\n" };
	headers["Host"] = host;
	for (const auto& header : headers)
		msg.append(header.first + ": " + header.second + "\n");
	msg.push_back('\n');
	SDLNet_TCP_Send(server, msg.data(), msg.size());
	res.clear();
	while (SDLNet_CheckSockets(set, 100))
	{
		res.push_back(0);
		SDLNet_TCP_Recv(server, &res.back(), 1);
	}
}

void Net::close()
{
	headers.clear();
	url.clear();
	host.clear();

	SDLNet_TCP_Close(server);
	SDLNet_TCP_DelSocket(set, server);
	SDLNet_FreeSocketSet(set);
}
