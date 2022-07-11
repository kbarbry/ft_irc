#include "SocketManager.hpp"

SocketManager::SocketManager(): port(0), fd(0) {}

void	SocketManager::init(const uint16_t &port) {
	int	status = 1;

	this->port = port;
	this->fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd <= 0)
		throw SocketFailure();
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &status, sizeof(int)))
		throw SocketOptFailure();
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
		throw FcntlFailure();

	// Socket settings
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) < 0)
		throw BindFailure();
	if (listen(fd, addr.sin_port) < 0)
		throw ListenFailure();
}

void SocketManager::runtime(void) {
	fd_set			input_fds, output_fds;
	struct timeval	timeout{ 5, 0 };
	int				clients[Server::max_clients];
	int				max_fd;
	int				new_client;
	int				addrlen = sizeof(addr);

	for (int i = 0; i < Server::max_clients; i++)
		clients[i] = 0;

	Server &server = Server::getInstance();
	while (1) {
		// Reset la liste de FDs a lire
		FD_ZERO(&input_fds);
		FD_ZERO(&output_fds);
		// Ajouter le socket master a la liste de lecture
		FD_SET(fd, &input_fds);

		// Globalement verif si un client est present et a un mot a dire
		max_fd = fd;
		for (int i = 0; i < Server::max_clients; i++) {
			int client_fd = clients[i];

			if (client_fd > 0) {
				try {
					if (server.getUser(client_fd).should_disconnect)
					{
						server.users.erase(server.users.find(client_fd));
						getpeername(client_fd , (struct sockaddr*) &addr, (socklen_t*) &addrlen);
						std::cout << "Host was disconnected, ip " << inet_ntoa(addr.sin_addr) << ", port " << ntohs(addr.sin_port) << std::endl;
						close(client_fd);
						clients[i] = 0;
					} else {
						FD_SET(client_fd, &input_fds);
						FD_SET(client_fd, &output_fds);
					}
				} catch (Server::UserNotFound &) {}
			}

			if (client_fd > max_fd)
				max_fd = client_fd;
		}

		// Attendre une activite d'au moins un client
		int activity = select(max_fd + 1, &input_fds, &output_fds, NULL, &timeout);

		if ((activity < 0) && (errno != EINTR))
			std::cerr << "select error\n";

		for (Server::user_map::iterator it = server.users.begin(); it != server.users.end(); it++) {
			if (it->second.last_ping + 30 < std::time(NULL)) {
				if (!it->second.pong_received) {
					it->second.disconnect("Shitty connexion");
					continue;
				}
				it->second.pong_received = false;
				it->second.send_msg("PING localhost");
			}
		}

		// Si le client est pas dans la liste de FD (nouveau client)
		if (FD_ISSET(fd, &input_fds)) {
			// Accepter sa connection
			new_client = accept(fd, (struct sockaddr *) &addr, (socklen_t *) &addrlen);

			if (new_client < 0)
				throw SocketFailure();

			std::cout << "New connection, socket fd is " << new_client << ", ip is " << inet_ntoa(addr.sin_addr) << ", port: " << ntohs(addr.sin_port) << std::endl;

			// Ajouter le nouveau client dans la liste de FD
			for (int i = 0; i < Server::max_clients; i++)
			{
				if (clients[i] == 0)
				{
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
				// On lis ce qu'il dit ce batard
				valread = read(client_fd , buffer, 1024);
				if (valread < 0)
					return;
				// Si le mec s'est deco...
				if (valread == 0)
				{
					server.users.erase(server.users.find(client_fd));
					getpeername(client_fd , (struct sockaddr*) &addr, (socklen_t*) &addrlen);
					std::cout << "Host disconnected, ip " << inet_ntoa(addr.sin_addr) << ", port " << ntohs(addr.sin_port) << std::endl;
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
