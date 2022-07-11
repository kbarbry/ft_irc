#pragma once

#include "Server.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <stdint.h>
#include <cstddef>
#include <exception>
#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

class SocketManager
{

private:
	SocketManager();

	struct sockaddr_in	_addr;
	uint16_t			_port;
	int 				_fd;

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
