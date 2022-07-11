#include "Commands.hpp"
#include "Server.hpp"

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
	if (!user.isOnline()) {
		user.send_srv_msg("451", ":You are not authenticated");
		return;
	}
	if (args.size() < 2) {
		user.send_srv_msg("461", ":Channel name is missing");
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

			if (chan.isBanned(user.getId())) {
				user.send_srv_msg("474", ":You are banned from this channel");
				return;
			}
			if (chan.isPrivate() && !chan.isInvited(user.getId())) {
				user.send_srv_msg("473", ":You are not invited to this channel");
				return;
			}
			if (!chan.isPrivate() || (chan.isPrivate() && chan.isInvited(user.getId()))) {
				chan.addMember(user.getId());
				if (chan.isPrivate())
					chan.uninviteUser(user.getId());
				user.send_raw(":" + user.getId() + " JOIN " + chan.getId());
				user.send_msg(chan.getId() + ": " + chan.getTopic());

				std::string	lst_str = ":";
				Channel::id_vector lst = chan.getMembers();
				for (std::vector<std::string>::iterator ite = lst.begin(); ite != lst.end(); ++ite) {
					lst_str += *ite;
					if (ite != lst.end() - 1)
						lst_str += " ";
				}
				user.send_srv_msg("JOIN 353", lst_str);
			}
		} catch (Server::ChannelNotFound &) {
			if (it->size() != 0 && (*it)[0] != '#') {
				user.send_srv_msg("403", ":Channel name must start with a '#'");
				return;
			}
			if (std::count(it->begin(), it->end(), '#') != 1) {
				user.send_srv_msg("403", ":Channel name cannot have more than one '#'");
				return;
			}
			try {
				Server::getInstance().addChannel(user, *it);
				Channel &chan = Server::getInstance().getChannel(*it);
				user.send_raw(":" + user.getId() + " JOIN " + chan.getId());
				user.send_msg(chan.getId() + ": " + chan.getTopic());

				std::string	lst_str = ":";
				Channel::id_vector lst = chan.getMembers();
				for (std::vector<std::string>::iterator ite = lst.begin(); ite != lst.end(); ++ite) {
					lst_str += *ite;
					if (ite != lst.end() - 1)
						lst_str += " ";
				}
				user.send_srv_msg("JOIN 353", lst_str);
			} catch (Server::ChannelNotFound &) {}
		}
	}
}