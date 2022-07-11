#include "Commands.hpp"
#include "Server.hpp"

void NoticeCommand::run(User &user, std::vector<std::string> &args) {
	if (args.size() == 1) {
		user.send_srv_msg("411", ":No target to send the message");
		return;
	}
	if (args.size() == 2) {
		user.send_srv_msg("412", ":No message to send");
		return;
	}
	if (!user.isOnline()) {
		user.send_srv_msg("451", ":You are not authenticated");
		return;
	}
	std::vector<std::string> v;
	std::istringstream ss(args[1]);
	std::string word;

	while(std::getline(ss, word, ','))
		v.push_back(word);

	std::string msg = "";
	for (std::vector<std::string>::iterator it = args.begin() + 2; it != args.end(); ++it) {
		msg += *it;
		if (it != args.end() - 1)
			msg += " ";
	}

	for (std::vector<std::string>::iterator it = v.begin(); it != v.end(); ++it) {
		std::string chan_name;
		if (it->find(':') != it->npos)
			chan_name = it->substr(0, it->find(':'));
		else
			chan_name = *it;

		if (chan_name[0] == '#') {
			try {
				Channel &chan = Server::getInstance().getChannel(chan_name);

				if (!chan.isMember(user.getId())) {
					user.send_srv_msg("404", ":You're not in this channel");
					return;
				}
				Channel::id_vector lst = chan.getMembers();
				for (std::vector<std::string>::iterator ite = lst.begin(); ite != lst.end(); ++ite) {
					try {
						User &target = Server::getInstance().getUser(*ite);

						if (target.getId() == user.getId())
							continue;

						target.send_raw(":" + user.getId() + " NOTICE " + chan.getId() + " " + msg);
					} catch (Server::UserNotFound &) {}
				}
			} catch (Server::ChannelNotFound &) {
				user.send_srv_msg("402", ":No such channel");
				return;
			}
		} else {
			try {
				User &target = Server::getInstance().getUser(chan_name);

				target.send_raw(":" + user.getId() + " NOTICE " + target.getId() + " " + msg);
			} catch (Server::UserNotFound &) {
				user.send_srv_msg("301", ":User is offline or does not exist");
			}
		}

	}
}