#include "Server.hpp"

int Server::max_clients = 50;

Server::Server(): port(0), password("") {}
Server::~Server(void) {}

void Server::configure(const uint16_t &port, const std::string &password) {
	if (this->port && !this->password.empty())
		throw AlreadyConfigured();
	if (port < 1024 || port == std::numeric_limits<uint16_t>::max())
		throw InvalidPort();
	if (password.empty() || password.length() > 16 || password.length() < 4)
		throw InvalidPassword();
	addOperator("admin", "admin");
	addOperator("Rick", "Astley");
	this->port = port;
	this->password = password;
}

void Server::start(void) {
	init_cmds();

	SocketManager &sm = SocketManager::getInstance();

	try {
		sm.init(port);
	} catch (SocketManager::SocketFailure &) {
		std::cerr << "Failed to create the socket." << std::endl;
		throw StartFailure();
	} catch (SocketManager::SocketOptFailure &) {
		std::cerr << "Failed to configure socket." << std::endl;
		throw StartFailure();
	} catch (SocketManager::FcntlFailure &) {
		std::cerr << "Failed to configure socket file descriptor." << std::endl;
		throw StartFailure();
	} catch (SocketManager::BindFailure &) {
		std::cerr << "Failed to bind port." << std::endl;
		throw StartFailure();
	} catch (SocketManager::ListenFailure &) {
		std::cerr << "Failed to listen to port." << std::endl;
		throw StartFailure();
	}

	std::cout << "Server started on port " << port << "." << std::endl;

	try {
		sm.runtime();
	} catch (SocketManager::ConnectionFailure &) {
		throw StartFailure();
	}
}

User		&Server::getUser(int socket_fd) {
	user_map::iterator res = users.find(socket_fd);

	if (res == users.end())
		throw UserNotFound();
	return res->second;
}

const std::string	&Server::getOperator(const std::string &op_name) {
	operator_map::iterator res = operators.find(op_name);

	if (res == operators.end())
		throw OperatorNotFound();
	return res->second;
}

Channel	&Server::getChannel(const std::string &id) {
	channel_map::iterator res = channels.find(id);

	if (res == channels.end())
		throw ChannelNotFound();
	return res->second;
}


void Server::init_cmds(void) {
	commands.insert(std::make_pair("INVITE", new InviteCommand));
	commands.insert(std::make_pair("JOIN", new JoinCommand));
	commands.insert(std::make_pair("KICK", new KickCommand));
	commands.insert(std::make_pair("MODE", new ModeCommand));
	commands.insert(std::make_pair("NICK", new NickCommand));
	commands.insert(std::make_pair("NOTICE", new NoticeCommand));
	commands.insert(std::make_pair("OPER", new OperCommand));
	commands.insert(std::make_pair("PART", new PartCommand));
	commands.insert(std::make_pair("PASS", new PassCommand));
	commands.insert(std::make_pair("PING", new PingCommand));
	commands.insert(std::make_pair("PONG", new PongCommand));
	commands.insert(std::make_pair("PRIVMSG", new PrivmsgCommand));
	commands.insert(std::make_pair("QUIT", new QuitCommand));
	commands.insert(std::make_pair("TOPIC", new TopicCommand));
	commands.insert(std::make_pair("USER", new UserCommand));
}

void Server::check_cmd(const std::string &cmd, User &runner) {
	runner.command.append(cmd);
	if (cmd.find("\n") == cmd.npos) {
		return;
	}

	while (runner.command.find('\r') != runner.command.npos)
		runner.command.erase(runner.command.find('\r'));
	
	std::stringstream ss(runner.command);
	std::string line;
	while (std::getline(ss, line, '\n')) {
		if (!line.empty())
		{
			std::cout << "\033[0;32m--> " << line << "\033[0;0m" << std::endl;
			run_cmd(line, runner);
		}
		runner.command.erase(0, line.length() + 1);
	}
}

void Server::run_cmd(const std::string &txt, User &runner) {
	std::stringstream ss(txt);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> args(begin, end);

	if (args.size() == 0)
		return;
	command_map::iterator res = commands.find(args[0]);
	if (res != commands.end())
		res->second->run(runner, args);
}

void	Server::addUser(int socket_fd) {
	users.insert(std::make_pair(socket_fd, User(socket_fd)));
}

void 	Server::addChannel(User &user, const std::string &channel_name) {
	channels.insert(std::make_pair(channel_name, Channel(channel_name, user.username)));
}

void 	Server::addChannel(const std::string &channel_name) {
	channels.insert(std::make_pair(channel_name, Channel(channel_name)));
}

void	Server::addOperator(const std::string &op_name, const std::string &oppassword) {
	operators.insert(std::make_pair(op_name, oppassword));
}

bool Server::isOnline(const std::string &nickname) {
	for (user_map::iterator it = users.begin(); it != users.end(); ++it) {
		if (it->second.nickname == nickname) {
			return true;
		}
	}
	return false;
}

User &Server::getUserByNickname(const std::string &nickname) {
	for (user_map::iterator it = users.begin(); it != users.end(); ++it) {
		if (it->second.nickname == nickname)
			return it->second;
	}
	throw UserNotFound();
}

User &Server::getUserByUsername(const std::string &username) {
	for (user_map::iterator it = users.begin(); it != users.end(); ++it) {
		if (it->second.username == username)
			return it->second;
	}
	throw UserNotFound();
}
