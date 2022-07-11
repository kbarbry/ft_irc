#include "Commands.hpp"

// ERR_NEEDMOREPARAMS	461
// ERR_ALREADYREGISTRED	462

void UserCommand::run(User &user, std::vector<std::string> &args) {
	if (!user.is_auth) {
		user.send_srv_msg("461", ":You are not authenticated");
		return ;
	}
	if (args.size() < 5 || args[2] != "0" || args[3] != "*") {
		user.send_srv_msg("461", ":Arguments are invalid");
		return ;
	}
	if (user.is_online) {
		user.send_srv_msg("462", ":You are already connected");
		return ;
	}
	if (args.size() > 5 && args[4][0] != ':') {
		user.send_srv_msg("461", ":Arguments are invalid");
		return ;
	}
	if (args[1].length() > 9) {
		user.send_srv_msg("461", ":Username is too long (max is 9 characters)");
		return ;
	}
	for (size_t i = 0; i < args[1].size() - 1; i++) {
		if (!isalnum(args[1][i]) && args[1][i] != '_') {
			user.send_srv_msg("461", ":Username can only contain alphanumeric and '_' characters.");
			return ;
		}
	}

	if (args[4][0] == ':')
		args[4].replace(0, 1, "");

	std::string realname;

	for (std::vector<std::string>::iterator it = args.begin() + 4; it != args.end(); ++it)
		realname.append(*it).append(" ");
	realname.pop_back();

	int fd = user.socket_fd;

	user.socket_fd = fd;
	Server	&serv = Server::getInstance();
	if (user.nickname == "") {
		for (Server::user_map::iterator it = serv.users.begin(); it != serv.users.end(); it++) {
			if (it->second.nickname == args[1]) {
				user.send_srv_msg("461", ":Nickname is already in use. Hint: Change nick with the NICK command");
				return;
			}
		}
		user.nickname = args[1];
	}
	user.username = args[1];
	user.real_name = realname;
	user.is_online = true;
	user.send_srv_msg("001 " + user.nickname, ":Welcome to our IRC server!");

	try {
		serv.getChannel("#general");
	} catch (Server::ChannelNotFound &) {
		serv.addChannel("#general");
		serv.getChannel("#general").topic = "Default channel for talking";
	}
	Channel &chan = serv.getChannel("#general");
	if (chan.is_private)
		return;
	if (!chan.isMember(user.nickname))
		chan.members.push_back(user.username);

	std::string	lst_str = ":";
	Channel::id_vector lst = chan.members;
	for (std::vector<std::string>::iterator ite = lst.begin(); ite != lst.end(); ++ite) {
		lst_str += *ite;
		if (ite != lst.end() - 1)
			lst_str += " ";
	}
	user.send_raw(":" + user.nickname + " JOIN " + chan.name);
	chan.send_msg_srv("353 " + user.nickname + " = " + chan.name + " :" + lst_str);
	chan.send_msg_srv("366 " + user.nickname + " " + chan.name + " :End of /NAMES list.");
}
