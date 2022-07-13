#include "xx_BotPifPafPoufProMaxExtreme_xx.hpp"

Xx_BotPifPafPoufProMaxExtreme_xX::Xx_BotPifPafPoufProMaxExtreme_xX(const std::string &host, int port, const std::string &pass, const std::string &name): _host(host), _port(port), _pass(pass), _name(name) {}
Xx_BotPifPafPoufProMaxExtreme_xX::~Xx_BotPifPafPoufProMaxExtreme_xX() {}

void Xx_BotPifPafPoufProMaxExtreme_xX::run()
{
	int	fd = 0;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd <= 0)
		throw std::exception();

	// Socket settings
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(_port);
	inet_pton(AF_INET, _host.c_str(), &addr.sin_addr);

	if (connect(fd, (sockaddr *) &addr, sizeof(addr)))
	{
		std::cerr << "Could not connect to " << _host << "/" << _port << std::endl;
		throw std::exception();
	}

	std::string msg = "PASS " + _pass + "\nNICK " + _name + "\n" + "USER " + _name + " 0 * :Xx_BotPifPafPoufProMaxExtreme_xX\r\n";
	if (send(fd, msg.c_str(), msg.length() + 1, 0) <= 0) {
		std::cerr << "Could not send msg to " << _host << "/" << _port << std::endl;
		close(fd);
		throw std::exception();
	}

	int response_nbr = 0;
	while (1) {
		char		buf[1024];
		size_t		len = recv(fd, buf, 1024, 0);
		std::string	mess = std::string(buf, 0, len);

		if (!len) {
			continue;
		}
		if (response_nbr++ == 0) {
			if (mess.find("001 " + _name) == mess.npos) {
				std::cerr << "Failed to authenticate to " << _host << "/" << _port << std::endl;
				close(fd);
				throw std::exception();
			}
		}
		std::cout << "\033[0;32m--> " << mess;
		handle_Xx_PifPafPoufProMaxExtreme_xX(fd, mess);
	}
	close(fd);
}

void Xx_BotPifPafPoufProMaxExtreme_xX::handle_Xx_PifPafPoufProMaxExtreme_xX(int fd, const std::string &msg) {
	std::stringstream ss(msg);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> args(begin, end);

	if (args.empty())
		return;
	if (args[0].find(":") != msg.npos)
		args.erase(args.begin()); // on retire le host si il y est
	if (args.size() > 1 && args[0] == "PING")
	{
		std::string res = "PONG " + args[1] + "\r\n";
		send(fd, res.c_str(), res.length(), 0); // send message
		std::cout << "\033[0;31m<-- " << res << "\033[0;0m";
	}
	if (args.size() < 3 || args[0] != "PRIVMSG" || args[1][0] != '#')
		return; // On ne reponds qu'aux messages de channel

	std::string res = "NOTICE " + args[1] + " ";
	size_t start_size = res.length();
	if (msg.find("pif") != msg.npos) {
		res += "paf";
	}
	else if (msg.find("paf") != msg.npos) {
		res += "pouf";
	}
	else if (msg.find("pouf") != msg.npos) {
		res += "pif";
	}
	else if (msg.find("rick") != msg.npos) {
		res += "roll";
	}
	else if (msg.find("help") != msg.npos) {
		res += "(...) but nobody came to help you";
	}
	else if (msg.find("karen") != msg.npos) {
		res += "I love having extra bacon for my 7XL-double-cheese-triple-pickle-specialketchup burger. I really do! I cannot believe adding extra bacon costs more money. You didn’t put enough bacon in my burger! If you did, I wouldn’t be asking for more! I think I deserve to have some extra bacon for free. I’ve been coming for years whereas you started working here since last month. This is unacceptable! I want to speak to the manager now.";
	}
	if (res.length() != start_size) {
		res += "\r\n";
		send(fd, res.c_str(), res.length(), 0); // send message
		std::cout << "\033[0;31m<-- " << res << "\033[0;0m";
	}
}