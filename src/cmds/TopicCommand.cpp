#include "Commands.hpp"
#include "Server.hpp"

// ERR_NEEDMOREPARAMS	461
// ERR_NOSUCHCHANNEL	403
// ERR_NOTONCHANNEL		442
// ERR_CHANOPRIVSNEEDED	482
// RPL_NOTOPIC			331
// RPL_TOPIC			332
// RPL_TOPICWHOTIME		333

void TopicCommand::run(User &user, std::vector<std::string> &args) {
	if (!user.isOnline()) {
		user.send_srv_msg("451", ":You are not authenticated");
		return;
	}
	if (args.size() == 1) {
		user.send_srv_msg("461", ":Arguments are invalid");
		return;
	}
	if (args.size() == 2) {
		try {
			Channel &chan = Server::getInstance().getChannel(args[1]);

			if (!chan.isMember(user.getId())) {
				user.send_srv_msg("442", ":You're not in this channel");
				return;
			}
			if (chan.getTopic() == "")
				user.send_srv_msg("331", ":No topic set");
			else
				user.send_srv_msg("332", user.getId() + " " + chan.getId() + " :" + chan.getTopic());
		}
		catch (Server::ChannelNotFound &) {
			user.send_srv_msg("403", ":Channel not found");
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
			if (!chan.isMember(user.getId())) {
				user.send_srv_msg("442", ":You're not in this channel");
				return;
			}
			if (!chan.isOperator(user.getId()) && !user.isOperator()) {
				user.send_srv_msg("482", ":Operator privilege needed");
				return;
			}
			if (args[2][0] == ':')
				topic.replace(0, 1, "");
			chan.setTopic(topic);
		}
		catch (Server::ChannelNotFound &) {
			user.send_srv_msg("403", ":Channel not found");
			return;
		}
	}
}