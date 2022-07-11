#pragma once

#include <vector>
#include <string>
#include <algorithm>

class Channel
{
public:
	typedef std::vector<std::string>	id_vector;

private:
	std::string			_id;
	std::string			_owner;
	std::string			_topic;
	bool				_private;

private:
	id_vector			_operators;
	id_vector			_members;
	id_vector			_banned;
	id_vector			_invited;

public:
	Channel(const std::string &id, const std::string &owner);
	Channel(const std::string &id);
	~Channel(void);

	const std::string	&getId(void)			const;
	const std::string	&getOwner(void)			const;
	const std::string	&getTopic(void)			const;
	const id_vector		&getOperators(void)		const;
	const id_vector		&getMembers(void)		const;
	const id_vector		&getBanned(void)		const;
	const id_vector		&getInvited(void)		const;

	void	addMember(const std::string &id);
	void	addOperator(const std::string &id);
	void	banUser(const std::string &id);
	void	inviteUser(const std::string &id);
	void	setTopic(const std::string &topic);
	void	setPrivate(bool is_private);

	void	unbanUser(const std::string &id);
	void	uninviteUser(const std::string &id);
	void	removeMember(const std::string &id);
	void	removeOperator(const std::string &id);

	bool	isOperator(const std::string &id)	const;
	bool	isMember(const std::string &id)		const;
	bool	isBanned(const std::string &id)		const;
	bool	isInvited(const std::string &id)	const;
	bool	isPrivate(void)						const;
};
