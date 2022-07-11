#include <iterator>
#include <sstream>
#include "Server.hpp"

int Server::max_clients = 50;

Server::Server(): _port(), _password() {}
Server::~Server(void) {}

void Server::configure(const uint16_t &port, const std::string &password) {
	if (_port && !_password.empty())
		throw AlreadyConfigured();
	if (port < 1024 || port == std::numeric_limits<uint16_t>::max())
		throw InvalidPort();
	if (password.empty() || password.length() > 16 || password.length() < 4)
		throw InvalidPassword();
	// Add an OP account in the database
	addOperator("admin", "admin");
	addOperator("Rick", "Astley");
	_port = port;
	_password = password;
}

void Server::start(void) {
	init_cmds();

	SocketManager &sm = SocketManager::getInstance();

	try {
		sm.init(_port);
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

	std::cout << "Server started on port " << _port << "." << std::endl;

	try {
		sm.runtime();
	} catch (SocketManager::ConnectionFailure &) {
		throw StartFailure();
	}
}

void	Server::send_msg_to_all(const std::string &msg) {
	for (user_map::iterator	it = _users.begin(); it != _users.end(); it++)
		it->second.send_msg(msg);
}

User &Server::getStoredUser(const std::string &id) {
	user_map::iterator	res = _users.find(id);

	if (res == _users.end())
		throw UserNotFound();
	res->second.setLastConnexion(std::time(NULL));
	return res->second;
}

User		&Server::getUser(int socket_fd) {
	temp_user_map::iterator res = _temp_users.find(socket_fd);

	if (res == _temp_users.end())
		throw UserNotFound();
	return res->second;
}

const std::string	&Server::getOperator(const std::string &op_name) {
	operator_map::iterator res = _operators.find(op_name);

	if (res == _operators.end())
		throw OperatorNotFound();
	return res->second;
}

Channel	&Server::getChannel(const std::string &id) {
	channel_map::iterator res = _channels.find(id);

	if (res == _channels.end())
		throw ChannelNotFound();
	return res->second;
}

const std::string &Server::getPassword() {
	return _password;
}

void Server::init_cmds(void) {
	_commands.insert(std::make_pair("INVITE", new InviteCommand));
	_commands.insert(std::make_pair("JOIN", new JoinCommand));
	_commands.insert(std::make_pair("KICK", new KickCommand));
	_commands.insert(std::make_pair("MODE", new ModeCommand));
	_commands.insert(std::make_pair("NICK", new NickCommand));
	_commands.insert(std::make_pair("NOTICE", new NoticeCommand));
	_commands.insert(std::make_pair("OPER", new OperCommand));
	_commands.insert(std::make_pair("PART", new PartCommand));
	_commands.insert(std::make_pair("PASS", new PassCommand));
	_commands.insert(std::make_pair("PING", new PingCommand));
	_commands.insert(std::make_pair("PRIVMSG", new PrivmsgCommand));
	_commands.insert(std::make_pair("QUIT", new QuitCommand));
	_commands.insert(std::make_pair("TOPIC", new TopicCommand));
	_commands.insert(std::make_pair("USER", new UserCommand));
}

void Server::check_cmd(const std::string &cmd, User &runner) {
	runner.command.append(cmd);
	if (cmd.find("\n") == cmd.npos) {
		return;
	}

	while (runner.command.find('\r') != runner.command.npos)
		runner.command.erase(runner.command.find('\r'));

	// for (size_t i = 0; i < runner.command.length(); ++i) {
	// 	if (std::isalnum(runner.command[i]) || runner.command[i] == '\n')
	// 		std::cout << runner.command[i];
	// 	else
	// 		std::cout << "\033[0;32m%" << std::hex << int(runner.command[i]) << "\033[0;0m";
	// }
	// std::cout << std::endl;
	
	std::stringstream ss(runner.command);
	std::string line;
	while (std::getline(ss, line, '\n')) {
		if (!line.empty())
		{
			std::cout << "\033[0;32m--> " << line << "\033[0;0m [" << line.length() << "]" << std::endl;
			run_cmd(line, runner);
		}
		std::cout << std::endl;
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
	command_map::iterator res = _commands.find(args[0]);
	if (res != _commands.end())
		res->second->run(runner, args);
}

void	Server::addUser(int socket_fd) {
	_temp_users.insert(std::make_pair(socket_fd, User(socket_fd)));
}

void 	Server::addChannel(User &user, const std::string &channel_name) {
	_channels.insert(std::make_pair(channel_name, Channel(channel_name, user.getId())));
}

void 	Server::addChannel(const std::string &channel_name) {
	_channels.insert(std::make_pair(channel_name, Channel(channel_name)));
}

void	Server::addOperator(const std::string &op_name, const std::string &op_password) {
	_operators.insert(std::make_pair(op_name, op_password));
}

void Server::storeUser(int socket_fd) {
	temp_user_map::iterator it = _temp_users.find(socket_fd);

	if (it == _temp_users.end())
		return;

	User	user(it->second);

	if (user.isOnline()) {
		user_map::iterator usr = _users.find(user.getId());

		user.setLastConnexion(std::time(NULL)).setIsAuth(false).setOnline(false).setIsOperator(false).setSocketFd(-1);
		user.command = "";
		user.should_disconnect = false;
		if (usr == _users.end())
			_users.insert(std::make_pair(user.getId(), user));
		else
			usr->second = user;
	}
	_temp_users.erase(it);
}

bool Server::isOnline(const std::string &id) {
	for (temp_user_map::iterator it = _temp_users.begin(); it != _temp_users.end(); ++it) {
		if (it->second.getId() == id) {
			return true;
		}
	}
	return false;
}

User &Server::getUser(const std::string &id) {
	for (temp_user_map::iterator it = _temp_users.begin(); it != _temp_users.end(); ++it) {
		if (it->second.getId() == id)
			return it->second;
	}
	throw UserNotFound();
}
