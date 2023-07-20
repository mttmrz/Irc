#include "irc.hpp"

string find_usrs(sbase *base, string channel)
{
	string users;

	vector<string>::iterator i = base->ch.chlist[channel]->usrs.begin();
	i = base->ch.chlist[channel]->usrs.begin();
	while (i != base->ch.chlist[channel]->usrs.end())
	{ 
		for (deque<string>::iterator it = base->ch.chlist[channel]->admins.begin(); it != base->ch.chlist[channel]->admins.end(); it++)
		{
			if (*it == *i)
			{
				users += "@";
				break ;
			}
		}
		users += *i;
		users += " ";
		i++;
	}
	if (users[0] != '\0')
		users.erase(users.length() - 1, users.length());
	return (users);
}

void	find_rec(string tofind, sbase *base, int fd)
{
	string	reciver;
	string	message;
	int		recfd = -1;
	char	prova[1024];
	string	nsender;
	memset(prova, 0, 1024);

	if (tofind[0] == ',')
		tofind.erase(0, 1);
	if (multiple_rec(tofind) == -1)
		reciver.assign(tofind, 0, tofind.find(" "));
	else
		reciver.assign(tofind, 0, tofind.find(","));
	tofind.erase(0, reciver.length());
	nsender = base->usr.ppl[fd]->getNick();
	std::map<int, User *>::iterator it = base->usr.ppl.begin();
	for (size_t i = 0; i < base->usr.ppl.size();i++)
	{
		if (it->second->getNick() == reciver)
			recfd = it->first;
		it++;
	}
	message.assign(tofind, tofind.find(":"), tofind.length());
	if (ft_safe_words(message, fd, base) == -1)
		return ;
	nsender = ":" + nsender + " PRIVMSG " + message + "\r\n";
	if (recfd > 0)
		send(recfd, nsender.c_str(), nsender.length(), 0);
	if (multiple_rec(tofind) == 0)
		find_rec(tofind, base, fd);
}

int isIn(sbase *base, int fd)
{
	vector<int>::iterator it = base->connected.begin();
	while (it != base->connected.end())
	{
		if (*it == fd)
			return (1);
		it++;
	}
	return (0);
}

int isReg(sbase *base, int fd)
{
	vector<int>::iterator it = base->registered.begin();
	for (; it != base->registered.end(); it++)
	{
		if (*it ==  fd)
			return (1);
	}
	return (0);
}

int isNick(sbase *base, int fd)
{
	vector<int>::iterator it = base->nicked.begin();
	for (; it != base->nicked.end(); it++)
	{
		if (*it ==  fd)
			return (1);
	}
	return (0);
}
