#pragma once
#include <iostream>
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
#include <vector>
#include <istream>
#include <sstream>
#include <iterator>

class Xx_BotPifPafPoufProMaxExtreme_xX {
public:
	void run();

	Xx_BotPifPafPoufProMaxExtreme_xX(const std::string &host, int port, const std::string &pass, const std::string &name);
	~Xx_BotPifPafPoufProMaxExtreme_xX();
private:
	std::string	_host;
	int 		_port;
	std::string	_pass;
	std::string	_name;

	void handle_Xx_PifPafPoufProMaxExtreme_xX(int fd, const std::string &msg);
};