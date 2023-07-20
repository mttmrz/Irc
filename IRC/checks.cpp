#include "irc.hpp"

int	kickCheck(sbase *base, int fd, string channel)
{
	string res;
	if (base->ch.chlist.find(channel) == base->ch.chlist.end())
		return (1);
	vector<int>::iterator it = base->ch.chlist[channel]->usrs_fds.begin();
	if (it == base->ch.chlist[channel]->usrs_fds.end())
		return (1);
	for (; it != base->ch.chlist[channel]->usrs_fds.end(); it++)
	{
		if (*it == fd)
			return (0);
	}
	res = "IRcServ " + base->usr.ppl[fd]->getNick() + " is not connected to the channel.\n";
	it = base->ch.chlist[channel]->usrs_fds.begin();
		send(fd, res.c_str(), res.length(), 0);
	return (1);

}

int checkBuffer(char *buffer, int fd, sbase *base)
{
	string full;
	full = buffer;
	istringstream ss(full);
	string trash;
	string res;
	while (ss)
	{
		ss >> trash;
		if (trash.find("PASS") != string::npos)
		{
			ss >> trash;
			if (trash.find(":") == 0)
				trash.erase(0, 1);
			if (trash == base->password)
			{
				if (isIn(base, fd) == 0)
					base->connected.push_back(fd);
			}
			else
			{
				res = "Invalid password, leaving the server...\n";
				send(fd, res.c_str(), res.length(), 0);
				close(fd);
				check = 3;
				return (1);
			}
		}
		if (trash.find("NICK") != string::npos)
		{
			ss >> trash;
			base->nick = trash;
			getTime(base);
			res =  ":IRCserv NOTICE Auth :Welcome to IRCSRV!\r\n";
			send(fd, res.c_str(), res.length(), 0);
			res =  ":IRC 001 " + trash + " :Welcome To IRCSERV " + trash + "!\n";
			send(fd, res.c_str(), res.length(), 0);
			res = ":002 IRCserv Server version: v.7.0\n";
			send(fd, res.c_str(), res.length(), 0);
			res = ":003 IRcserv You joined on :" + base->time + "\n";
			send(fd, res.c_str(), res.length(), 0);
			res = base->rules + "\n";
			send(fd, res.c_str(), res.length(), 0);
			if (isNick(base, fd) == 0)
				base->nicked.push_back(fd);
		}
		if (trash.find("USER") != string::npos)
		{
			while (ss)
			{
				ss >> trash;
				if (trash.find(":") != string::npos)
					break ;
			}
			trash.erase(0, 1);
			string tmp;
			ss >> tmp;
			trash = trash + " " + tmp;
			base->ussr = trash;
			if (isReg(base, fd) == 0)
				base->registered.push_back(fd);	
		}
	}
	return (0);
	
}

int	ft_check_ar(char **av)
{
	for (int i = 0; av[1][i]; i++)
		if (isdigit(av[1][i]) == 0)
			return (1);
	return (0);
}

int	ft_checkNick(std::string toCheck, int fd, sbase *base)
{
	if (toCheck.c_str() == NULL)
		return (-1);
	if (ft_safe_nicks(toCheck, fd, base) == -1)
		return (-1);
	std::map<int, User *>::iterator it = base->usr.ppl.begin();
	while (it != base->usr.ppl.end())
	{
		if (it->first != fd && it->second->getNick() == toCheck)
		{
			send(fd, "Nickname already in use, choose another one!\r\n" , 47, 0);
			return (-1);
		}
		it++;
	}
	return (0);
}


int		ft_check_chmember(sbase *base, string channel, string name, int fd)
{
	if (base->ch.chlist.find(channel) == base->ch.chlist.end())
		return (0);
	vector<string>::iterator it = base->ch.chlist[channel]->usrs.begin();
	while (it != base->ch.chlist[channel]->usrs.end())
	{
		if (*it == name)
        {
            send(fd, "IRC User already connected\n", 27, 0);
			return (0);
        }
		it++;
	}
	return (-1);
}

int ft_checkch(sbase *base, string tofind)
{
	map<string, Channel *>::iterator it = base->ch.chlist.find(tofind);
	if (it != base->ch.chlist.end())
		return (1); 
	return (0);
}

int	ft_checkchUsr(sbase *base, int fd, string cn)
{
	map<string, Channel *>::iterator it = base->ch.chlist.begin();

	while (it != base->ch.chlist.end())
	{
		if (it->first == cn)
		{
			vector<string>::iterator k = base->ch.chlist[cn]->usrs.begin();
			while (k != base->ch.chlist[cn]->usrs.end())
			{
				if (*k == base->usr.ppl[fd]->getNick())
					return (-1);
			k++;
			}
		}
		it++;
	}
	if (base->ch.chlist[cn]->usrs.empty())
		base->ch.chlist[cn]->usrs.clear();
	base->ch.chlist[cn]->usrs.push_back(base->usr.ppl[fd]->getNick());
	return (0); 
}