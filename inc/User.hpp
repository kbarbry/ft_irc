#pragma once

#include <iostream>
#include <cstddef>
#include <ctime>

class User
{
public:
	std::string 	command;
	bool			should_disconnect;

private:
	std::string		_nickname;
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

	const std::string	&getNickname(void)		const;
	const std::string	&getRealName(void)		const;
	long				getLastConnexion(void)	const;
	int					getSocketFd(void)		const;
	bool				isOnline(void)			const;
	bool				isOperator()			const;
	bool				isAuth()				const;

	User	&setNickname(const std::string &newId);
	User	&setRealName(const std::string &newRealName);
	User	&setOnline(bool is_online);
	User	&setLastConnexion(long lastConnexion);
	User	&setIsOperator(bool isOperator);
	User	&setIsAuth(bool isAuth);
	User	&setSocketFd(int socketFd);

	void	send_srv_msg(const std::string &code, const std::string &msg);
	void	send_msg(const std::string &msg);
	void	send_raw(const std::string &msg);


};
