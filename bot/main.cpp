#include "xx_BotPifPafPoufProMaxExtreme_xx.hpp"

int main(int ac, char **av) {
	if (ac < 3)
	{
		std::cout << "\033[1;31mUsage: ./ircbot <port> <password>.\033[0;0m" << std::endl;
		return 1;
	}
	uint16_t	port = std::atoi(std::string(av[1]).c_str());
	std::string	password = av[2];

	Xx_BotPifPafPoufProMaxExtreme_xX bot("127.0.0.1", port, password, "XxbotxX");
	try {
		bot.run();
	} catch (std::exception &) {
		std::cout << "\033[1;31mSomething happened.\033[0;0m" << std::endl;
		return 1;
	}
	return 0;
}