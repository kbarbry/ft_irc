#pragma once

#include <exception>
#include <stdint.h>
#include <map>
#include "Channel.hpp"
#include "Commands.hpp"
#include "SocketManager.hpp"
#include "ICommand.hpp"
#include "User.hpp"

class Server
{
public:
	typedef	std::map<int, User>					user_map;
	typedef std::map<std::string, std::string>	operator_map;
	typedef	std::map<std::string, Channel>		channel_map;
	typedef	std::map<std::string, ICommand *>	command_map;

	static int		max_clients;

	user_map			_users;
	operator_map 		_operators;
	channel_map			_channels;
	command_map			_commands;
	uint16_t 			_port;
	std::string			_password;

	Server(const Server &);
	void operator=(const Server &);

private:
	Server();

public:
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
	void	send_msg_to_all(const std::string &msg);

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
