#include "Channel.hpp"
#include "Server.hpp"

Channel::Channel(const std::string &id, const std::string &owner): _id(id), _owner(owner), _topic("No Topic") {
	addMember(owner);
	addOperator(owner);
}

Channel::Channel(const std::string &id): _id(id), _topic("No Topic") {}

Channel::~Channel(void) {}

const std::string			&Channel::getId(void)		const	{ return _id; }
const std::string			&Channel::getOwner(void)	const	{ return _owner; }
const std::string			&Channel::getTopic()		const	{ return _topic; }

void Channel::setTopic(const std::string &topic)				{ _topic = topic; }

bool	Channel::isOperator(const std::string &id)		const {
	return std::find(_operators.begin(), _operators.end(), id) != _operators.end();
}

bool	Channel::isMember(const std::string &id)		const {
	return std::find(_members.begin(), _members.end(), id) != _members.end();
}

bool	Channel::isBanned(const std::string &id) const {
	return std::find(_banned.begin(), _banned.end(), id) != _banned.end();
}

bool	Channel::isInvited(const std::string &id) const {
	return std::find(_invited.begin(), _invited.end(), id) != _invited.end();
}

void Channel::banUser(const std::string &id) {
	_banned.push_back(id);
}

void Channel::inviteUser(const std::string &id) {
	_invited.push_back(id);
}

void Channel::addMember(const std::string &id) {
	_members.push_back(id);
}

void Channel::addOperator(const std::string &id) {
	_operators.push_back(id);
}

void Channel::setPrivate(bool is_private) {
	_private = is_private;
}

bool Channel::isPrivate() const {
	return _private;
}

void Channel::unbanUser(const std::string &id) {
	if (isBanned(id))
		_banned.erase(std::find(_banned.begin(), _banned.end(), id));
}

void Channel::uninviteUser(const std::string &id) {
	if (isInvited(id))
		_invited.erase(std::find(_invited.begin(), _invited.end(), id));
}

void Channel::removeMember(const std::string &id) {
	if (isMember(id))
		_members.erase(std::find(_members.begin(), _members.end(), id));
}

void Channel::removeOperator(const std::string &id) {
	if (isOperator(id))
		_operators.erase(std::find(_operators.begin(), _operators.end(), id));
}

const Channel::id_vector &Channel::getOperators() const {
	return _operators;
}

const Channel::id_vector &Channel::getMembers() const {
	return _members;
}

const Channel::id_vector &Channel::getBanned() const {
	return _banned;
}

const Channel::id_vector &Channel::getInvited() const {
	return _invited;
}