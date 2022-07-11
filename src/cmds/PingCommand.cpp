#include "Commands.hpp"

void PingCommand::run(User &user, std::vector<std::string> &args) {
	if (!user.is_online) {
		user.send_srv_msg("451", ":You are not authenticated");
		return;
	}
	if (args.size() < 2 || (args[1] != "localhost" && args[1] != "127.0.0.1") ) {
		user.send_srv_msg("402", ":No such server");
		return ;
	}
	user.send_msg("PONG " + args[1]);
}
