#include <iostream>
#include <stdint.h>
#include "Server.hpp"

int main(int ac, char **av)
{
	if (ac != 3) {
		std::cerr << "Use: ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	uint16_t	port = std::atoi(std::string(av[1]).c_str());
	std::string	password = std::string(av[2]).c_str();

	Server &server = Server::getInstance();
	try {
		server.configure(port, password);
	} catch (Server::AlreadyConfigured &) {
		std::cerr << "(╯°□°)╯︵ ┻━┻" << std::endl;
		return 1;
	} catch (Server::InvalidPort &) {
		std::cerr << "Invalid port." << std::endl;
		return 1;
	} catch (Server::InvalidPassword &) {
		std::cerr << "Invalid password (must contain between 4 and 16 characters)." << std::endl;
		return 1;
	}
	try {
		server.start();
	} catch (Server::StartFailure &) {
		return 1;
	} catch (std::exception &e) {
		std::cerr << "Global exception: " << e.what() << std::endl;
		return 1;
	}
    return 0;
}