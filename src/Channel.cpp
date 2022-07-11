#include "Channel.hpp"
#include "Server.hpp"

Channel::Channel(const std::string &name, const std::string &owner): name(name), owner(owner), topic("No Topic") {
	members.push_back(owner);
	operators.push_back(owner);
}

Channel::Channel(const std::string &name): name(name), topic("No Topic") {}

Channel::~Channel(void) {}

void	Channel::send_msg_srv(const std::string &msg) {
	send_raw(":localhost " + msg);
}

void	Channel::send_raw(const std::string &msg) {
	Server	&serv = Server::getInstance();

	for (Channel::id_vector::iterator it = members.begin(); it != members.end(); it++) {
		// Pas securise expres, si ca crash, c'est qu'on delete pas le user quand il part d'un salon
		serv.getUserByUsername(*it).send_raw(msg);
	}
}

bool	Channel::isOperator(const std::string &nickname)		const {
	try {
		User &user = Server::getInstance().getUserByNickname(nickname);

		return std::find(operators.begin(), operators.end(), user.username) != operators.end();
	} catch (Server::UserNotFound &) {}
	return false;
}

bool	Channel::isMember(const std::string &nickname)		const {
	try {
		User &user = Server::getInstance().getUserByNickname(nickname);

		return std::find(members.begin(), members.end(), user.username) != members.end();
	} catch (Server::UserNotFound &) {}
	return false;
}

bool	Channel::isInvited(const std::string &nickname) const {
	try {
		User &user = Server::getInstance().getUserByNickname(nickname);

		return std::find(invited.begin(), invited.end(), user.username) != invited.end();
	} catch (Server::UserNotFound &) {}
	return false;
}

void Channel::uninviteUser(const std::string &nickname) {
	try {
		User &user = Server::getInstance().getUserByNickname(nickname);
		
		if (isInvited(nickname))
			invited.erase(std::find(invited.begin(), invited.end(), user.username));
	} catch (Server::UserNotFound &) {}
}

void Channel::removeMember(const std::string &nickname) {
	try {
		User &user = Server::getInstance().getUserByNickname(nickname);

		if (isMember(nickname))
			members.erase(std::find(members.begin(), members.end(), user.username));
	} catch (Server::UserNotFound &) {}
}

void Channel::removeOperator(const std::string &nickname) {
	try {
		User &user = Server::getInstance().getUserByNickname(nickname);

		if (isOperator(nickname))
			operators.erase(std::find(operators.begin(), operators.end(), user.username));
	} catch (Server::UserNotFound &) {}
}
