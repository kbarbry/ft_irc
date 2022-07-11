#include <sys/socket.h>
#include "User.hpp"

User::User(int socket_fd): command(""), should_disconnect(false), _id("") , _nickname(""), _real_name(""), _socket_fd(socket_fd), _is_auth(false), _is_online(false), _is_operator(false) {
	_last_connexion = std::time(NULL);
}

User::~User(void) {}

const User &User::operator=(const User &rhs) {
	command				= rhs.command;
	should_disconnect	= rhs.should_disconnect;

	_is_auth			= rhs._is_auth;
	_nickname			= rhs._nickname;
	_socket_fd			= rhs._socket_fd;
	_is_online			= rhs._is_online;
	_real_name			= rhs._real_name;
	_is_operator		= rhs._is_operator;
	_last_connexion		= rhs._last_connexion;

	return *this;
}

bool User::operator==(const User &rhs) const {
	return _id == rhs._id;
}

bool User::operator!=(const User &rhs) const {
	return !(rhs == *this);
}

const std::string	&User::getId(void)				const	{ return _id; }
const std::string	&User::getNickname(void)		const	{ return _nickname; }
const std::string	&User::getRealName(void)		const	{ return _real_name; }
bool				User::isOnline(void)			const	{ return _is_online; }
long				User::getLastConnexion(void)	const	{ return _last_connexion; }

User	&User::setId(const std::string &newId)				{ _id = newId; return *this; }
User	&User::setNickname(const std::string &newNickname)	{ _nickname = newNickname; return *this; }
User	&User::setRealName(const std::string &newRealName)	{ _real_name = newRealName; return *this; }
User	&User::setOnline(bool is_online)					{ _is_online = is_online; return *this; }
User	&User::setLastConnexion(long lastConnexion)			{ _last_connexion = lastConnexion; return *this; }
User	&User::setSocketFd(int socketFd)					{ _socket_fd = socketFd; return *this; }
User	&User::setIsOperator(bool isOperator)				{	_is_operator = isOperator; return *this; }
User	&User::setIsAuth(bool isAuth)						{	_is_auth = isAuth; return *this; }

int		User::getSocketFd(void)						const	{ return _socket_fd; }
bool	User::isAuth()								const	{ return _is_auth; }
bool	User::isOperator()							const	{ return _is_operator; }

void User::send_msg(const std::string &msg) {
	std::string real_message = ":localhost " + msg;

	std::cout << "\033[0;31m<-- " << real_message << "\033[0;0m" << std::endl;

	real_message += "\r\n";
	send(_socket_fd, real_message.c_str(), real_message.length(), 0);
}

void User::send_srv_msg(const std::string &code, const std::string &msg) {
	std::string real_message = ":localhost " + code + " " + msg;

	std::cout << "\033[0;31m<-- " << real_message << "\033[0;0m" << std::endl;

	real_message += "\r\n";
	send(_socket_fd, real_message.c_str(), real_message.length(), 0);
}

void User::send_raw(const std::string &msg) {
	std::string real_message = msg;
	std::cout << "\033[0;31m<-- " << real_message << "\033[0;0m" << std::endl;

	real_message += "\r\n";
	send(_socket_fd, real_message.c_str(), real_message.length(), 0);
}
