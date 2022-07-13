#include "Commands.hpp"

// ERR_NEEDMOREPARAMS	461
// ERR_NOSUCHCHANNEL	403
// ERR_NOTONCHANNEL		442
// ERR_CHANOPRIVSNEEDED	482
// RPL_NOTOPIC			331
// RPL_TOPIC			332
// RPL_TOPICWHOTIME		333

void TopicCommand::run(User &user, std::vector<std::string> &args) {
	if (!user.is_online) {
		user.send_srv_msg("451", ":You haven't registered");
		return;
	}
	if (args.size() == 1) {
		user.send_msg("461 " + user.nickname + " " + args[0] + " :Arguments are invalid");
		return;
	}
	if (args.size() == 2) {
		try {
			Channel &chan = Server::getInstance().getChannel(args[1]);

			if (!chan.isMember(user.nickname)) {
				user.send_msg("442 " + user.nickname + " " + chan.name + " :You're not in this channel");
				return;
			}
			if (chan.topic == "")
				user.send_msg("331 " + user.nickname + " " + chan.name + " :No topic is set");
			else
				user.send_msg("332 " + user.nickname + " " + chan.name + " :" + chan.topic);
		}
		catch (Server::ChannelNotFound &) {
			user.send_msg("403 " + user.nickname + " " + args[1] + " :Channel not found");
			return;
		}
	}
	else {
		try {
			Channel &chan = Server::getInstance().getChannel(args[1]);

			std::string topic = "";
			for (std::vector<std::string>::iterator it = args.begin() + 2; it != args.end(); ++it) {
				topic += *it;
				if (it != args.end() - 1)
					topic += " ";
			}
			if (!chan.isMember(user.nickname)) {
				user.send_msg("442 " + user.nickname + " " + chan.name + " :You're not in this channel");
				return;
			}
			if (!chan.isOperator(user.nickname) && !user.is_operator) {
				user.send_msg("482 " + user.nickname + " " + chan.name + " :Operator privilege needed");
				return;
			}
			if (args[2][0] == ':')
				topic.replace(0, 1, "");
			chan.topic =  topic;
			chan.send_msg_srv("332 " + user.nickname + " " + chan.name + " :" + topic);
		}
		catch (Server::ChannelNotFound &) {
			user.send_msg("403 " + user.nickname + " " + args[1] + " :Channel not found");
			return;
		}
	}
}