#include "Commands.hpp"

// RPL_INVITING			341
// ERR_NEEDMOREPARAMS	461
// ERR_NOSUCHCHANNEL	403
// ERR_NOTONCHANNEL		442
// ERR_CHANOPRIVSNEEDED	482
// ERR_USERONCHANNEL	443

void InviteCommand::run(User &user, std::vector<std::string> &args) {
	if (!user.is_online) {
		user.send_srv_msg("451", ":You are not authenticated");
		return;
	}
	if (args.size() < 3) {
		user.send_msg("461 " + user.nickname + " " + args[0] + " :Arguments are invalid");
		return ;
	}
	try {
		Channel &chan = Server::getInstance().getChannel(args[2]);

		if (!chan.isMember(user.nickname)) {
			user.send_msg("442 " + user.nickname + " " + args[2] + " :You are not in this channel");
		}
		if (!chan.isOperator(user.nickname) && !user.is_operator) {
			user.send_msg("482 " + user.nickname + " " + chan.name + " :Operator privilege needed");
			return;
		}
		if (!chan.is_private) {
			user.send_msg("461 " + args[0] + " :Channel is not private, why would you invite people?");
			return;
		}
		if (chan.isMember(args[1])) {
			user.send_msg("443 " + user.nickname + " " + args[1] + " " + args[2] + " :User already in channel");
			return;
		}
		try {
			User &target = Server::getInstance().getUserByNickname(args[1]);

			chan.invited.push_back(target.username);
			user.send_msg("341 " + user.nickname + " " + args[1] + " " + args[2]);
			target.send_raw(":" + user.nickname + " INVITE " + args[1] + " " + args[2]);
		} catch (Server::UserNotFound &) {
			user.send_msg("401 " + user.nickname + " " + args[1] + " :User not found");
		}
	}
	catch(const Server::ChannelNotFound &) {
		user.send_msg("403 " + user.nickname + " " + args[1] + " :Channel not found");
		return ;
	}
}