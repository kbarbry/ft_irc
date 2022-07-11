#include "Commands.hpp"
#include "Server.hpp"

// RPL_INVITING			341
// ERR_NEEDMOREPARAMS	461
// ERR_NOSUCHCHANNEL	403
// ERR_NOTONCHANNEL		442
// ERR_CHANOPRIVSNEEDED	482
// ERR_USERONCHANNEL	443

void InviteCommand::run(User &user, std::vector<std::string> &args) {
	if (!user.isOnline()) {
		user.send_srv_msg("451", ":You are not authenticated");
		return;
	}
	if (args.size() < 3) {
		user.send_srv_msg("461", ":Arguments are invalid");
		return ;
	}
	try {
		Channel &chan = Server::getInstance().getChannel(args[2]);

		if (!chan.isMember(user.getId())) {
			user.send_srv_msg("442", ":You have to be part of the channel to invite someone on it");
			return;
		}
		if (chan.isPrivate() && !chan.isOperator(user.getId()) && !user.isOperator()) {
			user.send_srv_msg("482", ":Operator priviledge needed");
			return;
		}
		if (!chan.isMember(user.getId())) {
			user.send_srv_msg("442", ":You have to be part of the channel to invite someone on it");
			return;
		}
		if (chan.isMember(args[1])) {
			user.send_srv_msg("443", ":User allready on channel");
			return;
		}
		chan.inviteUser(args[1]);
		user.send_srv_msg("341", ":" + user.getId() + " INVITE " + args[1] + chan.getId());
	}
	catch(const Server::ChannelNotFound &) {
		user.send_srv_msg("403", ":No such channel");
		return ;
	}
}