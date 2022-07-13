#include "Commands.hpp"

// ERR_NOSUCHCHANNEL	403
// ERR_TOOMANYCHANNELS	405
// ERR_NEEDMOREPARAMS	461
// ERR_CHANNELISFULL	471
// ERR_INVITEONLYCHAN	473
// ERR_BANNEDFROMCHAN	474
// ERR_BADCHANNELKEY	475
// RPL_TOPIC 			332
// RPL_TOPICWHOTIME		333
// RPL_NAMREPLY			353
// RPL_ENDOFNAMES		366

void JoinCommand::run(User &user, std::vector<std::string> &args) {
	if (!user.is_online) {
		user.send_srv_msg("451", ":You are not authenticated");
		return;
	}
	if (args.size() < 2) {
		user.send_msg("461 " + user.nickname + " " + args[0] + " :Arguments are invalid");
		return ;
	}
	std::vector<std::string> v;
	std::istringstream ss(args[1]);
	std::string word;

	while(std::getline(ss, word, ',')) {
		v.push_back(word);
	}
	for (std::vector<std::string>::iterator it = v.begin(); it != v.end(); ++it) {
		try {
			Channel &chan = Server::getInstance().getChannel(*it);

			if (chan.is_private && !chan.isInvited(user.nickname)) {
				user.send_msg("473 " + user.nickname + " " + args[1] + " :You are not invited to this channel");
				return;
			}
			if (!chan.is_private || (chan.is_private && chan.isInvited(user.nickname))) {
				chan.members.push_back(user.username);
				if (chan.is_private)
					chan.uninviteUser(user.nickname);

				std::string	lst_str = "";
				Channel::id_vector lst = chan.members;
				for (std::vector<std::string>::iterator ite = lst.begin(); ite != lst.end(); ++ite) {
					lst_str += Server::getInstance().getUserByUsername(*ite).nickname;
					if (ite != lst.end() - 1)
						lst_str += " ";
				}
				chan.send_raw(":" + user.nickname + " JOIN " + chan.name);
				chan.send_msg_srv("353 " + user.nickname + " = " + chan.name + " :" + lst_str);
				chan.send_msg_srv("366 " + user.nickname + " " + chan.name + " :End of /NAMES list.");
			}
		} catch (Server::ChannelNotFound &) {
			if (it->size() != 0 && (*it)[0] != '#') {
				user.send_msg("403 " + user.nickname + " " + args[1] + " :Channel name must start with a '#'");
				return;
			}
			if (std::count(it->begin(), it->end(), '#') != 1) {
				user.send_msg("403 " + user.nickname + " " + args[1] + " :Channel name cannot have more than one '#'");
				return;
			}
			try {
				Server::getInstance().addChannel(user, *it);
				Channel &chan = Server::getInstance().getChannel(*it);

				std::string	lst_str = "";
				Channel::id_vector lst = chan.members;
				for (std::vector<std::string>::iterator ite = lst.begin(); ite != lst.end(); ++ite) {
					lst_str += Server::getInstance().getUserByUsername(*ite).nickname;
					if (ite != lst.end() - 1)
						lst_str += " ";
				}
				chan.send_raw(":" + user.nickname + " JOIN " + chan.name);
				chan.send_msg_srv("353 " + user.nickname + " = " + chan.name + " :" + lst_str);
				chan.send_msg_srv("366 " + user.nickname + " " + chan.name + " :End of /NAMES list.");

			} catch (Server::ChannelNotFound &) {}
		}
	}
}