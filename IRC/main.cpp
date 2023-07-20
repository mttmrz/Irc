/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarzano <mmarzano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 17:57:25 by bmuni             #+#    #+#             */
/*   Updated: 2023/06/07 18:38:38 by mmarzano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
	
void sendAll(sbase *base, string res)
{
	map<int, User *>::iterator it = base->usr.ppl.begin();
	for (; it != base->usr.ppl.end(); it++)
	{
		send(it->first, res.c_str(), res.length(), 0);
	}
	return ;
	
}

void changeInchannel(int fd, sbase *base, string old)
{
	map<int, User *>::iterator ite = base->usr.ppl.find(fd);
	map<string, Channel *>::iterator i = base->ch.chlist.begin();
	if (i == base->ch.chlist.end())
		return ;
	vector<string>::iterator o = base->ch.chlist[i->first]->usrs.begin();
	deque<string>::iterator d = base->ch.chlist[i->first]->admins.begin();
	for (; i != base->ch.chlist.end(); i++)
	{
		o = base->ch.chlist[i->first]->usrs.begin();
		for (; o !=  base->ch.chlist[i->first]->usrs.end(); o++)
		{
			if (*o == old)
			{
				base->ch.chlist[i->first]->usrs.erase(o);
				base->ch.chlist[i->first]->usrs.push_back(ite->second->getNick());
				break ;
			}
		}
		d = base->ch.chlist[i->first]->admins.begin();
		for (; d !=  base->ch.chlist[i->first]->admins.end(); d++)
		{
			if (*d == old)
			{
				base->ch.chlist[i->first]->admins.erase(d);
				base->ch.chlist[i->first]->admins.push_back(ite->second->getNick());
				break ;
			}
		}
		d = base->ch.chlist[i->first]->invited.begin();
		for (; d !=  base->ch.chlist[i->first]->invited.end(); d++)
		{
			if (*d == old)
			{
				base->ch.chlist[i->first]->invited.erase(d);
				base->ch.chlist[i->first]->invited.push_back(ite->second->getNick());
				break ;
			}
		}
	}
}


void ft_register(std::string nick, std::string usr, sbase *base, int fd)
{
	string res;
	if (base->usr.ppl.find(fd) != base->usr.ppl.end())
	{
		string oldNick = base->usr.ppl[fd]->getNick();
		nick.erase(nick.length() - 1, nick.length());
		base->usr.ppl[fd]->setNick(nick);
		if (ft_checkNick(nick, fd, base) != -1)
			change_admin_usr(base, base->usr.ppl[fd]->getNick(), oldNick);
		else
			base->usr.ppl[fd]->setAuth(false);
		res = ":" + oldNick + "!" + base->usr.ppl[fd]->getNick() + " NICK " + nick + "\n";
		send(fd, res.c_str(), res.length(), 0);
		sendAll(base, res);
		changeInchannel(fd, base, oldNick);
		return ;
	}
	else
	{
		User *a = new User();	
		a->setNick(nick);
		a->setUsr(usr);
		base->usr.ppl.insert(make_pair(fd, a));
	}
 	if (ft_checkNick(nick, fd, base) == -1)
		base->usr.ppl[fd]->setAuth(false);
	else
		base->usr.ppl[fd]->setAuth(true);
}

int ft_user(int fd, sbase *base)
{
	char buffer[1024];
	bzero(buffer, 1024);
	std::string tofind;
	std::string message;
	string users;
	int status = 0;
	istringstream ss(message);
	status = recv(fd, buffer, sizeof(buffer), 0);
	message = buffer;
	if (buffer[0] == '\0' || status <= 0)
		return (-1);
	if (message[message.length() - 1] != '\n')
	{
		base->saves[fd] += message;
		return (0);
	}
	else if (base->saves[fd].length() != 0)
	{
		base->saves[fd] += message;
		message = base->saves[fd];
		char a[1024];
		bzero(a, 1024);
		base->saves[fd] = a;
	}
 	if (base->usr.ppl.find(fd) == base->usr.ppl.end())
		return 0; 
 	if (base->usr.ppl[fd]->getAuth() == true)
	{

		if (message.find("PING") != string::npos)
		{
			ss >> tofind;
			ss >> tofind;
			message = "PONG :" + tofind + "\n";
			send(fd, message.c_str(), message.length() , 0);
			return (0);
		}
		else if (message[0] == '!')
			ft_bot(message, base, fd);
		else if (message.find("DCC") != string::npos)
			ft_dcc(message, base, fd);
		else if (message.find("PRIVMSG ") != string::npos)
		{
			if (ft_safe_words(message, fd, base) == -1)
				return (0);
			if (ft_messageToChannelCk(message, base) == 1)
				return (ft_messageCh(message, base, fd));
			if (message.find(":") == string::npos)
				return (0);
			tofind = message.erase(0, 8);
			find_rec(tofind, base, fd);
		}
		else if (message.find("NICK ") != string::npos)
		{
			tofind = message.erase(0, 5);
			ft_register(tofind, base->usr.ppl[fd]->getUsr(), base, fd);
		}
		else if (message.find("JOIN ") != string::npos)
			ft_channels(base, message.erase(0, 5), fd);
		else if (message.find("QUIT ") != string::npos)
			return (-1);
 		else if (message.find("PART ") != string::npos)
			ft_checkOut(message.erase(0, 5), base, fd);
		else if (message.find("INVITE ") != string::npos)
			ft_invite(message, base, fd);
		else if (message.find("TOPIC ") != string::npos)
			ft_change_topic(message, base, fd);
		else if (message.find("KICK ") != string::npos)
 			ft_kicker(base, message, fd); 
		else if (message.find("mode ") != string::npos)
			ft_mode(message.erase(0, 5), base, fd);
	}
	else
	{
		if (message.find("NICK ") != string::npos)
		{
			tofind = message.erase(0, 5);
			ft_register(tofind, base->usr.ppl[fd]->getUsr(), base, fd);
		}
		else
			send(fd, "You need to change the nick to a unique one to send messages!\r\n", 63, 0);
	}
	return (0);
} 

int ft_recive(sbase *base, int fd, char *buffer)
{
	string toparse;
	toparse = buffer;
	istringstream ss(toparse);
	string trash;
	string res;

	ss >> trash;
 	checkBuffer(buffer, fd, base); 
 	if (trash.find("PASS") != 0 && isIn(base, fd) == 0)
		return (1);
	else if (isIn(base, fd) == 0)
	{
		ss >> trash;
		if (trash.find(":") == 0)
				trash.erase(0, 1);
		if (trash == base->password)
		{
			if (isIn(base, fd) == 0)
				base->connected.push_back(fd);
			return (1);
		}
		else
		{
			if (check != 3)
			{
				res = "Invalid password, leaving the server...\n";
				send(fd, res.c_str(), res.length(), 0);
				close(fd);
			}
			return (1);
		}
	}
	else if (isIn(base, fd) == 1 && isNick(base, fd) == 0)
	{	
		if (trash.find("NICK") != 0)
		{	
			res = "Insert a nickname.\n";
			return (1);
		}
		ss >> trash;
		base->nick = trash;
		if (isNick(base, fd) == 0)
			base->nicked.push_back(fd);
		res = "IRC Welcome to the server, " + trash + "!\n" + base->rules +"\n";
		send(fd, res.c_str(), res.length(), 0);
		return (1);
	}
	else if (isReg(base, fd) == 0)
	{
		if (trash.find("USER") != 0)
		{
			res = "Insert a username.\n";
			return (1);
		}
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
		
	}
	ft_register(base->nick, base->ussr, base, fd);
	if (isReg(base, fd) == 0)
		base->registered.push_back(fd);
	return (0);

}

int	main(int ac, char **av, char **env)
{
	if (ac != 3)
		ft_error("usage is ./ircserv <port> <password>");
	if (ft_check_ar(av) == 1)
		return (1);
	sbase base;
	botName(&base);
	check = 0;
	base.port = atoi(av[1]);
	base.password = av[2];	
	ft_start(&base, env);
}