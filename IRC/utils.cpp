/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarzano <mmarzano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 15:40:58 by bmuni             #+#    #+#             */
/*   Updated: 2023/06/07 18:50:22 by mmarzano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void ft_sighand(int sig)
{
	check = 0;
	if (sig == SIGINT || sig == SIGQUIT || sig == SIGTSTP)
		check = 1;
	return ;
}

void	ft_error(std::string type)
{
	std::cerr << type << std::endl;
	exit(-1);
}

string	ft_toupper(string str)
{
	for (size_t i = 0; i < str.length(); i++)
		str[i] = toupper(str[i]);
	return (str);
}

void	change_admin_usr(sbase *base, string toCheck, string oldNick)
{
	map<string, Channel *>::iterator it = base->ch.chlist.begin();
	while (it != base->ch.chlist.end())
	{
		deque<string>::iterator k = it->second->admins.begin();
		while (k != it->second->admins.end())
		{
			if (*k == oldNick)
			{
				it->second->admins.erase(k);
				it->second->admins.push_back(toCheck);
			}
			k++;
		}
		it++;
	}
}

bool	is_channel_op(sbase *base, string channel, int fd)
{	
	if (base->ch.chlist.find(channel) == base->ch.chlist.end())
		return (false);
	deque<string>::iterator iter = base->ch.chlist[channel]->admins.begin();
	while (iter != base->ch.chlist[channel]->admins.end())
	{
		if (*iter == base->usr.ppl[fd]->getNick())
			break;
		iter++;
	}
	if (iter == base->ch.chlist[channel]->admins.end())
	{
		string res = channel + " You are not an admin of this channel!\r\n";
		send(fd, res.c_str(), res.length(), 0);
		return (false);
	}
	return (true);
}

int	multiple_rec(string tofind)
{
	if (tofind.find(" ") != string::npos)
		tofind = tofind.erase(tofind.find(" "), tofind.length());
	if (tofind.find(",") != string::npos)
		return (0);
	return (-1);
}

void	ft_freeall(sbase *base)
{
	cout << "Server shutting down..." << endl;
	map<int, User *>::iterator it = base->usr.ppl.begin();
	
 	while (it != base->usr.ppl.end())
	{
		delete it->second;
		it++;
	} 
	base->usr.ppl.clear();
	map<string, Channel *>::iterator i = base->ch.chlist.begin();
	while (i != base->ch.chlist.end())
	{
		delete i->second;
		i++;
	}
	base->ch.chlist.clear();
	close(base->sockfd);
	cout << "Server shut down." << endl;
	return;
}