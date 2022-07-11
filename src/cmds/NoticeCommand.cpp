#include "Commands.hpp"
#include "Server.hpp"

void NoticeCommand::run(User &user, std::vector<std::string> &args) {
	if (args.size() == 1 || args.size() == 2 || !user.is_online) {
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

				if (!chan.isMember(user.nickname)) {
					return;
				}
				Channel::id_vector lst = chan.members;
				for (std::vector<std::string>::iterator ite = lst.begin(); ite != lst.end(); ++ite) {
					try {
						User &target = Server::getInstance().getUserByUsername(*ite);

						if (target.nickname == user.nickname)
							continue;

						target.send_raw(":" + user.nickname + " NOTICE " + chan.name + " " + msg);
					} catch (Server::UserNotFound &) {}
				}
			} catch (Server::ChannelNotFound &) {}
		} else {
			try {
				User &target = Server::getInstance().getUserByNickname(chan_name);

				target.send_raw(":" + user.nickname + " NOTICE " + target.nickname + " " + msg);
			} catch (Server::UserNotFound &) {}
		}

	}
}