#include "Commands.hpp"

// ERR_NONICKNAMEGIVEN	431
// ERR_ERRONEUSNICKNAME	432
// ERR_NICKNAMEINUSE	433
// ERR_NICKCOLLISION	436

void NickCommand::run(User &user, std::vector<std::string> &args) {
	if (args.size() != 2) {
		user.send_srv_msg("431", ": No nickname given");
		return ;
	}
	Server	&serv = Server::getInstance();
	for (Server::user_map::iterator it = serv.users.begin(); it != serv.users.end(); it++) {
		if (it->second.nickname == args[1]) {
			user.send_srv_msg("433", ":Nickname already in use");
			return ;
		}
	}
	for (size_t i = 0; i < args[1].size() - 1; i++) {
		if (!isalnum(args[1][i]) && args[1][i] != '_') {
			user.send_msg("432 " + user.nickname + " " + args[1] + " :Nickname can only contain alphanumeric and '_' characters.");
			return ;
		}
	}
	if (user.is_online) {
		user.send_raw(":" + user.nickname + " NICK " + args[1]);
		user.broadcast_channel(":" + user.nickname + " NICK " + args[1]);
		user.nickname = args[1];
	}
}