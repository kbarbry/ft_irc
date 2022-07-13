#include "Commands.hpp"

// PRIVMSG #nul:localhost :message
// PRIVMSG remi :message

// ERR_NOSUCHNICK		401
// ERR_NOSUCHSERVER		402
// ERR_CANNOTSENDTOCHAN	404
// ERR_TOOMANYTARGETS	407
// ERR_NORECIPIENT		411
// ERR_NOTEXTTOSEND		412
// ERR_NOTOPLEVEL		413
// ERR_WILDTOPLEVEL		414
// RPL_AWAY				301

void PrivmsgCommand::run(User &user, std::vector<std::string> &args) {
	if (!user.is_online) {
		user.send_srv_msg("451", ":You haven't registered");
		return;
	}
	if (args.size() == 1) {
		user.send_msg("401" + user.nickname + " " + user.nickname + " :No target to send the message");
		return;
	}
	if (args.size() == 2) {
		user.send_msg("401" + user.nickname + " " + args[1] + " :No message to send");
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
				user.send_msg("404 " + user.nickname + " " + args[1] + " :You are not in this channel");
					return;
				}
				Channel::id_vector lst = chan.members;
				for (std::vector<std::string>::iterator ite = lst.begin(); ite != lst.end(); ++ite) {
					try {
						User &target = Server::getInstance().getUserByUsername(*ite);

						if (target.nickname == user.nickname)
							continue;

						target.send_raw(":" + user.nickname + " PRIVMSG " + chan.name + " " + msg);
					} catch (Server::UserNotFound &) {}
				}
			} catch (Server::ChannelNotFound &) {
				user.send_msg("401" + user.nickname + " " + args[1] + " :No target to send the message");
				return;
			}
		} else {
			try {
				User &target = Server::getInstance().getUserByNickname(chan_name);

				target.send_raw(":" + user.nickname + " PRIVMSG " + target.nickname + " " + msg);
			} catch (Server::UserNotFound &) {
				user.send_msg("301" + user.nickname + " " + args[1] + " :User is offline or does not exist");
			}
		}

	}
}