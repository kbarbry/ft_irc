#include "Commands.hpp"

// ERR_NEEDMOREPARAMS	461
// ERR_NOSUCHCHANNEL	403
// ERR_NOTONCHANNEL		442

void PartCommand::run(User &user, std::vector<std::string> &args) {
	if (!user.is_online) {
		user.send_srv_msg("451", ":You haven't registered");
		return;
	}
	if (args.size() < 2) {
		user.send_msg("461 " + user.nickname + " " + args[0] + " :Arguments are invalid");
		return ;
	}
	std::vector<std::string> v;
	std::istringstream ss(args[1]);
	std::string word;

	while(std::getline(ss, word, ','))
		v.push_back(word);
	std::string	reason = "";
	if (args.size() > 2) {
		if (args[2][0] == ':')
			args[2].replace(0, 1, "");
		for (std::vector<std::string>::iterator	it = args.begin() + 2; it != args.end(); it++)
			reason += *it;
	}
	else
		reason = "No reason specified";
	for (std::vector<std::string>::iterator it = v.begin(); it != v.end(); ++it) {
		try {
			Channel &chan = Server::getInstance().getChannel(*it);

			if (!chan.isMember(user.nickname)) {
				user.send_msg("442 " + user.nickname + " " + args[1] + " :You are not in this channel");
				return;
			}
			Channel::id_vector mbrs = chan.members;
			for (Channel::id_vector::iterator ite = mbrs.begin(); ite != mbrs.end(); ++ite) {
				try {
					User &target = Server::getInstance().getUserByUsername(*ite);

					target.send_raw(":" + user.nickname + " PART " + chan.name + " :" + reason);
				}
				catch(const Server::UserNotFound &) { }
			}
			chan.removeMember(user.nickname);
		} catch (Server::ChannelNotFound &) {
			user.send_msg("403 " + user.nickname + " " + *it + " :Channel not found");
			return ;
		}
	}
}