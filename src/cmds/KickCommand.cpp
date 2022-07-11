#include "Channel.hpp"
#include "Server.hpp"

// ERR_NEEDMOREPARAMS	461
// ERR_NOSUCHCHANNEL	403
// ERR_CHANOPRIVSNEEDED	482
// ERR_USERNOTINCHANNEL	441
// ERR_NOTONCHANNEL		442

void KickCommand::run(User &user, std::vector<std::string> &args) {
	if (!user.is_online) {
		user.send_srv_msg("451", ":You are not authenticated");
		return;
	}
	if (args.size() < 3) {
		user.send_srv_msg("461", ":Arguments are invalid");
		return ;
	}
	try {
		Channel &chan = Server::getInstance().getChannel(args[1]);

		if (!chan.isMember(user.nickname)) {
			user.send_srv_msg("442", ":You have to be part of the channel to kick someone from it");
			return;
		}
		if (!chan.isOperator(user.nickname) && !user.is_operator) {
			user.send_srv_msg("482", ":Operator priviledge needed");
			return;
		}
		if (!chan.isMember(args[2])) {
			user.send_srv_msg("441", ":User not found");
			return;
		}
		chan.removeMember(args[2]);
		std::string	reason = "";
		if (args.size() > 4) {
			if (args[4][0] == ':')
				args[4].replace(0, 1, "");
			for (std::vector<std::string>::iterator	it = args.begin() + 4; it != args.end(); it++)
				reason += *it;
		}
		else
			reason = "No reason specified";
		user.send_raw(args[2] + " have been kicked for " + reason + " by " + user.nickname);
	}
	catch(const Server::ChannelNotFound &) {
		user.send_srv_msg("403", ":No such channel");
		return ;
	}
}