#pragma once

#include <string>
#include <vector>
#include "User.hpp"

class ICommand
{
public:
	ICommand(void) {}
	virtual ~ICommand(void) {}

	virtual void	run(User &user, std::vector<std::string> &args) = 0;
};
