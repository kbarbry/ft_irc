#pragma once

#include <iostream>
#include <cstddef>
#include <ctime>

class User
{
public:
	std::string 	command;
	std::string 	response;
	std::string		nickname;
	std::string		username;
	std::string		real_name;

	int				socket_fd;

	bool			should_disconnect;
	bool			is_auth;
	bool			is_online;
	bool			is_operator;
	bool			pong_received;
	long int		last_ping;

public:
	User(int socket_fd);
	~User(void);

	const User	&operator=(const User &rhs);
	bool	operator==(const User &rhs)			const;
	bool	operator!=(const User &rhs)			const;

	void	broadcast_channel(const std::string &msg);
	void	send_srv_msg(const std::string &code, const std::string &msg);
	void	send_msg(const std::string &msg);
	void	send_raw(const std::string &msg);
	void	disconnect(const std::string &reason);
};
