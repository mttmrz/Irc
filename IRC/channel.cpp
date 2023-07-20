/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarzano <mmarzano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 15:37:43 by bmuni             #+#    #+#             */
/*   Updated: 2023/06/07 18:54:29 by mmarzano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	ft_create_ch(sbase *base, string name, int fd)
{
	Channel *a = new Channel();

	a->setName(name);
	a->setInvite(false);
	a->setKey(false);
	a->setTopic("Welcome to the channel " + name + "!");
	a->setMax(false);
	a->setPriv(false);
	a->setRtopic(false);
	a->setNMax(10000);
	base->ch.chlist.insert(make_pair(name, a));
	base->ch.chlist[name]->admins.push_back(base->usr.ppl[fd]->getNick());
	base->usr.ppl[fd]->setAdmin(true);
}

int ft_alreadyINch(sbase *base, int fd, string cn, string res)
{
	string sName = "IRcserv";
	string users = find_usrs(base, cn);
	vector<int>::iterator it = base->ch.chlist[cn]->usrs_fds.begin();
	while (it != base->ch.chlist[cn]->usrs_fds.end())
	{
		if (*it != fd)
		{
			res = ":" + base->usr.ppl[fd]->getNick() +  "!" + " JOIN :" + cn + "\n";
			send(*it, res.c_str(), res.length(), 0);
		}
		it++;
	}
	base->ch.chlist[cn]->usrs_fds.push_back(fd);
	return (0);
}

void	ft_refreshCH(sbase *base, string full, int fd)
{
	string users;
	string sName = "IRCserv";
	string channel;
	string res;
	
	channel = full;
	(void)fd;
	users = find_usrs(base, channel);
	ft_alreadyINch(base, fd, channel, res);
}

int notInvited(sbase *base, int fd, string full)
{
	istringstream ss(full);
	string channel;
	
	ss >> channel;
	map<int, User *>::iterator i = base->usr.ppl.find(fd);
	deque<string>::iterator it = base->ch.chlist[channel]->invited.begin();
	for (; it != base->ch.chlist[channel]->invited.end(); it++)
	{
		if (*it == i->second->getNick())
			return (1);
	}
	return (0);
}

int		ft_chModes(sbase *base, string channel, int fd)
{
	string	key;
	string 	invite;
	map<int, User *>::iterator ps = base->usr.ppl.find(fd);
	invite = ps->second->getNick();
	map<string, Channel *>::iterator it = base->ch.chlist.find(channel);
	if (it->second->getInvite() == true)
	{
		std::deque<std::string>::iterator result = find(it->second->getInvited().begin(), it->second->getInvited().end(), invite);
		if (result == it->second->getInvited().end())
		{
			send(fd, "channel You are not invited!\r\n", 30, 0);
			return (-1);
		}
	}
	if (it->second->getKey() == true && notInvited(base, fd, channel) == 0)
	{
		if (channel.find(" ") != string::npos)
			channel.erase(0, channel.find(" "));
		if (channel.find(" ") != string::npos)
			key = channel.erase(0, channel.find(" "));
		else
			send(fd, "channel You need the password to join!\r\n", 40, 0);
		if (key != it->second->getPass())
		{
			send(fd, "channel Wrong password!\r\n", 25, 0);
			return (-1);
		}
	}
	if (it->second->getMax() == true)
	{
		if (static_cast<size_t>(it->second->getNMax()) <= it->second->getUsrs().size())
		{
			send(fd, "channel Channel is full!\r\n", 25, 0);
			return (-1);
		}
	}
	return (0);
}

int	ft_channels(sbase *base, string full, int fd)
{
	string channel;
	string res;
	string users;
	string check;
	string sName = "IRCserv";

	if (full.find("#") == string::npos)
	{
		send(fd, "Wrong input format.\n", 21, 0);
		return (0);
	}
	if (full[0] == ',')
		full.erase(0, 1);
	if (multiple_rec(full) == -1)
		channel.assign(full, full.find("#"), full.size() - 1);
	else
		channel.assign(full, full.find("#"), full.find(","));
	if (full.find(channel) != string::npos)
	{
		full.erase(0, 1);
		full.erase(0, full.find("#"));
	}
	if (ft_checkch(base, channel) == 0)
		ft_create_ch(base, channel, fd);
	else
	{
		if (ft_chModes(base, channel, fd) == -1)
			return (-1);
	}
	if (ft_checkchUsr(base, fd, channel) == -1)
	{	
		res = "IRc " + base->usr.ppl[fd]->getNick() + " already connected!\r\n";
		send(fd, res.c_str(), res.length(), 0);
		return (-1);
	}
 	res = ":" + base->usr.ppl[fd]->getNick() + " JOIN " + channel + "\r\n"; 
	users = "@" + base->bot + " ";
	users += find_usrs(base, channel);
	
	string  msg1 = ":" + sName + " 332 " + base->usr.ppl[fd]->getNick() + " " + channel + " :" + base->ch.chlist[channel]->getTopic() + "\r\n";
 	std::string msg2 = ":" + sName + " 353 " + base->usr.ppl[fd]->getNick() + " = " + channel + " :" + users  + "\r\n";
	std::string msg3 = ":" + sName + " 366 " + base->usr.ppl[fd]->getNick() + " " + channel + " :End of /NAMES list\r\n";  					

	send(fd, res.c_str(), res.length(), 0); 
	send(fd, msg1.c_str(), msg1.length(), 0);
 	send(fd, msg2.c_str(), msg2.length(), 0);
 	send(fd, msg3.c_str(), msg3.length(), 0);  
	ft_refreshCH(base, channel, fd);
	if (full.find("#") != string::npos)
		ft_channels(base, full, fd);
	return (0);
}


void sentToCh(sbase *base, string res, string channel)
{
	if (base->ch.chlist.find(channel) == base->ch.chlist.end())
		return ;
	vector<string>::iterator i = base->ch.chlist[channel]->usrs.begin();
	map<int, User *>::iterator it = base->usr.ppl.begin();
	while (it != base->usr.ppl.end())
	{
		if (*i == it->second->getNick())
		{
			send(it->first, res.c_str(), res.length(), 0 );
			it = base->usr.ppl.begin();
			i++;
			if (i == base->ch.chlist[channel]->usrs.end())
				break ;
			continue ;
		}
		it++;
	}
	
}

int ft_messageToChannelCk(string message, sbase *base)
{
	map<string, Channel *>::iterator it = base->ch.chlist.begin();
	while (it != base->ch.chlist.end())
	{
		if (message.find(it->second->getName()) != string::npos)
			return (1);
		it++;
	}
	return (0);
}

int ft_messageCh(string full, sbase *base, int fd)
{
	string channel;
	string message;
	string tmp;
	string resp;
	string tmp2;
	string msg;
	string trash;
	istringstream mm(full);
	
	mm >> trash;
	mm >> channel;
	if (base->ch.chlist.find(channel) == base->ch.chlist.end())
		return (0);
	if (kickCheck(base, fd, channel) == 1)
		return (0);
	if (full.find(":") == string::npos)
		return (0);
	tmp.assign(full, full.find(":") + 1, full.length());
	istringstream ss(tmp);
	while (ss)
	{
		ss >> tmp2;
		resp += tmp2;
		resp += " ";
		tmp2 = "";
	}
	resp.erase(resp.length() - 1, resp.length());
	resp.erase(resp.length() - 1, resp.length());
	message = base->usr.ppl[fd]->getNick() + " " + tmp + "\r\n";
 	msg =  ":" + base->usr.ppl[fd]->getNick() + " PRIVMSG " + channel + " :" + resp + "\r\n";
	vector<string>::iterator it = base->ch.chlist[channel]->usrs.begin();
	map<int, User *>::iterator i = base->usr.ppl.begin();
	while (it != base->ch.chlist[channel]->usrs.end())
	{
		if (it == base->ch.chlist[channel]->usrs.end())
			break ;
		if (i == base->usr.ppl.end())
			break ;
		if (*it == i->second->getNick())
		{
			if (fd != i->first)
				send(i->first, msg.c_str(), msg.length(), 0);
			it++;
			i = base->usr.ppl.begin();
			continue;
	
		}
		i++;
	}
	return (0);
}