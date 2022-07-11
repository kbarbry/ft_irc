#include "Commands.hpp"

void PongCommand::run(User &user, std::vector<std::string> &args) {
	(void) args;
	user.pong_received = true;
	user.last_ping = std::time(NULL);
}
