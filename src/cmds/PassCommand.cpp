#include "Commands.hpp"
#include "Server.hpp"

// ERR_NEEDMOREPARAMS		461
// ERR_ALREADYREGISTERED	462
// ERR_PASSWDMISMATCH		464

void PassCommand::run(User &user, std::vector<std::string> &args) {
	if (user.is_auth) {
		user.send_srv_msg("462", ":You may not reregister");
		return;
	}
	if (args.size() != 2) {
		user.send_msg("461 " + user.nickname + " " + args[0] + " :Arguments are invalid");
		return;
	}
	if (Server::getInstance().password != args[1]) {
		user.send_srv_msg("464", ":Wrong password");
		return;
	}
	user.is_auth = true;
}
