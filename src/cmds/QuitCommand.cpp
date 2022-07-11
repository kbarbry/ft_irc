#include "Commands.hpp"
#include "Server.hpp"

void QuitCommand::run(User &user, std::vector<std::string> &args) {
	std::string reason = "";
	if (args.size() < 2)
		reason = "Unknown reason";
	else {
		for (std::vector<std::string>::iterator it = args.begin() + 1;
			 it != args.end(); ++it) {
			reason += *it;
			if (it != args.end() - 1)
				reason += " ";
		}
	}
	user.disconnect(reason);
}