#pragma once

#include <iostream>
#include <cstddef>
#include <ctime>

class User
{
public:
	std::string 	command;
	bool			should_disconnect;
	std::string		_nickname;
	std::string		_username;
	std::string		_real_name;

	int				_socket_fd;
	bool			_is_auth;
	bool			_is_online;
	bool			_is_operator;
	long			_last_connexion;

public:
	User(int socket_fd);
	~User(void);

	const User	&operator=(const User &rhs);
	bool	operator==(const User &rhs)			const;
	bool	operator!=(const User &rhs)			const;

	void	send_srv_msg(const std::string &code, const std::string &msg);
	void	send_msg(const std::string &msg);
	void	send_raw(const std::string &msg);


};
