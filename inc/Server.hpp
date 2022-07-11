#pragma once

#include <exception>
#include <stdint.h>
#include <iterator>
#include <map>
#include "Channel.hpp"
#include "Commands.hpp"
#include "SocketManager.hpp"
#include "ICommand.hpp"
#include "User.hpp"

class Server
{
private:
	Server();
	Server(const Server &);
	void operator=(const Server &);

public:
	typedef	std::map<int, User>					user_map;
	typedef std::map<std::string, std::string>	operator_map;
	typedef	std::map<std::string, Channel>		channel_map;
	typedef	std::map<std::string, ICommand *>	command_map;

	static int		max_clients;

	user_map			users;
	operator_map 		operators;
	channel_map			channels;
	command_map			commands;
	uint16_t 			port;
	std::string			password;

	~Server(void);

	// Singleton
	static Server	&getInstance(void) {
		static Server instance;
		return instance;
	}

	// Initializers
	void	init_cmds(void);
	void	configure(const uint16_t &port, const std::string &password);

	// Runners
	void	check_cmd(const std::string &txt, User &runner);
	void	run_cmd(const std::string &txt, User &runner);
	void	start(void);

	// Getters
	User				&getUser(int socket_fd);
	User				&getUserByUsername(const std::string &username);
	User				&getUserByNickname(const std::string &nickname);
	Channel				&getChannel(const std::string &id);
	const std::string	&getOperator(const std::string &op_name);
	bool				isOnline(const std::string &id);

	// Setters
	void	addUser(int socket_fd);
	void 	addChannel(User &user, const std::string &chan);
	void	addChannel(const std::string &channel_name);
	void	addOperator(const std::string &op_name, const std::string &op_password);

	// Exceptions
	class	UserNotFound:			public std::exception {};
	class	ChannelNotFound:		public std::exception {};
	class	OperatorNotFound:		public std::exception {};

	class	InvalidPort:			public std::exception {};
	class	InvalidPassword:		public std::exception {};
	class	AlreadyConfigured:		public std::exception {};

	class	StartFailure:			public std::exception {};
};
