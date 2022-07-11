#include "Commands.hpp"
#include "Server.hpp"

// ERR_NEEDMOREPARAMS	461
// ERR_PASSWDISMATCH	464
// ERR_NOOPERHOST		491
// RPL_YOUREOPER		381

void OperCommand::run(User &user, std::vector<std::string> &args) {
	if (!user.isOnline()) {
		user.send_srv_msg("451", ":You are not authenticated");
		return;
	}
	if (args.size() != 3) {
		user.send_srv_msg("461", ":Arguments are invalid");
		return ;
	}
	try {
		std::string	mdp = Server::getInstance().getOperator(args[1]);
		if (mdp != args[2]) {
			user.send_srv_msg("464", ":Incorect password");
			return ;
		}
	} catch (Server::OperatorNotFound &) {
		user.send_srv_msg("464", ":No admin account matching this name");
		return ;
	}
	user.setIsOperator(true);
	user.send_srv_msg("381", ":You are now an operator");
}