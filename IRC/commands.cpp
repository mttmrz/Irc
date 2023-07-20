/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarzano <mmarzano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 15:38:50 by bmuni             #+#    #+#             */
/*   Updated: 2023/06/06 22:01:11 by mmarzano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	ft_change_topic(string comm, sbase *base, int fd)
{
	string channel;
	string topic;
	string response;
	string oldTopic;
	string trash;
	istringstream ss(comm);

	ss >> trash;
	ss >> channel;
	if (comm.find(":") == string::npos)
		return ;
	topic.assign(comm, comm.find(":") + 1, comm.length());
	if (channel.find("#") == string::npos)
	{
		send(fd, "Wrong format.\n", 15, 0);
		return ;
	}
	if (topic.find(":") != string::npos)
		topic.erase(0, 1);
	if (base->ch.chlist.find(channel) == base->ch.chlist.end())
		return ;
	if (base->ch.chlist[channel]->getRtopic() == true && is_channel_op(base, channel, fd) == false)
		return ;
	if (ft_safe_topic(topic, fd, base) == -1)
		return ;
	map<string, Channel *>::iterator it = base->ch.chlist.find(channel);
	if (it != base->ch.chlist.end())
	{
		oldTopic = it->second->getTopic();
		it->second->setTopic(topic);
		response = ":" + base->usr.ppl[fd]->getNick() + "!" + " TOPIC " + channel + " :" + topic + "\n";
		for (vector<int>::iterator it = base->ch.chlist[channel]->usrs_fds.begin(); it !=base->ch.chlist[channel]->usrs_fds.end(); it++)
			send(*it, response.c_str(), response.length(), 0);
	}
	else
		send(fd, "channel No such channel!\r\n", 26, 0);
}

void	ft_invite(string message, sbase *base, int fd)
{
	string channel;
	string user;
	string response;
	string oldTopic;
	istringstream ss(message);

	ss >> message;
	ss >> user;
	ss >> channel;
	cout << "MES" << message << "CH" << channel << "USR" <<  user << endl;
	if (user.size() == 0 || channel.size() == 0)
		return ;
	if (base->ch.chlist.find(channel) == base->ch.chlist.end())
		return ;
	map<string, Channel *>::iterator it = base->ch.chlist.find(channel);
	if (it != base->ch.chlist.end())
	{
		if (is_channel_op(base, channel, fd) == false)
			return ;
		map<int, User *>::iterator it = base->usr.ppl.begin();
		while (it != base->usr.ppl.end())
		{
			if (it->second->getNick() == user && ft_check_chmember(base, channel, user, fd) == -1)
			{	
				base->ch.chlist[channel]->invited.push_back(user);
				response = ":" + base->usr.ppl[fd]->getNick() + "!" + " INVITE " + user + " :" + channel + "\n";
				send(it->first, response.c_str(), response.length(), 0);
				break ;
			}
			it++;
		}
	}
	else
		send(fd, "channel No such channel!\r\n", 26, 0);
}

int	ft_dcc(string message, sbase *base, int fd)
{
	string name;
	
	
	string tmp = "";
	string msg;
	istringstream ss(message);
	while(ss)
	{
		ss >> tmp;
		msg += tmp;
		msg += " ";
		tmp = "";
	}
	if (message.find("PRIVMSG") != string::npos)
		message.erase(0, message.find(" ") + 1);
	name.assign(message, 0, message.find(" ")).c_str();
	map<int, User *>::iterator it = base->usr.ppl.begin();
	while (it != base->usr.ppl.end())
	{
		if (it->second->getNick() == name)
			break ;
		it++;
	}
	string popupMessage = ":" + base->usr.ppl[fd]->getNick() + "! " + msg + "\r\n";
	send(it->first, popupMessage.c_str(), strlen(popupMessage.c_str()), 0);
	return 0;	
}