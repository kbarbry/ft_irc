#include "Channel.hpp"

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
		user.send_msg("461 " + user.nickname + " " + args[0] + " :Arguments are invalid");
		return ;
	}
	try {
		Channel &chan = Server::getInstance().getChannel(args[1]);

		if (!chan.isMember(user.nickname)) {
			user.send_msg("442 " + user.nickname + " " + args[1] + " :You are not in this channel");
			return;
		}
		if (!chan.isOperator(user.nickname) && !user.is_operator) {
			user.send_msg("482 " + user.nickname + " " + chan.name + " :Operator privilege needed");
			return;
		}
		if (!chan.isMember(args[2])) {
			user.send_msg("441 " + user.nickname + " " + args[2] + " " + args[1] + " :User not in this channel");
			return;
		}
		std::string	reason = "";
		if (args.size() > 4) {
			if (args[4][0] == ':')
				args[4].replace(0, 1, "");
			for (std::vector<std::string>::iterator	it = args.begin() + 4; it != args.end(); it++)
				reason += *it;
		}
		else
			reason = "No reason specified";
		chan.send_raw(":" + user.nickname + " KICK " + chan.name + " " + args[2] + " :" + reason);
		chan.removeMember(args[2]);
	}
	catch(const Server::ChannelNotFound &) {
		user.send_msg("403 " + user.nickname + " " + args[1] + " :Channel not found");
		return ;
	}
}