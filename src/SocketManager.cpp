#include "SocketManager.hpp"

SocketManager::SocketManager(): _port() {}

void	SocketManager::init(const uint16_t &port) {
	int	status = 1;

	_port = port;
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd <= 0)
		throw SocketFailure();
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &status, sizeof(int)))
		throw SocketOptFailure();
	if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0)
		throw FcntlFailure();

	// Socket settings
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = INADDR_ANY;
	_addr.sin_port = htons(_port);

	if (bind(_fd, (struct sockaddr *) &_addr, sizeof(_addr)) < 0)
		throw BindFailure();
	if (listen(_fd, _addr.sin_port) < 0)
		throw ListenFailure();
}

void SocketManager::runtime(void) {
	fd_set			input_fds, output_fds, except_fds;
	struct timeval	*timeout;
	int				clients[Server::max_clients];
	int				max_fd;
	int				new_client;
	int				addrlen = sizeof(_addr);

	for (int i = 0; i < Server::max_clients; i++)
		clients[i] = 0;

	while (1) {
		// Reset la liste de FDs a lire
		FD_ZERO(&input_fds);
		// Ajouter le socket master a la liste de lecture
		FD_SET(_fd, &input_fds);

		// Globalement verif si un client est present et a un mot a dire
		max_fd = _fd;
		for (int i = 0; i < Server::max_clients; i++) {
			int client_fd = clients[i];

			if (client_fd > 0) {
				try {
					if (Server::getInstance().getUser(client_fd).should_disconnect)
					{
						Server::getInstance().storeUser(client_fd);
						getpeername(client_fd , (struct sockaddr*) &_addr, (socklen_t*) &addrlen);
						std::cout << "Host disconnected, ip " << inet_ntoa(_addr.sin_addr) << ", port " << ntohs(_addr.sin_port) << std::endl;
						close(client_fd);
						clients[i] = 0;
					} else {
						FD_SET(client_fd, &input_fds);
					}
				} catch (Server::UserNotFound &) {}
			}

			if (client_fd > max_fd)
				max_fd = client_fd;
		}

		// Attendre une activite d'au moins un client
		int activity = select(max_fd + 1, &input_fds, &output_fds, &except_fds, timeout);

		if ((activity < 0) && (errno != EINTR))
			std::cerr << "select error\n";

		// Si le client est pas dans la liste de FD (nouveau client)
		if (FD_ISSET(_fd, &input_fds)) {
			// Accepter sa connection
			new_client = accept(_fd, (struct sockaddr *) &_addr, (socklen_t *) &addrlen);

			if (new_client < 0)
				throw SocketFailure();

			std::cout << "New connection, socket fd is " << new_client << ", ip is " << inet_ntoa(_addr.sin_addr) << ", port: " << ntohs(_addr.sin_port) << std::endl;

			// Ajouter le nouveau client dans la liste de FD
			for (int i = 0; i < Server::max_clients; i++)
			{
				if (clients[i] == 0)
				{
					Server &server = Server::getInstance();

					clients[i] = new_client;
					server.addUser(new_client);
					break;
				}
			}
		}

		for (int i = 0; i < Server::max_clients; i++)
		{
			int		client_fd = clients[i];
			ssize_t	valread;
			char	buffer[1025];

			// Un des clients a dit un truc?
			if (FD_ISSET(client_fd , &input_fds))
			{
				Server &server = Server::getInstance();
				// On lis ce qu'il dit ce batard
				valread = read(client_fd , buffer, 1024);
				if (valread < 0)
					return;
				// Si le mec s'est deco...
				if (valread == 0)
				{
					server.storeUser(client_fd);
					getpeername(client_fd , (struct sockaddr*) &_addr, (socklen_t*) &addrlen);
					std::cout << "Host disconnected, ip " << inet_ntoa(_addr.sin_addr) << ", port " << ntohs(_addr.sin_port) << std::endl;
					close(client_fd);
					clients[i] = 0;
				}
				// Ptn insane le mec a dis un truc :D
				else
				{
					buffer[valread] = '\0';
//					std::cout << i << ": " << buffer;
					server.check_cmd(std::string().insert(0, buffer), server.getUser(client_fd));
				}
			}
		}
	}
}
