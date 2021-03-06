#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <algorithm>
#include <cstring>
#include <stdint.h>
#include <cstddef>
#include <exception>
#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include "Server.hpp"

class SocketManager
{

private:
	SocketManager();

	struct sockaddr_in	addr;
	uint16_t			port;
	int 				fd;

public:
	// Singleton
	static SocketManager	&getInstance(void) {
		static SocketManager instance;
		return instance;
	}

	SocketManager(const SocketManager &);
	void operator=(const SocketManager &);

	// Initializers
	void	init(const uint16_t &port);

	void	runtime(void);

	// Exceptions
	class SocketFailure:			public std::exception {};
	class SocketOptFailure:			public std::exception {};
	class FcntlFailure:				public std::exception {};
	class BindFailure:				public std::exception {};
	class ListenFailure:			public std::exception {};
	class ConnectionFailure:		public std::exception {};

};
