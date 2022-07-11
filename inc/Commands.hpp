#pragma once

#include "ICommand.hpp"
#include "SocketManager.hpp"
#include "Server.hpp"
#include <vector>
#include <sstream>

// Auth Commands
class PassCommand: public ICommand {
public:
	void	run(User &user, std::vector<std::string> &args);
};

class UserCommand: public ICommand {
public:
	void	run(User &user, std::vector<std::string> &args);
};

// Oper Commands
class InviteCommand: public ICommand {
public:
	void	run(User &user, std::vector<std::string> &args);
};

class KickCommand: public ICommand {
public:
	void	run(User &user, std::vector<std::string> &args);
};

class TopicCommand: public ICommand {
public:
	void	run(User &user, std::vector<std::string> &args);
};

class ModeCommand: public ICommand {
private:
	static	std::string modes;
public:
	void	run(User &user, std::vector<std::string> &args);
};

// Basic Commands
class NickCommand: public ICommand {
public:
	void	run(User &user, std::vector<std::string> &args);
};

class OperCommand: public ICommand {
public:
	void	run(User &user, std::vector<std::string> &args);
};

class QuitCommand: public ICommand {
public:
	void	run(User &user, std::vector<std::string> &args);
};

class JoinCommand: public ICommand {
public:
	void	run(User &user, std::vector<std::string> &args);
};

class PartCommand: public ICommand {
public:
	void	run(User &user, std::vector<std::string> &args);
};

class PrivmsgCommand: public ICommand {
public:
	void	run(User &user, std::vector<std::string> &args);
};

class NoticeCommand: public ICommand {
public:
	void	run(User &user, std::vector<std::string> &args);
};

class PingCommand: public ICommand {
public:
	void	run(User &user, std::vector<std::string> &args);
};

class PongCommand: public ICommand {
public:
	void	run(User &user, std::vector<std::string> &args);
};
