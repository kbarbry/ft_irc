#include "Commands.hpp"

// ERR_NEEDMOREPARAMS	461
// ERR_ALREADYREGISTRED	462

void UserCommand::run(User &user, std::vector<std::string> &args) {
	if (!user.isAuth()) {
		user.send_srv_msg("461", ":You are not authenticated");
		return ;
	}
	if (args.size() < 5 || args[2] != "0" || args[3] != "*") {
		user.send_srv_msg("461", ":Arguments are invalid");
		return ;
	}
	if (user.isOnline() || Server::getInstance().isOnline(args[1])) {
		user.send_srv_msg("462", ":User is already connected");
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

	int fd = user.getSocketFd();

	user.setSocketFd(fd);
	try {
		Server	&serv = Server::getInstance();

		for (Server::user_map::iterator it; it != serv._users.end(); it++) {
			if ()
		}
	} catch(const std::exception& e) {
		std::cerr << e.what() << '\n';
	}
	
	if (user.getNickname() == "")
		user.setNickname(args[1]);
	user.setRealName(realname);
	user.setOnline(true);
	user.send_srv_msg("001 " + user.getId(), ":Welcome to our IRC server!");

	try {
		Server::getInstance().getChannel("#general");
	} catch (Server::ChannelNotFound &) {
		Server::getInstance().addChannel("#general");
		Server::getInstance().getChannel("#general").setTopic("Default channel for talking");
	}
	Channel &chan = Server::getInstance().getChannel("#general");
	if (chan.isPrivate())
		return;
	if (!chan.isMember(user.getId()))
		chan.addMember(user.getId());

	user.send_raw(":" + user.getId() + " JOIN " + chan.getId());
	user.send_msg(chan.getId() + ": " + chan.getTopic());

	std::string	lst_str = ":";
	Channel::id_vector lst = chan.getMembers();
	for (std::vector<std::string>::iterator ite = lst.begin(); ite != lst.end(); ++ite) {
		lst_str += *ite;
		if (ite != lst.end() - 1)
			lst_str += " ";
	}
	user.send_srv_msg("JOIN 353", lst_str);
}
