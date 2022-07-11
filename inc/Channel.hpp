#pragma once

#include <vector>
#include <string>
#include <algorithm>

class Channel
{
public:
	typedef std::vector<std::string>	id_vector;

	std::string			name;
	std::string			owner;
	std::string			topic;
	bool				is_private;

	id_vector			operators;
	id_vector			members;
	id_vector			invited;

public:
	Channel(const std::string &id, const std::string &owner);
	Channel(const std::string &id);
	~Channel(void);

	void	send_msg_srv(const std::string &msg);
	void	send_raw(const std::string &msg);

	void	uninviteUser(const std::string &id);
	void	removeMember(const std::string &id);
	void	removeOperator(const std::string &id);

	bool	isOperator(const std::string &id)	const;
	bool	isMember(const std::string &id)		const;
	bool	isInvited(const std::string &id)	const;

};
