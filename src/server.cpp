//-----------------------------------------
// Written by - 		Alex Goldberg
// File Name-			server.cpp
// Code Reviewer -
// Review Date-
//-----------------------------------------

// this is the main server implementation
#include <netinet/in.h> //sockaddr_in
#include <sys/socket.h> //socket()

#include <netdb.h> //addrinfo

#include "config.hpp"	
#include "socket.hpp"
//------------------------------------------------------------------------------
namespace ilrd
{

//------------------------------------------------------------------------------
void init_server();
int get_listen_socket();
//------------------------------------------------------------------------------

int main()
{
	init_server();
	return (0);
}
//------------------------------------------------------------------------------

void init_server()
{
}
//------------------------------------------------------------------------------

int get_listen_socket()
{
	int opt = 1;
	int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(listen_socket,
			   SOL_SOCKET,
			   SO_REUSEADDR | SO_REUSEPORT,
			   &opt,
			   sizeof(opt));

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(atoi(tcp_listen_port));

	bind(listen_socket, (struct sockaddr *)&address, sizeof(address));

	listen(listen_socket, tcp_backlog_size);

	return (listen_socket);
}

int get_udp_socket()
{
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;	// set to AF_INET to force IPv4
	hints.ai_socktype = SOCK_DGRAM; // SOCK_DGRAM for UDP
	hints.ai_flags = AI_PASSIVE;	// use my IP

	struct addrinfo *servinfo;
	getaddrinfo(NULL, udp_port, &hints, &servinfo);

	int sockfd = socket(
		servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

	bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);

	freeaddrinfo(servinfo);

	return (sockfd);
}
//------------------------------------------------------------------------------

} // namespace ilrd
//------------------------------------------------------------------------------
