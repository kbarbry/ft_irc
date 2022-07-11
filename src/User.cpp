#include <sys/socket.h>
#include "User.hpp"
#include "Server.hpp"

User::User(int socket_fd): command(""), response(""), nickname(""), username(), real_name(""), socket_fd(socket_fd), should_disconnect(false), is_auth(false), is_online(false), is_operator(false), pong_received(true), last_ping(std::time(NULL)) {}

User::~User(void) {}

const User &User::operator=(const User &rhs) {
	command				= rhs.command;
	should_disconnect	= rhs.should_disconnect;

	is_auth				= rhs.is_auth;
	username			= rhs.username;
	nickname			= rhs.nickname;
	socket_fd			= rhs.socket_fd;
	is_online			= rhs.is_online;
	real_name			= rhs.real_name;
	is_operator			= rhs.is_operator;

	return *this;
}

bool User::operator==(const User &rhs) const {
	return nickname == rhs.nickname;
}

bool User::operator!=(const User &rhs) const {
	return !(rhs == *this);
}

void	User::broadcast_channel(const std::string &msg) {
	Server	&serv = Server::getInstance();

	for (Server::channel_map::iterator it = serv.channels.begin(); it != serv.channels.end(); it++) {
		if (it->second.isMember(this->nickname) && username != this->username)
			it->second.send_msg_srv(msg);
	}
}

void User::send_msg(const std::string &msg) {
	std::string real_message = ":localhost " + msg;
	std::cout << "\033[0;31m<-- " << real_message << "\033[0;0m" << std::endl;

	real_message += "\r\n";
	response += real_message;
}

void User::send_srv_msg(const std::string &code, const std::string &msg) {
	std::string real_message = ":localhost " + code + " " + nickname + " " + msg;
	std::cout << "\033[0;31m<-- " << real_message << "\033[0;0m" << std::endl;

	real_message += "\r\n";
	response += real_message;
}

void User::send_raw(const std::string &msg) {
	std::string real_message = msg;
	std::cout << "\033[0;31m<-- " << real_message << "\033[0;0m" << std::endl;

	real_message += "\r\n";
	response += real_message;
}

void	User::disconnect(const std::string &reason) {
	Server	&serv = Server::getInstance();

	for (Server::user_map::iterator it = serv.users.begin(); it != serv.users.end(); it++) {
		it->second.send_raw(":" + nickname + " QUIT :Quit: " + reason);
	}

	for (Server::channel_map::iterator it = serv.channels.begin(); it != serv.channels.end(); it++) {
		it->second.removeMember(nickname);
		it->second.removeOperator(nickname);
		it->second.uninviteUser(nickname);
		it->second.send_raw(":" + nickname + " PART " + it->second.name + " :" + reason);
	}

	should_disconnect = true;
}