#include "Commands.hpp"
#include "Server.hpp"

std::string ModeCommand::modes = "io";

void ModeCommand::run(User &user, std::vector<std::string> &args) {
	if (!user.is_online) {
		user.send_srv_msg("451", ":You are not authenticated");
		return;
	}
	if (args.size() < 2) {
		user.send_srv_msg("461", ":Arguments are invalid");
		return;
	}
	if (args.size() == 2) {
		try {
			Channel &chan = Server::getInstance().getChannel(args[1]);

			if (!chan.isMember(user.nickname)) {
				user.send_srv_msg("404", ":You're not in this channel");
				return;
			}
			user.send_srv_msg("324 " + user.nickname + " " + chan.name, ":+" + (chan.is_private ? std::string("i") : std::string("")));
		} catch (Server::ChannelNotFound &)  {
			user.send_srv_msg("402", ":No such channel");
		}
		return;
	}
	if (args[2][0] != '-' && args[2][0] != '+') {
		user.send_srv_msg("501", ":Modifier toggle required before mode name (+/-)");
		return;
	}
	if (!args[2][1] || modes.find(args[2][1]) == modes.npos) {
		user.send_srv_msg("501", ":Unknown modifier[s]. Supported: " + ModeCommand::modes);
		return;
	}
	bool toggleState = args[2][0] == '+';
	// Change les infos
	try {
		Channel &chan = Server::getInstance().getChannel(args[1]);

		if (!chan.isMember(user.nickname)) {
			user.send_srv_msg("404", ":You're not in this channel");
			return;
		}
		if (!chan.isOperator(user.nickname) && !user.is_operator) {
			user.send_srv_msg("404", ":You are not an operator");
			return;
		}

		if (args[2][1] == 'i') {
			chan.is_private = toggleState;
			user.send_raw(":" + user.nickname + " MODE " + chan.name + " :" + (toggleState ? std::string("+i") : std::string("")));
		}
		if (args[2][1] == 'o') {
			if (args.size() < 4) {
				user.send_srv_msg("461", ":Arguments are invalid");
				return;
			}
			User &target = Server::getInstance().getUserByNickname(args[3]);

			if (toggleState && !chan.isOperator(target.nickname)) {
				chan.operators.push_back(target.username);
			}
			if (!toggleState && chan.isOperator(target.nickname)) {
				chan.removeOperator(target.nickname);
			}
			user.send_raw(":" + user.nickname + " MODE " + chan.name + " :" + (toggleState ? std::string("+o") : std::string("-o")) + " " + target.nickname);
		}
	} catch (Server::ChannelNotFound &)  {
		user.send_srv_msg("402", ":No such channel");
		return;
	} catch (Server::UserNotFound &)  {
		user.send_srv_msg("441", ":User not found");
		return;
	}
}