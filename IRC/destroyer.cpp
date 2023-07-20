#include "irc.hpp"

void dell2(sbase *base, int fd)
{
	map<int, User*>::iterator it = base->usr.ppl.begin();
	vector<int>::iterator j = base->connected.begin();
	vector<int>::iterator l = base->nicked.begin();
	vector<int>::iterator k = base->registered.begin();
	map<int, User *>::iterator find = base->usr.ppl.begin();
	for (; it != base->usr.ppl.end(); it++)
	{
		for (; j != base->connected.end(); j++)
		{
			if (*j == fd)
			{
				base->connected.erase(j);
				break;
			}
		}
		for (; l !=  base->nicked.end(); l++)
		{
			if (*l == fd)
			{
				base->nicked.erase(l);
				break;
			}
		}
		for (; k !=  base->registered.end(); k++)
		{
			if (*k == fd)
			{	
				base->registered.erase(k);
				find = base->usr.ppl.find(fd);
				delete find->second; 
				base->usr.ppl.erase(find);
				return ;
			}
			
		}
	}
}

void searchAndDestroy(sbase *base, int fd)
{	
	check = 5;
	int checks = 0;
	map<int, User *>::iterator it = base->usr.ppl.begin();
	map<string, Channel *>::iterator i = base->ch.chlist.begin();
	for (; it != base->usr.ppl.end() && check != 1; it++)
	{
		if (check == 1)
			break ;
		if (it->first == fd)
		{
			for (; i != base->ch.chlist.end() && check != 1; i++)
			{
						if (check == 1)
							break ;
				for (vector<int>::iterator k = base->ch.chlist[i->first]->usrs_fds.begin(); k != base->ch.chlist[i->first]->usrs_fds.end(); k++)
				{
					if (*k == fd)
					{
						ft_checkOut(i->first, base, fd);
						checks  = 1;
						break;
					}	
				}
			}
		}
	}
	dell2(base, fd);
}

void ft_checkOut(string channel, sbase *base, int fd)
{
	string res;
	string ress;
	string full = channel;
	string trash;
	string reason;
	istringstream ss(full);
	ss >> channel;
	trash = "";
	while (ss)
	{
		ss >> trash;
		reason += trash;
		reason += " ";
		trash = "";
	}
	if (reason.c_str())
		reason.erase(reason.length() - 1, reason.length());
	if (base->ch.chlist.find(channel) == base->ch.chlist.end())
		return ;
	if (base->ch.chlist[channel]->usrs.size() == 0)
		return ;
	vector<string>::iterator i = base->ch.chlist[channel]->usrs.begin();
	map<int, User *>::iterator it = base->usr.ppl.begin();
 	string users;
	users = find_usrs(base, channel);
	i = base->ch.chlist[channel]->usrs.begin();
	it = base->usr.ppl.begin();
	string sName = "iRCServ";
	string msg2;
	string msg3;
	vector<string>::iterator ite = base->ch.chlist[channel]->usrs.begin();
	vector<int>::iterator eti = base->ch.chlist[channel]->usrs_fds.begin();
	
	for (;ite != base->ch.chlist[channel]->usrs.end(); ite++, eti++)
	{	
		res = ":" + base->usr.ppl[fd]->getNick() + "!" + " PART " + channel + " " + reason + "\n";
		send(*eti, res.c_str(), res.length(), 0); 

	}
/* 	while (it != base->usr.ppl.end())
	{
		if (ite == base->ch.chlist[channel]->usrs.end())
				break ;
		if (it->second->getNick() ==  *ite)
		{

		 		res = ":" + base->usr.ppl[fd]->getNick() + "!" + " PART " + channel + " " + reason + "\n";
	 		 	send(it->first, res.c_str(), res.length(), 0);   
				ite++;
				if (ite == base->ch.chlist[channel]->usrs.end())
					break ;
				it = base->usr.ppl.begin();
				continue ;
		}
		it++;
	} */
	deLL(base, fd, channel);
} 

void deLL(sbase *base, int fd, string channel)
{
	if (base->ch.chlist.find(channel) == base->ch.chlist.end())
		return ;
	vector<string>::iterator i = base->ch.chlist[channel]->usrs.begin();
	while (i != base->ch.chlist[channel]->usrs.end())
	{
 		if (*i == base->usr.ppl[fd]->getNick())
		{
			base->ch.chlist[channel]->usrs.erase(i);
			break;
		} 
		i++;
	}
	vector<int>::iterator it = base->ch.chlist[channel]->usrs_fds.begin();
	for (; it != base->ch.chlist[channel]->usrs_fds.end(); it++)
	{
		if (*it == fd)
		{
			base->ch.chlist[channel]->usrs_fds.erase(it);
			break ;
		}
	}
	deque<string>::iterator ite = base->ch.chlist[channel]->admins.begin();
	map<int, User *>::iterator k = base->usr.ppl.find(fd);
	for (; ite != base->ch.chlist[channel]->admins.end(); ite++)
	{
		if (*ite == k->second->getNick())
		{
			base->ch.chlist[channel]->admins.erase(ite);
			break;
		}
	}
	deque<string>::iterator ites = base->ch.chlist[channel]->invited.begin();
	map<int, User *>::iterator s = base->usr.ppl.begin();
	if (ites == base->ch.chlist[channel]->invited.end())
			return ;
	for (; s != base->usr.ppl.end(); s++)
	{
		ites = base->ch.chlist[channel]->invited.begin();
		for (; ites != base->ch.chlist[channel]->invited.end(); ites++)
		{
			if (*ites == s->second->getNick())
			{
				base->ch.chlist[channel]->invited.erase(ites);
				break;
			}
		}
	}
}

void ft_kicker(sbase *base, string message, int fd)
{
	string res;
	string channel;
	string kicked;
	string reason;
	string trash;
	string tmp = "";
	istringstream ss(message);
	
	ss >> trash;
	ss >> channel;
	ss >> kicked;
	while (ss)
	{
		ss >> tmp;
		reason += tmp;
		reason += " ";
		tmp = "";
	}
	if (base->ch.chlist.find(channel) == base->ch.chlist.end())
		return ;
	if (is_channel_op(base, channel, fd) == false)
		return ;
	reason.erase(reason.length() - 1, reason.length());
	vector<string>::iterator it = base->ch.chlist[channel]->usrs.begin();
	for (; it != base->ch.chlist[channel]->usrs.end(); it++)
	{
		if (*it == kicked)
			break ;
	}
	if (it == base->ch.chlist[channel]->usrs.end())
	{
		if (kicked == base->bot)
		{
			res = base->bot + " You can't kick me!\n";
			send(fd, res.c_str(), res.length(), 0);	 
		}
		res = "IRCSERVER " + kicked +"	 not in the channel!\r\n";
		send(fd, res.c_str(), res.length(), 0);
		return;
	}
	map<int, User *>::iterator i = base->usr.ppl.begin();
	it = base->ch.chlist[channel]->usrs.begin();
	res = ":" + base->usr.ppl[fd]->getNick() +  "!" + " KICK " + channel + " " + kicked + " " + reason + "\n"; 
	for (; i != base->usr.ppl.end(); i++)
	{
		it = base->ch.chlist[channel]->usrs.begin();
		for (; it != base->ch.chlist[channel]->usrs.end(); it++)
		{
			if (i->second->getNick() == *it)
				send(i->first, res.c_str(), res.length(), 0);
		}
	} 
	string ress = ":" + base->usr.ppl[fd]->getNick() + " KICK " + channel + " " + kicked + " " + reason + "\r\n";
	send(fd, ress.c_str(), ress.length(), 0);
	i = base->usr.ppl.begin();
	while (i != base->usr.ppl.end())
	{
		if (i->second->getNick() == kicked)
			deLL(base, i->first, channel);
		i++;
	}
}