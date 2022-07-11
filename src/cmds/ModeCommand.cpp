#include "Commands.hpp"
#include "Server.hpp"

std::string ModeCommand::modes = "io";

void ModeCommand::run(User &user, std::vector<std::string> &args) {
	if (!user.isOnline()) {
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

			if (!chan.isMember(user.getId())) {
				user.send_srv_msg("404", ":You're not in this channel");
				return;
			}
			user.send_srv_msg("324 " + user.getId() + " " + chan.getId(), ":+" + (chan.isPrivate() ? std::string("i") : std::string("")));
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

		if (!chan.isMember(user.getId())) {
			user.send_srv_msg("404", ":You're not in this channel");
			return;
		}
		if (!chan.isOperator(user.getId()) && !user.isOperator()) {
			user.send_srv_msg("404", ":You are not an operator");
			return;
		}

		if (args[2][1] == 'i') {
			chan.setPrivate(toggleState);
			user.send_raw(":" + user.getId() + " MODE " + chan.getId() + " :" + (toggleState ? std::string("+i") : std::string("")));
		}
		if (args[2][1] == 'o') {
			if (args.size() < 4) {
				user.send_srv_msg("461", ":Arguments are invalid");
				return;
			}
			User &target = Server::getInstance().getUser(args[3]);

			if (toggleState && !chan.isOperator(target.getId())) {
				chan.addOperator(target.getId());
			}
			if (!toggleState && chan.isOperator(target.getId())) {
				chan.removeOperator(target.getId());
			}
			user.send_raw(":" + user.getId() + " MODE " + chan.getId() + " :" + (toggleState ? std::string("+o") : std::string("-o")) + " " + target.getId());
		}
	} catch (Server::ChannelNotFound &)  {
		user.send_srv_msg("402", ":No such channel");
		return;
	} catch (Server::UserNotFound &)  {
		user.send_srv_msg("441", ":User not found");
		return;
	}
}