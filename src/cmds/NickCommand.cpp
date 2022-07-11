#include "Commands.hpp"

void NickCommand::run(User &user, std::vector<std::string> &args) {
	if (args.size() != 2) {
		user.send_srv_msg("461", ":Arguments are invalid");
		return ;
	}
	user.setNickname(args[1]);
}