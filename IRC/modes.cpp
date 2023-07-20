/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modes.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarzano <mmarzano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 15:46:09 by bmuni             #+#    #+#             */
/*   Updated: 2023/06/07 15:11:16 by mmarzano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	send_mode_toall(string message, sbase *base, string channel)
{
	if (base->ch.chlist.find(channel) == base->ch.chlist.end())
		return ;
	for (vector<string>::iterator it = base->ch.chlist[channel]->usrs.begin(); it != base->ch.chlist[channel]->usrs.end(); it++)
	{
		map<int, User *>::iterator i = base->usr.ppl.begin();
		while (i != base->usr.ppl.end())
		{
			if (i->second->getNick() == *it)
			{
				send(i->first, message.c_str(), message.length(), 0);
				break ;
			}
			i++;
		}
	}
}

int	ft_mods_righuse(string message, int fd)
{
	send(fd, message.c_str(), message.length(), 0);
	return (-1);
}

void	ft_itmode(string mode, string channel, sbase *base, int fd)
{	
	if (mode.find("i") != string::npos)
	{
		if (mode[0] == '-')
		{
			base->ch.chlist[channel]->setInvite(false);
			send_mode_toall(":" + base->usr.ppl[fd]->getNick() + " MODE " + channel + " -i \r\n", base, channel);
		}
		else if (mode[0] == '+')
		{
			base->ch.chlist[channel]->setInvite(true);
			send_mode_toall(":" + base->usr.ppl[fd]->getNick() + " MODE " + channel + " +i \r\n", base, channel);
		}
	}
	if (mode.find("t") != string::npos)
	{
		if (mode[0] == '-')
		{
			base->ch.chlist[channel]->setRtopic(false);
			send_mode_toall(":" + base->usr.ppl[fd]->getNick() + " MODE " + channel + " -t \r\n", base, channel);
		}
		else if (mode[0] == '+')
		{
			base->ch.chlist[channel]->setRtopic(true);
			send_mode_toall(":" + base->usr.ppl[fd]->getNick() + " MODE " + channel + " +t \r\n", base, channel);
		}
	}
}

int	ft_klmode(string mode, string usr, string channel, sbase *base, int fd)
{
	std::stringstream ss;
	string s;

	if (mode.find("k") != string::npos)
	{
		if (mode[0] == '-')
		{
			base->ch.chlist[channel]->setKey(false);
			send_mode_toall(":" + base->usr.ppl[fd]->getNick() + " MODE " + channel + " -k \r\n", base, channel);
		}
		else if (mode[0] == '+')
		{
			if (usr.length() == 0)
				return (ft_mods_righuse("channel mode usage: mode #<channel name> +k <key>\r\n", fd));
			base->ch.chlist[channel]->setKey(true);
			send_mode_toall(":" + base->usr.ppl[fd]->getNick() + " MODE " + channel + " +k " + usr + "\r\n", base, channel);
			base->ch.chlist[channel]->setPass(usr);
		}
	}
	if (mode.find("l") != string::npos)
	{
		if (mode[0] == '-')
		{
			base->ch.chlist[channel]->setMax(false);
			send_mode_toall(":" + base->usr.ppl[fd]->getNick() + " MODE " + channel + " -l \r\n", base, channel);
		}
		else if (mode[0] == '+')
		{
			if (usr.length() == 0)
				return (ft_mods_righuse("channel l usage: mode #<channel name> +l <user limit> / mode -l #<channel>\r\n", fd));
			base->ch.chlist[channel]->setNMax(atoi(usr.c_str()));
			base->ch.chlist[channel]->setMax(true);
			ss << base->ch.chlist[channel]->getNMax();
			s = ss.str();
			send_mode_toall(":" + base->usr.ppl[fd]->getNick() + " MODE " + channel + " +l " + s +"\r\n", base, channel);
		}
	}
	return (0);
}

int	ft_omode(string mode, string usr, string channel, sbase *base, int fd)
{
	string response;

	if (usr.length() == 0)
		return (ft_mods_righuse("channel mode usage: mode #<channel name> +o <user>\r\n", fd));
	deque<string>::iterator it = base->ch.chlist[channel]->admins.begin();
	vector<string>::iterator i = base->ch.chlist[channel]->usrs.begin();
	while (i != base->ch.chlist[channel]->usrs.end())
	{
		if (*i == usr)
			break;
		i++;
	}
	if (i == base->ch.chlist[channel]->usrs.end())
		return (ft_mods_righuse("channel user is not in this channel!\r\n", fd));
	while (it != base->ch.chlist[channel]->admins.end())
	{
		if (*it == usr)
			break;
		it++;
	}
	if (it == base->ch.chlist[channel]->admins.end() && mode[0] == '-')
	{
		response = "channel " + usr + " is not an admin of this channel!\r\n";
		send(fd, response.c_str(), response.length(), 0);
	}
	else if (it != base->ch.chlist[channel]->admins.end() && mode[0] == '+')
	{
		response = "channel " + usr + " is already an admin of this channel!\r\n";
		send(fd, response.c_str(), response.length(), 0);
	}
	else if (it == base->ch.chlist[channel]->admins.end() && mode[0] == '+')
	{
		base->ch.chlist[channel]->admins.push_back(usr);
		send_mode_toall(":" + base->usr.ppl[fd]->getNick() + " MODE " + channel + " +o " + usr + "\r\n", base, channel);
	}
	else if (it != base->ch.chlist[channel]->admins.end() && mode[0] == '-')
	{
		base->ch.chlist[channel]->admins.erase(it);
		it = base->ch.chlist[channel]->admins.begin();
		send_mode_toall(":" + base->usr.ppl[fd]->getNick() + " MODE " + channel + " -o " + usr + "\r\n", base, channel);
	}
	return (0);
}

int	ft_mode(string message, sbase *base, int fd)
{
	std::stringstream ss;
	string channel;
	string mode;
	string usr;
	istringstream kk(message);

	kk >> channel;
	if (channel.find("#") == string::npos)
		return (ft_mods_righuse("channel mode usage: mode #<channel name> <+/-><i/t/k/o/l> (if needed)<user/user limit/key>\r\n", fd));
	kk >> mode;
	kk >> usr;
	if (base->ch.chlist.find(channel) == base->ch.chlist.end())
		return (0);
	if (is_channel_op(base, channel, fd) == false)
		return (0);
	if (base->ch.chlist.find(channel) == base->ch.chlist.end())
		return (ft_mods_righuse("channel No such channel!\r\n", fd));
	for (vector<string>::iterator it = base->ch.chlist[channel]->usrs.begin(); it != base->ch.chlist[channel]->usrs.end(); it++)
	{
		if (*it == base->usr.ppl[fd]->getNick())
			break;
		if (++it == base->ch.chlist[channel]->usrs.end())
			return (ft_mods_righuse("channel You are not in this channel!\r\n", fd));
	}
	ft_itmode(mode, channel, base, fd);
	if (ft_klmode(mode, usr, channel, base, fd) == -1)
		return (-1);
	if (mode.find("o") != string::npos)
		ft_omode(mode, usr, channel, base, fd);
	return (0);
}